/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_clock.cpp
  @brief  Custom clock using arduino/ESP-IDF functions.
*/
#include "gob_m5s_clock.hpp"
#include <Arduino.h>
#include <esp_timer.h>

namespace goblib { namespace m5s {

arduino_clock::time_point arduino_clock::now() noexcept
{
    return time_point( duration(static_cast<rep>(millis())) );
}

esp_clock::time_point esp_clock::now() noexcept
{
    return time_point( duration(static_cast<rep>(esp_timer_get_time())) );
}

//
}}
