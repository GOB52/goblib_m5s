/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_faces.cpp
  @brief  Get input from the Faces.
*/
#include "gob_m5s_faces.hpp"
#include "gob_utility.hpp" // goblib::to_underlying
#include "gob_suffix.hpp" // _u8
using goblib::suffix::operator"" _u8;

#include <Wire.h>
#include <esp_log.h>
#include <cassert>

namespace goblib { namespace m5s {

#if defined(GOBLIB_CPP14_OR_EARLIER)
const std::uint8_t FaceGB::REPEAT_CYCLE_OF_PUMP = 4;
#endif

FaceGB::FaceGB(std::uint8_t i2c_address, std::uint8_t pin_no)
        : _i2c_address(i2c_address), _pin_no(pin_no)
        , _last(0), _now(0), _edge(0), _edge_r(0), _repeat(0)
        , _repeatCount{0}
#ifdef GOBLIB_TEST_STUB
        , _record()
#endif
{
    _repeatCycle.fill(REPEAT_CYCLE_OF_PUMP);
}


void FaceGB::setup()
{
    pinMode(_pin_no, INPUT_PULLUP);

    auto r = Wire.requestFrom(_i2c_address, 1_u8);
    if(r == 0)
    {
        ESP_LOGE(__func__, "Call Wire.begin before FaceGB::setup().\n");
        return;
    }

    _last = _now = _edge = _edge_r = _repeat = 0;
    _repeatCount.fill(0);
    _repeatCycle.fill(REPEAT_CYCLE_OF_PUMP);
}

void FaceGB::pump()
{
    _last = _now;

#ifdef GOBLIB_TEST_STUB
    _now = readStubData();
#else
    if(digitalRead(_pin_no) == LOW)
    {
        _now = 0;
        Wire.requestFrom(_i2c_address, 1_u8);
        while(Wire.available())
        {
            _now = Wire.read() ^ 0xff;
        }
    }
#endif

    // detect edge
    _edge = (_last ^ _now) & _now;
    _edge_r = (_last ^ _now) & ~_now;

    // software repeat
    std::uint8_t k = 1;
    for(std::size_t i = 0; i < _repeatCount.size(); ++i, k <<= 1)
    {
        if(_edge & k)
        {
            _repeatCount[i] = _repeatCycle[i];
            _repeat |= k;
            continue;
        }
        if((_now & k) && _repeatCount[i]-- == 0)
        {
            _repeatCount[i] = _repeatCycle[i];
            _repeat |= k;
        }
        else
        {
            _repeat &= ~k;
        }
    }
}

std::uint8_t FaceGB::getRepeatCycle(const Button btn) const
{
    assert(__builtin_popcount(goblib::to_underlying(btn))==1 && "Invalid button value.");

    auto idx = __builtin_ffs(goblib::to_underlying(btn));
    if(idx > 0 && idx < _repeatCount.size())
    {
        return _repeatCycle[__builtin_ffs(goblib::to_underlying(btn)) - 1];
    }
    ESP_LOGE(__func__, "btn %x is unexpected.\n", btn);
    return 0;
}

void FaceGB::setRepeatCycle(Button btn, std::uint8_t cnt)
{
    assert(__builtin_popcount(goblib::to_underlying(btn))==1 && "Invalid button value.");
    
    auto mask = ~goblib::to_underlying(btn);
    _now &= mask;
    _edge &= mask;
    _edge_r &= mask;
    _repeat &= mask;

    auto idx = __builtin_ffs(goblib::to_underlying(btn));
    if(idx > 0 && idx < _repeatCount.size())
    {
        _repeatCycle[idx - 1] = cnt;
    }
    else
    {
        ESP_LOGE(__func__, "btn %x is unexpected.\n", btn);
    }
}

void FaceGB::setRepeatCycleAll( std::uint8_t cnt)
{
    std::fill(_repeatCycle.begin(), _repeatCycle.end(), cnt);
}

#ifdef GOBLIB_TEST_STUB
void FaceGB::setStubData(const ButtonType* data, std::size_t length)
{
    while(data && length-- > 0) { _record.push(*data++); }
}

std::uint8_t FaceGB::readStubData()
{
    std::uint8_t data = 0;
    if(!_record.empty())
    {
        data = _record.front();
        _record.pop();
    }
    return data;
}
#endif

//
}}
