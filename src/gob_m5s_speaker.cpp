/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_speaker.cpp
  @brief Make a sound
  @remark from Speaker_Class in M5Unified
*/
#include <Arduino.h>

#include "gob_pcm_stream.hpp"
#include "gob_m5s_speaker.hpp"
#include <gob_utility.hpp>

#include <esp_log.h>
#include <driver/gpio.h>

#include <cstring>

#if defined (ESP_IDF_VERSION_VAL)
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
#define COMM_FORMAT_I2S (I2S_COMM_FORMAT_STAND_I2S)
#define COMM_FORMAT_MSB (I2S_COMM_FORMAT_STAND_MSB)
#endif
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 3, 3)
#define SAMPLE_RATE_TYPE uint32_t
#endif
#endif

#ifndef COMM_FORMAT_I2S
#define COMM_FORMAT_I2S (I2S_COMM_FORMAT_I2S)
#define COMM_FORMAT_MSB (I2S_COMM_FORMAT_I2S_MSB)
#endif

#ifndef SAMPLE_RATE_TYPE
#define SAMPLE_RATE_TYPE int
#endif


namespace goblib
{
/*! 
  @brief From M5GFX
  @sa https://github.com/m5stack/M5GFX
  @note This code from M5GFX
  @note Copyright (c) M5Stack. All rights reserved.
  @note Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/
namespace m5gfx
{

enum pin_mode_t
{
    output
    , input
    , input_pullup
    , input_pulldown
};

static inline volatile uint32_t* get_gpio_hi_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1ts.val : &GPIO.out_w1ts; }
static inline volatile uint32_t* get_gpio_lo_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1tc.val : &GPIO.out_w1tc; }
static inline bool gpio_in(int_fast8_t pin) { return ((pin & 32) ? GPIO.in1.data : GPIO.in) & (1 << (pin & 31)); }
static inline void gpio_hi(int_fast8_t pin) { if (pin >= 0) *get_gpio_hi_reg(pin) = 1 << (pin & 31); }
static inline void gpio_lo(int_fast8_t pin) { if (pin >= 0) *get_gpio_lo_reg(pin) = 1 << (pin & 31); }

static void pinMode(int_fast16_t pin, pin_mode_t mode)
{
    if (pin < 0) return;

#if !defined (CONFIG_IDF_TARGET) || defined (CONFIG_IDF_TARGET_ESP32)
    if (pin < 6 || pin > 11)
#endif
    {
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_DISABLE);
    }
#if defined (ARDUINO)
    int m;
    switch (mode)
    {
    case pin_mode_t::output:         m = OUTPUT;         break;
    default:
    case pin_mode_t::input:          m = INPUT;          break;
    case pin_mode_t::input_pullup:   m = INPUT_PULLUP;   break;
    case pin_mode_t::input_pulldown: m = INPUT_PULLDOWN; break;
    }
    ::pinMode(pin, m);
#else

#if SOC_RTCIO_INPUT_OUTPUT_SUPPORTED
    if (rtc_gpio_is_valid_gpio((gpio_num_t)pin)) rtc_gpio_deinit((gpio_num_t)pin);
#endif
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask = (std::uint64_t)1 << pin;
    switch (mode)
    {
    case pin_mode_t::output:
        io_conf.mode = GPIO_MODE_OUTPUT;
        break;
    default:
        io_conf.mode = GPIO_MODE_INPUT;
        break;
    }
    io_conf.mode         = (mode == pin_mode_t::output) ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;
    io_conf.pull_down_en = (mode == pin_mode_t::input_pulldown) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en   = (mode == pin_mode_t::input_pullup  ) ? GPIO_PULLUP_ENABLE   : GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
#endif
}
//
}}

namespace goblib { namespace m5s {

const uint8_t Speaker::_default_tone_wav[14] = { 184, 230, 255, 255, 230, 184, 128, 72, 26, 1, 1, 26, 72, 128 }; // sin wave data

Speaker::Speaker(size_t max_channel)
        : _sound_channel_max(max_channel)
        , _cfg{}, _task_handle(nullptr), _task_semaphore(nullptr)
        , _play_channel_bits(0)
        , _master_volume(64)
        , _ch_info(nullptr)
        , _stream_info(nullptr)
        , _cb_set_enabled(nullptr)
        , _cb_set_enabled_args(nullptr)
        , _callbackStreaming(nullptr)
        , _cbsArg(nullptr)
{
    assert(max_channel > 0 && "max_channel muset be greater than zero");
    assert(max_channel <= 8 && "max_channel muset be lesser equal 8"); // max <= bits of _play_channel_bits
    
    _ch_info = new channel_info_t[_sound_channel_max];
    assert(_ch_info);
    _stream_info = new stream_info_t[_sound_channel_max];
    assert(_stream_info);
}


Speaker::~Speaker()
{
    end();
    goblib::safeDeleteArray(_stream_info);
    goblib::safeDeleteArray(_ch_info);
    vSemaphoreDelete(_task_semaphore);
}

esp_err_t Speaker::_setup_i2s(void)
{
    i2s_driver_uninstall(_cfg.i2s_port);

    if (_cfg.pin_data_out < 0) { return ESP_FAIL; }

    SAMPLE_RATE_TYPE sample_rate = _cfg.sample_rate;

    /*
      ESP-IDF ver4系にて I2S_MODE_DAC_BUILT_IN を使用するとサンプリングレートが正しく反映されない不具合があったため、特殊な対策を実装している。
      ・指定するサンプリングレートの値を1/16にする
      ・I2S_MODE_DAC_BUILT_INを使用せずに初期化を行う
      ・最後にI2S0のレジスタを操作してDACモードを有効にする。
    */
    if (_cfg.use_dac) { sample_rate >>= 4; }

