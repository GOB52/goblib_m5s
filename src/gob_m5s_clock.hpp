/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_clock.hpp
  @brief  Custom clock using arduino/ESP-IDF functions.
*/
#pragma once
#ifndef GOBLIB_M5S_CLOCK_HPP
#define GOBLIB_M5S_CLOCK_HPP

#include <chrono>

namespace goblib { namespace m5s {

/*!
  arduino_clock
  @brief Custom clock compatible with std::chrono clock using ardiono functions.
*/
struct arduino_clock
{
    /// @name Typedef compatible with std::chrono::any_clock
    ///@{
    using duration = std::chrono::milliseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<arduino_clock>;
    /// @}

    static const bool is_steady = true;
    static time_point now() noexcept;
};


/*!
  esp32_clock
  @brief Custom clock compatible with std::chrono clock using ESP-IDF functions.
*/
struct esp_clock
{
    /// @name Typedef compatible with std::chrono::any_clock
    ///@{
    using duration = std::chrono::microseconds;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<esp_clock>;
    /// @}

    static const bool is_steady = true;
    static time_point now() noexcept;
};

//
}}
#endif
