/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_cpu_usage.hpp
  @brief  Calculate CPU usage using ESP32 functions.
*/
#ifndef GOBLIB_M5S_CPUUSAGE_HPP
#define GOBLIB_M5S_CPUUSAGE_HPP

#include <esp_attr.h> // IRAM_ATTR
#include "gob_utility.hpp"
#include "gob_macro.hpp"

namespace goblib { namespace m5s {

/*!
  CpuUsage
  @brief Measurement task tick and idle tick by esp_freertos_hook.
*/
class CpuUsage
{
  public:

    /*! Setup */
    static void setup();

    /// @name Get cpu usage
    /// @{
    /*! @brief get cpu0 usage */
    static GOBLIB_INLINE float cpu0()
    {
        float rate = goblib::clamp(100.0f - (_tick_cnt_0 ? (_idle_cnt_0 * 100.0f / _tick_cnt_0) : 0.0f), 0.0f, 100.0f);
        reset0();
        return rate;
    }
    /*! @brief get cpu1 usage */
    static GOBLIB_INLINE float cpu1()
    {
        float rate = goblib::clamp(100.0f - (_tick_cnt_1 ? (_idle_cnt_1 * 100.0f / _tick_cnt_1) : 0.0f), 0.0f, 100.0f);
        reset1();
        return rate;;
    }
    /// @}

    /// @name Property
    /// @{
    static GOBLIB_INLINE long get_idle_count_0(){ return _idle_cnt_0; }
    static GOBLIB_INLINE long get_tick_count_0(){ return _tick_cnt_0; }
    static GOBLIB_INLINE long get_idle_count_1(){ return _idle_cnt_1; }
    static GOBLIB_INLINE long get_tick_count_1(){ return _tick_cnt_1; }
    /// @}

    /// @name Operation
    /// @{
    static GOBLIB_INLINE void reset0(){ _idle_cnt_0 = _tick_cnt_0 = 0; }
    static GOBLIB_INLINE void reset1(){ _idle_cnt_1 = _tick_cnt_1 = 0; }
    /// @}
    
  private:
    CpuUsage() = default;
    ~CpuUsage() = default;

    CpuUsage(CpuUsage&);
    CpuUsage(CpuUsage&&);
    CpuUsage& operator=(const CpuUsage&);
    CpuUsage& operator=(CpuUsage&&);

    static bool IRAM_ATTR function_idle_0();
    static bool IRAM_ATTR function_idle_1();
    static void IRAM_ATTR function_tick_0();
    static void IRAM_ATTR function_tick_1();

    static long _idle_cnt_0, _tick_cnt_0;
    static long _idle_cnt_1, _tick_cnt_1;
    static bool _initialized;

};

//
}}
#endif