    i2s_config_t i2s_config;
    memset(&i2s_config, 0, sizeof(i2s_config_t));
    i2s_config.mode                 = (i2s_mode_t)( I2S_MODE_MASTER | I2S_MODE_TX );
    i2s_config.sample_rate          = sample_rate;
    i2s_config.bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.channel_format       = _cfg.stereo
            ? I2S_CHANNEL_FMT_RIGHT_LEFT
            : I2S_CHANNEL_FMT_ONLY_RIGHT;
    i2s_config.communication_format = (i2s_comm_format_t)( COMM_FORMAT_I2S );
    i2s_config.dma_buf_count        = _cfg.dma_buf_count;
    i2s_config.dma_buf_len          = _cfg.dma_buf_len;
    i2s_config.tx_desc_auto_clear   = true;

    i2s_pin_config_t pin_config;
    memset(&pin_config, ~0u, sizeof(i2s_pin_config_t)); /// all pin set to I2S_PIN_NO_CHANGE
    pin_config.bck_io_num     = _cfg.pin_bck;
    pin_config.ws_io_num      = _cfg.pin_ws;
    pin_config.data_out_num   = _cfg.pin_data_out;

    esp_err_t err = i2s_driver_install(_cfg.i2s_port, &i2s_config, 0, nullptr);
    if (err != ESP_OK) { return err; }

    i2s_zero_dma_buffer(_cfg.i2s_port);

#if !defined (CONFIG_IDF_TARGET) || defined (CONFIG_IDF_TARGET_ESP32)
    if (_cfg.use_dac)
    {
        i2s_dac_mode_t dac_mode = i2s_dac_mode_t::I2S_DAC_CHANNEL_BOTH_EN;
        if (!_cfg.stereo)
        {
            dac_mode = (_cfg.pin_data_out == GPIO_NUM_25)
                    ? i2s_dac_mode_t::I2S_DAC_CHANNEL_RIGHT_EN // for GPIO 25
                    : i2s_dac_mode_t::I2S_DAC_CHANNEL_LEFT_EN; // for GPIO 26
        }
        err = i2s_set_dac_mode(dac_mode);
        if (_cfg.i2s_port == I2S_NUM_0)
        { /// レジスタを操作してDACモードの設定を有効にする ;
            I2S0.conf2.lcd_en = true;
            I2S0.conf.tx_right_first = true;
            I2S0.conf.tx_msb_shift = 0;
            I2S0.conf.tx_short_sync = 0;
        }
    }
    else
#endif
    {
        err = i2s_set_pin(_cfg.i2s_port, &pin_config);
    }

    return err;
}

