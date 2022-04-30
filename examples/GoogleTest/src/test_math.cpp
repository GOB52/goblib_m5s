/*
  Unittest for gob_math
*/
#include <gob_math.hpp>
#include "helper_constants.hpp"
using namespace test::constants;

#include "gtest/gtest.h"
#include <cmath>

constexpr float rad_deg_1 = 57.295776f;
constexpr float deg_rad_1 = 0.017453f;
//constexpr float tolerance = 0.00002f;
constexpr float tolerance = 0.001f;


TEST(Math, Constexpr)
{
    constexpr float f_pi = goblib::math::constants::pi_f;
    EXPECT_FLOAT_EQ(3.141592653589793f, f_pi);
}


TEST(Math, floor)
{
    //    EXPECT_FLOAT_EQ(f_nan, goblib::math::floor(f_nan));

    EXPECT_FLOAT_EQ(goblib::math::floor(0.0f), std::floor(0.0f));
    EXPECT_FLOAT_EQ(goblib::math::floor(0.5f), std::floor(0.5f));
    EXPECT_FLOAT_EQ(goblib::math::floor(0.999999f), std::floor(0.999999f));
    EXPECT_FLOAT_EQ(goblib::math::floor(1.0f), std::floor(1.0f));
    EXPECT_FLOAT_EQ(goblib::math::floor(123.0f), std::floor(123.0f));
    EXPECT_FLOAT_EQ(goblib::math::floor(123.4f), std::floor(123.4f));

    EXPECT_FLOAT_EQ(goblib::math::floor(-0.5f), std::floor(-0.5f));
    EXPECT_FLOAT_EQ(goblib::math::floor(-0.999999f), std::floor(-0.999999f));
    EXPECT_FLOAT_EQ(goblib::math::floor(-1.0f), std::floor(-1.0f));
    EXPECT_FLOAT_EQ(goblib::math::floor(-123.0f), std::floor(-123.0f));
    EXPECT_FLOAT_EQ(goblib::math::floor(-123.4f), std::floor(-123.4f));
}

