/*
  Unittest for gob_random
*/
#include <gob_random.hpp>
using namespace goblib;

#ifdef ARDUINO
#include <gob_m5s_random.hpp>
#include <Arduino.h>
#endif

#include "gtest/gtest.h"

TEST(Random, Basic)
{
    constexpr std::uint32_t seed = 52;

    EXPECT_TRUE(goblib::template_helper::is_rng<std::mt19937>::value);
    EXPECT_FALSE(goblib::template_helper::is_rng<std::vector<int>>::value);
    EXPECT_TRUE(goblib::template_helper::is_rng<std::ranlux24>::value);
    
    std::ranlux24 lux24(seed);
    std::uniform_int_distribution<> d_int(-100, +100);
    std::uniform_real_distribution<> d_real(-123.f, +123.f);

    Rng<std::ranlux24> rng_lux24(seed);
    EXPECT_EQ(d_int(lux24), rng_lux24(-100,100));
    EXPECT_FLOAT_EQ(d_real(lux24), rng_lux24(-123.f, 123.f));
    EXPECT_EQ(d_int(lux24), rng_lux24(-100,100));
    EXPECT_EQ(d_int(lux24), rng_lux24(-100,100));
    EXPECT_FLOAT_EQ(d_real(lux24), rng_lux24(-123.f, 123.f));
    EXPECT_FLOAT_EQ(d_real(lux24), rng_lux24(-123.f, 123.f));
    EXPECT_NE(d_int(lux24), rng_lux24(1000,10000));
    EXPECT_EQ(d_int(lux24), rng_lux24(-100,100));

    lux24.discard(100);
    rng_lux24.discard(99);
    EXPECT_NE(lux24(), rng_lux24());
    rng_lux24.discard(1);
    EXPECT_EQ(d_int(lux24), rng_lux24(-100,100));
}

#ifdef ARDUINO 

TEST(Random, Arduino)
{
    EXPECT_TRUE(goblib::template_helper::is_rng<goblib::m5s::arduino_engine>::value);

    Rng<goblib::m5s::arduino_engine> rng_ae; // hardware RNG
    goblib::m5s::arduino_engine ae;
    std::uniform_int_distribution<> d_int(-2,2);
    std::uniform_real_distribution<> d_real(0.0f, 0.00001f);

    //    printf("d_int %ld\n", d_int(ae));
    //    printf("rng %ld\n", rng_ae(-2,2));


    for(int i=0;i<10000;++i)
    {
        auto di = d_int(ae);
        auto df = d_real(ae);
        auto ri = rng_ae(-2,2);
        auto rf = rng_ae(0.0f, 0.00001f);
        EXPECT_GE(di, -2);
        EXPECT_LE(di,  2);
        EXPECT_GE(df, 0.0f);
        EXPECT_LE(df, 0.00001f);

        EXPECT_GE(ri, -2);
        EXPECT_LE(ri,  2);
        EXPECT_GE(rf, 0.0f);
        EXPECT_LE(rf, 0.00001f);
    }
}

#endif
