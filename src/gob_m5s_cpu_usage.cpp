/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_cpu_usage.cpp
  @brief  Calculate CPU usage using ESP32 functions.
*/
#if defined(ESP32)

#include "gob_m5s_cpu_usage.hpp"
#include <freertos/portmacro.h> // UBaseType_t;
#include <esp_freertos_hooks.h>
#include <esp_log.h>
#include <mutex>

namespace goblib { namespace m5s {

long CpuUsage::_idle_cnt_0;
long CpuUsage::_tick_cnt_0;
long CpuUsage::_idle_cnt_1;
long CpuUsage::_tick_cnt_1;
bool CpuUsage::_initialized = false;

void CpuUsage::setup()
{
    if(!_initialized)
    {

        if(esp_register_freertos_idle_hook_for_cpu(function_idle_0, 0) != ESP_OK)
        {
            ESP_LOGE(__func__, "Failed to hook");
            return;
        }
        if(esp_register_freertos_tick_hook_for_cpu(function_tick_0, 0) != ESP_OK)
        {
            ESP_LOGE(__func__, "Failed to hook");
            return;
        }
        if(esp_register_freertos_idle_hook_for_cpu(function_idle_1, 1) != ESP_OK)
        {
            ESP_LOGE(__func__, "Failed to hook");
            return;
        }
        if(esp_register_freertos_tick_hook_for_cpu(function_tick_1, 1) != ESP_OK)
        {
            ESP_LOGE(__func__, "Failed to hook");
            return;
        }
    }
    _initialized = true;
}


bool IRAM_ATTR CpuUsage::function_idle_0(){ ++_idle_cnt_0; return true; }
bool IRAM_ATTR CpuUsage::function_idle_1(){ ++_idle_cnt_1; return true; }
void IRAM_ATTR CpuUsage::function_tick_0(){ ++_tick_cnt_0; }
void IRAM_ATTR CpuUsage::function_tick_1(){ ++_tick_cnt_1; }

//
}}
#endif
