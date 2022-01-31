/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_random.cpp
  @brief  Custom random engine using arduino functions.
*/
#include "gob_m5s_random.hpp"
#include <Arduino.h>
#ifdef ESP32
#include <esp_log.h>
#endif

namespace goblib { namespace m5s {

/*!
  @bug srand() call in randmSeed() and lost 40bytes of heap on ESP32. Is it bug?
*/
void arduino_engine::seed(result_type s)
{
    #ifdef ESP32
    ESP_LOGW(__func__, "ESP32 uses hardware RNG in random(), so seed is ignored.");
    #endif
    ::randomSeed(s);
}

arduino_engine::result_type arduino_engine::operator()()
{
    return ::random(max());
}

void arduino_engine::discard(unsigned long long z)
{
    while(z-- > 0ULL) { (*this)(); }
}

//
}}