void Speaker::_playing_task()
{
    i2s_port_t i2s_port = _cfg.i2s_port;

    const bool out_stereo = _cfg.stereo;
    const int32_t spk_sample_rate = _cfg.sample_rate;
    const size_t dma_buf_len = _cfg.dma_buf_len & ~1;
    int nodata_count = 0;
    int32_t dac_offset = _cfg.dac_zero_level << 8;
    bool flg_i2s_started = false;
    bool in_stereo = false;
    float ch_v = 0.0f;
    const float magnification = (float)_cfg.magnification 
            / (~0u >> ((_cfg.use_dac || _cfg.buzzer) ? 0 : 8));
    const void* data = nullptr;

    union
    {
        int16_t surplus16 = 0;
        uint8_t surplus[2];
    };

    union
    {
        float* float_buf;
        int16_t* sound_buf;
    };
    float_buf = (float*)alloca(dma_buf_len * sizeof(float));

    for(;;)
    {
        if (++nodata_count > _cfg.dma_buf_count * 2)
        {
            nodata_count = 0;
            flg_i2s_started = false;
            i2s_stop(i2s_port);
#if !defined (CONFIG_IDF_TARGET) || defined (CONFIG_IDF_TARGET_ESP32)
            if (_cfg.use_dac)
            {
                i2s_set_dac_mode(i2s_dac_mode_t::I2S_DAC_CHANNEL_DISABLE);
            }
#endif
            ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
            continue;
        }

        memset(float_buf, 0, dma_buf_len * sizeof(float));

        for (size_t ch = 0; ch < _sound_channel_max; ++ch)
        {
            if (0 == (_play_channel_bits.load() & (1 << ch))) { continue; }
            nodata_count = 0;

            auto ch_info = &(_ch_info[ch]);
            wav_info_t* current_wav = &(ch_info->wavinfo[!ch_info->flip]);
            wav_info_t* next_wav = &(ch_info->wavinfo[ch_info->flip]);

            size_t idx = 0;
            int ch_diff = ch_info->diff;

            if (current_wav->repeat == 0 || next_wav->stop_current)
            {
          label_next_wav:
                bool clear_idx = (next_wav->repeat == 0
                                  || !next_wav->no_clear_index
                                  || (next_wav->data != current_wav->data));
                current_wav->clear();
                ch_info->flip = !ch_info->flip;
                xSemaphoreGive(_task_semaphore);
                std::swap(current_wav, next_wav);

                if (clear_idx)
                {
                    ch_info->index = 0;
                    if (current_wav->repeat == 0)
                    {
                        // Has stream?
                        if(_stream_info[ch].hasStream()) { continue; }

                        _play_channel_bits.fetch_and(~(1 << ch));
                        if (current_wav->repeat == 0)
                        {
                            ch_info->diff = 0;
                            continue;
                        }
                        _play_channel_bits.fetch_or(1 << ch);
                    }
                }
            }
            data = current_wav->data;
            in_stereo = current_wav->is_stereo;
            int32_t tmp = ch_info->volume;
            tmp *= tmp;
            if (!current_wav->is_16bit) { tmp <<= 8; }
            if (out_stereo) { tmp <<= 1; }
            ch_v = (float)tmp / spk_sample_rate;

            bool liner_flip = ch_info->liner_flip;
            auto liner_base = ch_info->liner_buf[ liner_flip];
            auto liner_next = ch_info->liner_buf[!liner_flip];
            do
            {
                if (ch_diff >= 0)
                {
                    size_t ch_index = ch_info->index;
                    do
                    {
                        if (ch_index >= current_wav->length)
                        {
                            ch_index -= current_wav->length;
                            if (current_wav->repeat && current_wav->repeat != ~0u)
                            {
                                if (--current_wav->repeat == 0)
                                {
                                    ch_info->index = ch_index;
                                    ch_info->liner_flip = (liner_next < liner_base);

                                    goto label_next_wav;
                                }
                            }
                        }
                        int32_t l, r;
                        if (current_wav->is_16bit)
                        {
                            auto wav = (const int16_t*)data;
                            l = wav[ch_index];
                            r = wav[ch_index += in_stereo];
                            ++ch_index;
                            if (!current_wav->is_signed)
                            {
                                l = (l & 0xFFFF) + INT16_MIN;
                                r = (r & 0xFFFF) + INT16_MIN;
                            }
                        }
                        else
                        {
                            auto wav = (const uint8_t*)data;
                            l = wav[ch_index];
                            r = wav[ch_index += in_stereo];
                            ++ch_index;
                            if (current_wav->is_signed)
                            {
                                l = (int8_t)l;
                                r = (int8_t)r;
                            }
                            else
                            {
                                l += INT8_MIN;
                                r += INT8_MIN;
                            }
                        }
                        std::swap(liner_base, liner_next);

                        if (!out_stereo) { l += r; }
                        else
                        {
                            liner_base[1] = r * ch_v;
                        }
                        liner_base[0] = l * ch_v;

                        ch_diff -= spk_sample_rate;
                    } while (ch_diff >= 0);
                    ch_info->index = ch_index;
                }

                const auto in_rate = current_wav->sample_rate;
                float base_l = liner_base[0];
                float next_l = base_l - liner_next[0];
                base_l *= spk_sample_rate;
                if (out_stereo)
                {
                    float base_r = liner_base[1];
                    float next_r = base_r - liner_next[1];
                    base_r *= spk_sample_rate;
                    do
                    {
                        float_buf[  idx] += next_l * ch_diff + base_l;
                        float_buf[++idx] += next_r * ch_diff + base_r;
                        ch_diff += in_rate;
                    } while (++idx < dma_buf_len && ch_diff < 0);
                }
                else
                {
                    do
                    {
                        float_buf[idx] += next_l * ch_diff + base_l;
                        ch_diff += in_rate;
                    } while (++idx < dma_buf_len && ch_diff < 0);
                }
            } while (idx < dma_buf_len);
            ch_info->liner_flip = (liner_next < liner_base);
            ch_info->diff = ch_diff;
        }

        float volume = magnification * (_master_volume * _master_volume);
        size_t idx = 0;
        if (_cfg.use_dac)
        {
            const bool zero_bias = (_cfg.dac_zero_level == 0);
            if (zero_bias && nodata_count)
            {
                do
                {
                    dac_offset = (dac_offset * 255) >> 8;
                    int32_t v = dac_offset + surplus[idx & out_stereo];
                    surplus[idx & out_stereo] = v;
                    sound_buf[idx ^ 1] = v;
                } while (++idx < dma_buf_len);
            }
            else
            {
                if (zero_bias) { dac_offset -= dma_buf_len >> 5; }
                do
                {
                    int32_t v = volume * float_buf[idx];
                    if (zero_bias)
                    {
                        int32_t vabs = abs(v);
                        if (dac_offset < vabs)
                        {
                            dac_offset = (INT16_MAX < vabs) ? INT16_MAX : vabs;
                        }
                    }
                    v += dac_offset;
                    auto s = &surplus[idx & out_stereo];
                    v += *s;
                    *s = v;

                    if (v < 0) { v = 0; }
                    else if (v > UINT16_MAX) { v = UINT16_MAX; }
                    sound_buf[idx ^ 1] = v;
                } while (++idx < dma_buf_len);
            }
        }
        else if (_cfg.buzzer)
        {
            if (nodata_count)
            {
                memset(sound_buf, 0, dma_buf_len << 1);
                surplus16 = 0;
            }
            else
            {
                int32_t tmp = surplus16;
                do
                {
                    int32_t v = volume * float_buf[idx];
                    v += 0x8000;
                    uint_fast16_t bitdata = 0;
                    uint_fast16_t bit = 0x8000;
                    do
                    {
                        if ((tmp += v) >= 0x8000)
                        {
                            tmp -= 0x10000;
                            bitdata |= bit;
                        }
                    } while (bit >>= 1);
                    sound_buf[idx ^ 1] = bitdata;
                } while (++idx < dma_buf_len);
                surplus16 = tmp;
            }
        }
        else
        {
            do
            {
                int32_t v = volume * float_buf[idx];
                v += surplus[idx & out_stereo];
                surplus[idx & out_stereo] = v;
                v >>= 8;
                if (v < INT16_MIN) { v = INT16_MIN; }
                else if (v > INT16_MAX) { v = INT16_MAX; }
                sound_buf[idx ^ 1] = v;
            } while (++idx < dma_buf_len);
        }

        if (!flg_i2s_started)
        {
            flg_i2s_started = true;
#if !defined (CONFIG_IDF_TARGET) || defined (CONFIG_IDF_TARGET_ESP32)
            if (_cfg.use_dac)
            {
                i2s_dac_mode_t dac_mode = i2s_dac_mode_t::I2S_DAC_CHANNEL_BOTH_EN;
                if (!_cfg.stereo)
                {
                    dac_mode = (_cfg.pin_data_out == GPIO_NUM_25)
                            ? i2s_dac_mode_t::I2S_DAC_CHANNEL_RIGHT_EN // for GPIO 25
                            : i2s_dac_mode_t::I2S_DAC_CHANNEL_LEFT_EN; // for GPIO 26
                }
                i2s_set_dac_mode(dac_mode);
            }
#endif
            i2s_start(i2s_port);
        }

        size_t write_bytes;
        i2s_write(i2s_port, sound_buf, dma_buf_len * sizeof(int16_t), &write_bytes, portMAX_DELAY);
    }
}

