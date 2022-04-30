/*
  Unittest for gob_m5s_cpu_usage
*/
#if defined(ESP32)

#include <gob_m5s_cpu_usage.hpp>
using namespace goblib::m5s;

#include <thread>
#include <chrono>

#include "gtest/gtest.h"

TEST(CpuUsage, Basic)
{
    CpuUsage::setup();

    auto idle0 = CpuUsage::get_idle_count_0();
    auto tick0 = CpuUsage::get_tick_count_0();
    auto idle1 = CpuUsage::get_idle_count_1();
    auto tick1 = CpuUsage::get_tick_count_1();

    //printf("%d:%d / %d:%d\n", idle0, tick0, idle1, tick1);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    /*
      printf("%d:%d / %d:%d\n", 
      CpuUsage::get_idle_count_0(),
      CpuUsage::get_tick_count_0(),
      CpuUsage::get_idle_count_1(),
      CpuUsage::get_tick_count_1());
    */
       
    EXPECT_GT(CpuUsage::get_idle_count_0(), idle0);
    EXPECT_GT(CpuUsage::get_tick_count_0(), tick0);
    EXPECT_GT(CpuUsage::get_idle_count_1(), idle1);
    EXPECT_GT(CpuUsage::get_tick_count_1(), tick1);

    //printf("%f:%f\n", CpuUsage::cpu0(), CpuUsage::cpu1());

    (void)CpuUsage::cpu0();
    (void)CpuUsage::cpu1();
    EXPECT_EQ(CpuUsage::get_idle_count_0(), 0);
    EXPECT_EQ(CpuUsage::get_tick_count_0(), 0);
    EXPECT_EQ(CpuUsage::get_idle_count_1(), 0);
    EXPECT_EQ(CpuUsage::get_tick_count_1(), 0);
    
}

#endif
