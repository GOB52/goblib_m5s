/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_thread.hpp
  @brief  Alternate std::this_thread function
*/
#ifndef GOBLIB_M5S_THREAD_HPP
#define GOBLIB_M5S_THREAD_HPP

#include <Arduino.h>
#include <chrono>
#include "gob_m5s_clock.hpp"
#include <gob_macro.hpp>

namespace goblib { namespace m5s {
/*!
  @brief Alternate std::this_thread
*/
namespace this_thread
{
/*! 
  @brief Blocks the execution of the current thread for at least the specified sleep_duration. 
  @param sleep_duration time duration to sleep. 
  @remark std::this_thread::sleep_for in ESP-IDF does not meet specifications.
  Return before the sleep duration.
  @sa https://en.cppreference.com/w/cpp/thread/sleep_for
*/
template <class Rep, class Period> void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
{
#if 1
    using tclock = goblib::m5s::esp_clock;
    auto from = tclock::now();
    auto us = std::chrono::duration_cast<tclock::duration>(sleep_duration); // to us

    if(us.count() > 0)
    {
        delay(us.count() / 1000); // ms
    }
    while(tclock::now() - from < us) { taskYIELD(); } // fraction (us)
#else
    auto s = std::chrono::steady_clock::now();
    while(std::chrono::steady_clock::now() - s < sleep_duration)
    {
        taskYIELD();
    }
#endif
}

//
}}}
#endif