bool Speaker::begin()
{
    if(isRunning()) { return true; }
    if (_task_semaphore == nullptr) { _task_semaphore = xSemaphoreCreateBinary(); }

#if 0
    printf("SConfig: pin_data_out:%d pin_bck:%d pin_ws:%d sample_rate:%u stereo:%d buzzer:%d use_dac:%d dac_zero_level:%u mag:%u dma_buf_len:%zu dma_buf_count:%zu task:%d pinn:%d port:%d\n",
           _cfg.pin_data_out,
           _cfg.pin_bck,
           _cfg.pin_ws,
           _cfg.sample_rate,
           _cfg.stereo,
           _cfg.buzzer,
           _cfg.use_dac,
           _cfg.dac_zero_level,
           _cfg.magnification,
           _cfg.dma_buf_len,
           _cfg.dma_buf_count,
           _cfg.task_priority,
           _cfg.task_pinned_core,
           _cfg.i2s_port);
#endif

    bool res = true;
    if (_cb_set_enabled) { res = _cb_set_enabled(_cb_set_enabled_args, true); }

    res = (ESP_OK == _setup_i2s()) && res;
    if (res)
    {
        size_t stack_size = 1024 + (_cfg.dma_buf_len * sizeof(float));
        if ((size_t)_cfg.task_pinned_core < portNUM_PROCESSORS)
        {
            xTaskCreatePinnedToCore(playing_task, "playing_task", stack_size, this, _cfg.task_priority, &_task_handle, _cfg.task_pinned_core);
        }
        else
        {
            xTaskCreate(playing_task, "playing_task", stack_size, this, _cfg.task_priority, &_task_handle);
        }
    }
    return _task_handle != nullptr;
}

