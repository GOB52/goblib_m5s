/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_speaker.hpp
  @brief Make a sound
  @remark from Speaker_Class in M5Unified
*/
#pragma once
#ifndef GOBLIB_M5U_SPEAKER_HPP
#define GOBLIB_M5U_SPEAKER_HPP

#include <freertos/FreeRTOS.h> // BaseType_t
#include <freertos/queue.h> // xQueueHandle
#include <freertos/task.h> // TaskHandle_t
#include <freertos/semphr.h> // xSemaphoreHandle
#include <driver/i2s.h>
#include <atomic>

namespace goblib { class PcmStream; }

namespace goblib { namespace m5s {

/*! 
  @brief Modified from M5Unified Speaker_Class
  @sa https://github.com/m5stack/M5Unified
  @note This code from Speaker_Class in M5Unified
  @note Copyright (c) M5Stack. All rights reserved.
  @note Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/
class Speaker
{
  protected:
    constexpr static std::size_t BUF_LEN = 384; //

    static const uint8_t _default_tone_wav[16];
    
    /*! @brief Same as M5Unified m5::speaker_config_t */
    struct speaker_config_t
    {
        int pin_data_out = I2S_PIN_NO_CHANGE;
        int pin_bck = I2S_PIN_NO_CHANGE;
        int pin_ws = I2S_PIN_NO_CHANGE;
        uint32_t sample_rate = 64000;
        bool stereo = false;
        bool buzzer = false;
        bool use_dac = false;
        uint8_t dac_zero_level = 0;
        uint8_t magnification = 16;
        size_t dma_buf_len = 128;
        size_t dma_buf_count = 8;
        UBaseType_t task_priority = 2;
        BaseType_t task_pinned_core = -1;
        i2s_port_t i2s_port = i2s_port_t::I2S_NUM_0;

        speaker_config_t()
        {
            // goblib default settings, CPU0, priority0, buffer length BUF_LEN
            pin_data_out = 25;
            use_dac = true;
            dma_buf_len = BUF_LEN;
            task_priority = 1;
            task_pinned_core = 0;
        }
    };

    /*! @brief Same as M5Unified Speaker_Class::wav_info_t */
    struct wav_info_t
    {
        volatile uint32_t repeat = 0;   /// -1 mean infinity repeat
        uint32_t sample_rate_x256 = 0;
        const void* data = nullptr;
        size_t length = 0;
        union
        {
            volatile uint8_t flg = 0;
            struct
            {
                uint8_t is_stereo      : 1;
                uint8_t is_16bit       : 1;
                uint8_t is_signed      : 1;
                uint8_t stop_current   : 1;
                uint8_t no_clear_index : 1;
            };
        };
        void clear(void);
    };

    /*! @brief Same as M5Unified Speaker_Class::channel_info_t */
    struct channel_info_t
    {
        wav_info_t wavinfo[2]; // current/next flip info.
        size_t index = 0;
        int diff = 0;
        volatile uint8_t volume = 64; // channel volume (not master volume)
        volatile bool flip = false;

        bool liner_flip = false;
        float liner_buf[2][2] = { { 0, 0 }, { 0, 0 } };
    };

  public:
    using CallbackStreaming = void(*)(void*, uint8_t, bool);

    explicit Speaker(size_t max_channel = 8);
    ~Speaker();

    /// @name Same as M5Unified Speaker_Class
    /// @{
    speaker_config_t config(void) const { return _cfg; }
    void config(const speaker_config_t& cfg) { _cfg = cfg; }

    bool begin();
    void end();

    bool isRunning(void) const { return _task_handle != nullptr; }
    bool isEnabled(void) const { return _cfg.pin_data_out >= 0; }
    bool isPlaying(void) const volatile { return _play_channel_bits ? true : false; }
    size_t isPlaying(uint8_t channel) const volatile { return (channel < _sound_channel_max) ? ((bool)_ch_info[channel].wavinfo[0].repeat) + ((bool)_ch_info[channel].wavinfo[1].repeat) : 0; }
    size_t getPlayingChannels(void) const volatile { return __builtin_popcount(_play_channel_bits.load()); }
    
    void setVolume(uint8_t master_volume) { _master_volume = master_volume; }
    uint8_t getVolume(void) const { return _master_volume; }
    void setAllChannelVolume(uint8_t volume) { for (size_t ch = 0; ch < _sound_channel_max; ++ch) { _ch_info[ch].volume = volume; } }
    void setChannelVolume(uint8_t channel, uint8_t volume) { if (channel < _sound_channel_max) { _ch_info[channel].volume = volume; } }
    uint8_t getChannelVolume(uint8_t channel) const { return (channel < _sound_channel_max) ? _ch_info[channel].volume : 0; }