TEST(Math, ceil)
{
    EXPECT_FLOAT_EQ(goblib::math::ceil(0.0f), std::ceil(0.0f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(0.5f), std::ceil(0.5f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(0.999999f), std::ceil(0.999999f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(1.0f), std::ceil(1.0f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(123.0f), std::ceil(123.0f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(123.4f), std::ceil(123.4f));

    EXPECT_FLOAT_EQ(goblib::math::ceil(-0.5f), std::ceil(-0.5f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(-0.999999f), std::ceil(-0.999999f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(-1.0f), std::ceil(-1.0f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(-123.0f), std::ceil(-123.0f));
    EXPECT_FLOAT_EQ(goblib::math::ceil(-123.4f), std::ceil(-123.4f));
}


TEST(Math, round)
{
#if defined(__GNUG__) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    EXPECT_FLOAT_EQ(goblib::math::round(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(0.5f), 1.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(0.999999f), 1.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(123.0f), 123.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(123.4f), 123.0f);

    EXPECT_FLOAT_EQ(goblib::math::round(-0.5f), -1.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(-0.999999f), -1.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(-1.0f), -1.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(-123.0f), -123.0f);
    EXPECT_FLOAT_EQ(goblib::math::round(-123.4f), -123.0f);
#else
    EXPECT_FLOAT_EQ(goblib::math::round(0.0f), std::round(0.0f));
    EXPECT_FLOAT_EQ(goblib::math::round(0.5f), std::round(0.5f));
    EXPECT_FLOAT_EQ(goblib::math::round(0.999999f), std::round(0.999999f));
    EXPECT_FLOAT_EQ(goblib::math::round(1.0f), std::round(1.0f));
    EXPECT_FLOAT_EQ(goblib::math::round(123.0f), std::round(123.0f));
    EXPECT_FLOAT_EQ(goblib::math::round(123.4f), std::round(123.4f));

    EXPECT_FLOAT_EQ(goblib::math::round(-0.5f), std::round(-0.5f));
    EXPECT_FLOAT_EQ(goblib::math::round(-0.999999f), std::round(-0.999999f));
    EXPECT_FLOAT_EQ(goblib::math::round(-1.0f), std::round(-1.0f));
    EXPECT_FLOAT_EQ(goblib::math::round(-123.0f), std::round(-123.0f));
    EXPECT_FLOAT_EQ(goblib::math::round(-123.4f), std::round(-123.4f));
#endif
}

TEST(Math, radian)
{
    // Wrap 360 degree.
    EXPECT_NEAR(  0.0f,  goblib::math::wrapDeg360(   0.0f), tolerance);
    EXPECT_NEAR(  0.0f,  goblib::math::wrapDeg360( 360.0f), tolerance);

    EXPECT_NEAR( 90.1f,  goblib::math::wrapDeg360(  90.1f), tolerance);
    EXPECT_NEAR(180.2f,  goblib::math::wrapDeg360( 180.2f), tolerance);
    EXPECT_NEAR(270.3f,  goblib::math::wrapDeg360( 270.3f), tolerance);
    EXPECT_NEAR(  0.4f,  goblib::math::wrapDeg360( 360.4f), tolerance);
    EXPECT_NEAR( 90.5f,  goblib::math::wrapDeg360( 450.5f), tolerance);

    EXPECT_NEAR(269.4f,  goblib::math::wrapDeg360( -90.6f), tolerance);
    EXPECT_NEAR(179.3f,  goblib::math::wrapDeg360(-180.7f), tolerance);
    EXPECT_NEAR( 89.2f,  goblib::math::wrapDeg360(-270.8f), tolerance);
    EXPECT_NEAR(359.1f,  goblib::math::wrapDeg360(-360.9f), tolerance);
    EXPECT_NEAR(269.99f, goblib::math::wrapDeg360(-450.01f), tolerance);


    // Wrap 180 degree..
    EXPECT_NEAR(   0.0f,  goblib::math::wrapDeg180(   0.0f), tolerance);
    EXPECT_NEAR(-180.0f,  goblib::math::wrapDeg180(-180.0f), tolerance);
    EXPECT_NEAR(-180.0f,  goblib::math::wrapDeg180( 180.0f), tolerance);
    EXPECT_NEAR(   0.0f,  goblib::math::wrapDeg180( 360.0f), tolerance);

    EXPECT_NEAR(  90.1f,  goblib::math::wrapDeg180(  90.1f), tolerance);
    EXPECT_NEAR(-179.8f,  goblib::math::wrapDeg180( 180.2f), tolerance);
    EXPECT_NEAR( -89.7f,  goblib::math::wrapDeg180( 270.3f), tolerance);
    EXPECT_NEAR(   0.4f,  goblib::math::wrapDeg180( 360.4f), tolerance);
    EXPECT_NEAR(  90.5f,  goblib::math::wrapDeg180( 450.5f), tolerance);

    EXPECT_NEAR( -90.6f,  goblib::math::wrapDeg180( -90.6f), tolerance);
    EXPECT_NEAR( 179.3f,  goblib::math::wrapDeg180(-180.7f), tolerance);
    EXPECT_NEAR(  89.2f,  goblib::math::wrapDeg180(-270.8f), tolerance);
    EXPECT_NEAR(  -0.9f,  goblib::math::wrapDeg180(-360.9f), tolerance);
    EXPECT_NEAR( -90.01f, goblib::math::wrapDeg180(-450.01f), tolerance);

    // Radian to degree.
    constexpr float d_zero = goblib::math::rad2deg(0.0f);
    constexpr float d_one = goblib::math::rad2deg(1.0f);

    EXPECT_FLOAT_EQ(     0.0f, d_zero);    
    EXPECT_FLOAT_EQ(rad_deg_1, d_one);

    EXPECT_FLOAT_EQ(180.0f,  goblib::math::rad2deg(goblib::math::constants::pi_f));
    EXPECT_FLOAT_EQ(360.0f,  goblib::math::rad2deg(2.0f * goblib::math::constants::pi_f));
    EXPECT_FLOAT_EQ(-180.0f, goblib::math::rad2deg(-goblib::math::constants::pi_f));
    EXPECT_FLOAT_EQ(-360.0f, goblib::math::rad2deg(-2.0f * goblib::math::constants::pi_f));

    EXPECT_NEAR( 45.0f, goblib::math::rad2deg(deg_rad_1 *  45.0f), tolerance);
    //    printf("%f,%f,%f\n",   90.0f,goblib::math::rad2deg(deg_rad_1 *  90.0f), std::abs(90.0f - goblib::math::rad2deg(deg_rad_1 *  90.0f)));
    
    EXPECT_FLOAT_EQ(  90.0f, round(goblib::math::rad2deg(deg_rad_1 *  90.0f)));
    EXPECT_FLOAT_EQ( 180.0f, round(goblib::math::rad2deg(deg_rad_1 * 180.0f)));
    EXPECT_FLOAT_EQ( 270.0f, round(goblib::math::rad2deg(deg_rad_1 * 270.0f)));
    EXPECT_FLOAT_EQ( 360.0f, round(goblib::math::rad2deg(deg_rad_1 * 360.0f)));

    EXPECT_FLOAT_EQ( -45.0f,round(goblib::math::rad2deg(-deg_rad_1 *  45.0f)));
    EXPECT_FLOAT_EQ( -90.0f,round(goblib::math::rad2deg(-deg_rad_1 *  90.0f)));
    EXPECT_FLOAT_EQ(-180.0f,round(goblib::math::rad2deg(-deg_rad_1 * 180.0f)));
    EXPECT_FLOAT_EQ(-270.0f,round(goblib::math::rad2deg(-deg_rad_1 * 270.0f)));
    EXPECT_FLOAT_EQ(-360.0f,round(goblib::math::rad2deg(-deg_rad_1 * 360.0f)));

    // Degree to radian.
    constexpr float r_zero = goblib::math::deg2rad(0.0f);
    constexpr float r_one = goblib::math::deg2rad(rad_deg_1);

    EXPECT_FLOAT_EQ(0.0f, r_zero);
    EXPECT_FLOAT_EQ(1.0f, r_one);

    EXPECT_FLOAT_EQ(        goblib::math::constants::pi_f, goblib::math::deg2rad( 180.0f));
    EXPECT_FLOAT_EQ( 2.0f * goblib::math::constants::pi_f, goblib::math::deg2rad( 360.0f));
    EXPECT_FLOAT_EQ(       -goblib::math::constants::pi_f, goblib::math::deg2rad(-180.0f));
    EXPECT_FLOAT_EQ(-2.0f * goblib::math::constants::pi_f, goblib::math::deg2rad(-360.0f));
    
    EXPECT_NEAR( 45.0f * deg_rad_1, goblib::math::deg2rad( 45.0f), tolerance);
    EXPECT_NEAR( 90.0f * deg_rad_1, goblib::math::deg2rad( 90.0f), tolerance);
    EXPECT_NEAR(180.0f * deg_rad_1, goblib::math::deg2rad(180.0f), tolerance);
    EXPECT_NEAR(270.0f * deg_rad_1, goblib::math::deg2rad(270.0f), tolerance);
    EXPECT_NEAR(360.0f * deg_rad_1, goblib::math::deg2rad(360.0f), tolerance);

    EXPECT_NEAR( -45.0f * deg_rad_1, goblib::math::deg2rad( -45.0f), tolerance);
    EXPECT_NEAR( -90.0f * deg_rad_1, goblib::math::deg2rad( -90.0f), tolerance);
    EXPECT_NEAR(-180.0f * deg_rad_1, goblib::math::deg2rad(-180.0f), tolerance);
    EXPECT_NEAR(-270.0f * deg_rad_1, goblib::math::deg2rad(-270.0f), tolerance);
    EXPECT_NEAR(-360.0f * deg_rad_1, goblib::math::deg2rad(-360.0f), tolerance);

    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi2_f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi2_f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi_f),  -goblib::math::constants::pi_f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi_f),  -goblib::math::constants::pi_f);

    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi2_f * 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi2_f * 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi_f * 3.0f),  -goblib::math::constants::pi_f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi_f * 3.0f),  -goblib::math::constants::pi_f);

    

}

TEST(Math, isPowerOf2)
{
    EXPECT_FALSE(goblib::math::is_powerof2(0));
    EXPECT_TRUE(goblib::math::is_powerof2(1));
    EXPECT_TRUE(goblib::math::is_powerof2(2));
    EXPECT_FALSE(goblib::math::is_powerof2(3));

    constexpr auto b = goblib::math::is_powerof2(-1);
    EXPECT_FALSE(b);

    EXPECT_TRUE(goblib::math::is_powerof2(2147483648));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648U));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648UL));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648LL));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648ULL));

    EXPECT_TRUE(goblib::math::is_powerof2(1099511627776LL));
}