void Speaker::end()
{
    if(!isRunning()) { return; }
    if(_task_handle) { vTaskDelete(_task_handle); _task_handle = nullptr; }

    i2s_stop(_cfg.i2s_port);
#if !defined (CONFIG_IDF_TARGET) || defined (CONFIG_IDF_TARGET_ESP32)
    if (_cfg.use_dac)
    {
        i2s_set_dac_mode(i2s_dac_mode_t::I2S_DAC_CHANNEL_DISABLE);
        goblib::m5gfx::gpio_lo(_cfg.pin_data_out);
        goblib::m5gfx::pinMode(_cfg.pin_data_out, goblib::m5gfx::pin_mode_t::output);
    }
#endif
    stop();
    if (_cb_set_enabled) { _cb_set_enabled(_cb_set_enabled_args, false); }
}

void Speaker::stop(void)
{
    wav_info_t tmp;
    tmp.stop_current = 1;
    for (size_t ch = 0; ch < _sound_channel_max; ++ch)
    {
        auto chinfo = &_ch_info[ch];
        chinfo->wavinfo[chinfo->flip] = tmp;
        auto sinfo = &_stream_info[ch];
        sinfo->clear();
    }
}

void Speaker::stop(uint8_t ch)
{
    if ((size_t)ch >= _sound_channel_max)
    {
        stop();
    }
    else
    {
        wav_info_t tmp;
        tmp.stop_current = 1;
        auto chinfo = &_ch_info[ch];
        chinfo->wavinfo[chinfo->flip] = tmp;
        auto sinfo = &_stream_info[ch];
        sinfo->clear();
    }
}

void Speaker::wav_info_t::clear(void)
{
    length = 0;
    data = nullptr;
    sample_rate = 0;
    flg = 0;
    repeat = 0;
}

bool Speaker::_set_next_wav(size_t ch, const wav_info_t& wav)
{
    auto chinfo = &_ch_info[ch];
    uint8_t chmask = 1 << ch;
    if (!wav.stop_current)
    {
        if (isPlaying(ch) > 1) { return false; }
        while ((_play_channel_bits.load() & chmask) && (chinfo->wavinfo[chinfo->flip].repeat)) { xSemaphoreTake(_task_semaphore, 1); }
    }
    chinfo->wavinfo[chinfo->flip] = wav;
    _play_channel_bits.fetch_or(chmask);

    xTaskNotifyGive(_task_handle);
    return true;
}