    void stop(void);
    void stop(uint8_t channel);

    bool tone(float frequency, uint32_t duration, int channel, bool stop_current_sound, const uint8_t* raw_data, size_t array_len, bool stereo = false)
    {
      return _play_raw(raw_data, array_len, false, false, frequency * (array_len >> stereo), stereo, (duration != ~0u) ? (duration * frequency / 1000) : ~0u, channel, stop_current_sound, true);
    }
    bool tone(float frequency, uint32_t duration = ~0u, int channel = -1, bool stop_current_sound = true) { return tone(frequency, duration, channel, stop_current_sound, _default_tone_wav, sizeof(_default_tone_wav), false); }

    bool playRaw(const int8_t* raw_data, size_t array_len, uint32_t sample_rate = 44100, bool stereo = false, uint32_t repeat = 1, int channel = -1, bool stop_current_sound = false)
    {
      return _play_raw(static_cast<const void* >(raw_data), array_len, false, true, sample_rate, stereo, repeat, channel, stop_current_sound, false);
    }
    bool playRaw(const uint8_t* raw_data, size_t array_len, uint32_t sample_rate = 44100, bool stereo = false, uint32_t repeat = 1, int channel = -1, bool stop_current_sound = false)
    {
        return _play_raw(static_cast<const void* >(raw_data), array_len, false, false, sample_rate, stereo, repeat, channel, stop_current_sound, false);
    }
    bool playRaw(const int16_t* raw_data, size_t array_len, uint32_t sample_rate = 44100, bool stereo = false, uint32_t repeat = 1, int channel = -1, bool stop_current_sound = false)
    {
      return _play_raw(static_cast<const void* >(raw_data), array_len, true, true, sample_rate, stereo, repeat, channel, stop_current_sound, false);
    }

    bool playWav(const uint8_t* wav_data, size_t data_len = ~0u, uint32_t repeat = 1, int channel = -1, bool stop_current_sound = false);

    /// @}

    //
    bool play(goblib::PcmStream& stream,uint32_t repeat = 1, int channel = -1);
    void pump();
    void setStreamingCallback(CallbackStreaming f, void* arg) { _callbackStreaming = f; _cbsArg = arg; }    

  protected:
    esp_err_t _setup_i2s(void);
    bool _play_raw(const void* wav, size_t array_len, bool flg_16bit, bool flg_signed, float sample_rate, bool flg_stereo, uint32_t repeat_count, int channel, bool stop_current_sound, bool no_clear_index);

    bool _set_next_wav(size_t ch, const wav_info_t& wav);
    void setCallback(void* args, bool(*func)(void*, bool)) { _cb_set_enabled = func; _cb_set_enabled_args = args; }
    
    /*! @brief stream inforamtion for streaming */
    struct stream_info_t
    {
        constexpr static std::size_t NUMBER_OF_BUF = 2;
        goblib::PcmStream* stream;
        std::size_t position; // read position (Same source may be played on multiple channels)
        uint32_t repeat; // argument forward play
        uint8_t buf[NUMBER_OF_BUF][BUF_LEN]; // read buffer
        uint8_t bindex; // read target

        stream_info_t() : stream(nullptr), position(0), repeat(0), buf{}, bindex(0) {}
        void clear() { stream = nullptr; position = 0; repeat = 0; bindex = false; }
        bool hasStream() const volatile{ return stream != nullptr; }
    };
    
  private:
    Speaker(const Speaker&) = delete;
    Speaker(Speaker&&) = delete;
    Speaker& operator=(const Speaker&) = delete;
    Speaker& operator=(Speaker&&) = delete;

    static void playing_task(void* arg) { static_cast<Speaker*>(arg)->_playing_task(); }
    void _playing_task();
    int getFreeChannel() const;
    
  private:
    const size_t _sound_channel_max;
    speaker_config_t _cfg;
    TaskHandle_t _task_handle;
    volatile SemaphoreHandle_t _task_semaphore;

    std::atomic<uint8_t> _play_channel_bits;
    volatile uint8_t _master_volume;

    channel_info_t* _ch_info;
    stream_info_t* _stream_info;

    bool (*_cb_set_enabled)(void* args, bool enabled);
    void* _cb_set_enabled_args;

    //
    CallbackStreaming _callbackStreaming; // for streaming
    void* _cbsArg;
};

//
}}
#endif
