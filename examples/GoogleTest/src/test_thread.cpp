/*
  Unittest for gob_thread
*/
#include <gob_m5s_thread.hpp>

#include "gtest/gtest.h"

TEST(Thread, sleep_for)
{
    using clock = std::chrono::steady_clock;

    auto wt = std::chrono::duration_cast<clock::duration>(std::chrono::milliseconds(10));    
    auto s = clock::now();

    goblib::m5s::this_thread::sleep_for(wt);

    EXPECT_GE((clock::now() - s).count(), wt.count());

    // std::this_thread::sleep_for in ESP-IDF does not meet specifications.
    // return before the sleep duration.
    // sa https://en.cppreference.com/w/cpp/thread/sleep_for
    //s = clock::now();
    // std::this_thread::sleep_for(wt);
}