bool Speaker::_play_raw(const void* data, size_t array_len, bool flg_16bit, bool flg_signed, uint32_t sample_rate, bool flg_stereo, uint32_t repeat_count, int channel, bool stop_current_sound, bool no_clear_index)   
{
    if (!begin() || (_task_handle == nullptr)) { return true; }
    if (array_len == 0) { return true; }
    size_t ch = (size_t)channel;
    if (ch >= _sound_channel_max)
    {
        size_t bits = _play_channel_bits.load();
        for (ch = _sound_channel_max - 1; ch < _sound_channel_max; --ch)
        {
            if (0 == ((bits >> ch) & 1)) { break; }
        }
        if (ch >= _sound_channel_max) { return false; }
    }
    wav_info_t info;
    info.data = data;
    info.length = array_len;
    info.repeat = repeat_count ? repeat_count : ~0u;
    info.sample_rate = sample_rate;
    info.is_stereo = flg_stereo;
    info.is_16bit = flg_16bit;
    info.is_signed = flg_signed;
    info.stop_current = stop_current_sound;
    info.no_clear_index = no_clear_index;

    return _set_next_wav(ch, info);
}

bool Speaker::play(goblib::PcmStream& stream,uint32_t repeat, int channel)
{
    if (!stream.valid() || !begin() || (_task_handle == nullptr)) { return false; }

    size_t ch = (size_t)channel;
    if (ch >= _sound_channel_max)
    {
        size_t bits = _play_channel_bits.load();
        for (ch = _sound_channel_max - 1; ch < _sound_channel_max; --ch)
        {
            if (0 == ((bits >> ch) & 1)) { break; }
        }
        if (ch >= _sound_channel_max) { return true; }
    }

    auto& sinfo = _stream_info[ch];
    sinfo.stream = &stream;
    sinfo.repeat = repeat ? repeat : ~0u;
    sinfo.bindex = true; // next
    
    stream.rewind();
    auto len = stream.read(sinfo.buf[0], sizeof(sinfo.buf[0]));
    sinfo.position = len;

    bool f16 = stream.bitsPerSample() >> 4;
    wav_info_t info;
    info.data = sinfo.buf[0];
    info.length = len >> f16;
    info.repeat = 1;
    info.sample_rate = stream.sampleRate();
    info.is_stereo = stream.channels() >= 2;
    info.is_16bit = f16;
    info.is_signed = f16;
    info.stop_current = false;
    info.no_clear_index = true;

    if(!_set_next_wav(ch, info))
    {
        stream.rewind();
        sinfo.clear();
        return false;
    }

    if(_callbackStreaming) { _callbackStreaming(_cbsArg, ch, true); }
    return true;
}

void Speaker::pump()
{
    if(!isRunning() || !isPlaying()) { return; }

    for(int ch = 0; ch < _sound_channel_max; ++ch)
    {
        if((_play_channel_bits.load() & (1 << ch)) == 0) { continue; }

        channel_info_t* ch_info;
        ch_info = &_ch_info[ch];
        wav_info_t* next_wav = &(ch_info->wavinfo[ch_info->flip]);
        if(next_wav->data) { continue; }

        auto& sinfo = _stream_info[ch];
        auto s = sinfo.stream;

        if(!s) { continue; }

        auto old_pos = s->position();

        s->seek(sinfo.position);
        auto len = s->read(sinfo.buf[ sinfo.bindex ], sizeof(sinfo.buf[0]));
        sinfo.position += len;

        if(s->is_tail())
        {
            if(sinfo.repeat == ~0u || --sinfo.repeat)
            {
                s->rewind();
                sinfo.position = 0;
                if(len < _cfg.dma_buf_len)
                {
                    sinfo.position = s->read(sinfo.buf[ sinfo.bindex ] + len, _cfg.dma_buf_len - len);
                    len += sinfo.position;
                }
            }
            else
            {
                // End of streaming
                if(_callbackStreaming) { _callbackStreaming(_cbsArg, ch, false); }
                sinfo.clear();
            }
        }

        if(len > 0)
        {
            bool f16 = s->bitsPerSample() >> 4;
            wav_info_t info;
            info.data = sinfo.buf[sinfo.bindex];
            info.length = len >> f16;
            info.repeat = 1;
            info.sample_rate = s->sampleRate();
            info.is_stereo = s->channels() >= 2;
            info.is_16bit = f16;
            info.is_signed = f16;
            info.stop_current = false;
            info.no_clear_index = true;

            if(_set_next_wav(ch, info))
            {
                sinfo.bindex = !sinfo.bindex;
            }
            else
            {
                s->seek(old_pos);
                sinfo.position = old_pos;
            }
        }
    }
}

//
}}

