
#include <cstdio>
#include <gob_fixed_point_number.hpp>
#include <gob_shape2d.hpp>
#include <gob_line2d.hpp>
#include <gob_rect2d.hpp>
#include <gob_ellipse2d.hpp>
#include <gob_math.hpp>
#include <gob_utility.hpp>
#include "gtest/gtest.h"

using fx16 = goblib::FixedPointNumber<std::int16_t, 6>; // Q6
using fx32 = goblib::FixedPointNumber<std::int32_t, 8>; // Q8

TEST(FixedPointNumber, Basic)
{
// Compile error
#if 0
    goblib::FixedPointNumber<float, 8> err0;
    goblib::FixedPointNumber<int, 0> err1;
    goblib::FixedPointNumber<std::uint8_t, 9> err2;
    goblib::FixedPointNumber<std::uint64_t, 20> err3;
#endif    

// Runtime assert(C++17 or later)
#if 0
    // <int16_t,6> Q6 [-512 - +511]
    fx16 over(512);
    fx16 over(-513);
#endif
    
    bool b16 = goblib::is_fixed_point_number<fx16>::value;
    bool b32 = goblib::is_fixed_point_number<fx32>::value;
    bool bint = goblib::is_fixed_point_number<int>::value;
    bool bfloat = goblib::is_fixed_point_number<float>::value;

    EXPECT_TRUE(b16);
    EXPECT_TRUE(b32);
    EXPECT_FALSE(bint);
    EXPECT_FALSE(bfloat);
    
    constexpr fx32 tmp(-123.456);
    constexpr fx16 s1;
    constexpr fx16 s2(123);
    constexpr fx16 s3(-123.456f); // frac: 011101 => 0.453125
    constexpr fx16 s4(s2);
    constexpr fx16 s5(tmp);
    fx16 s6, s7;
    s6 = s2;
    s7 = tmp;
    fx32 tmp2 = s3;

#if 0
    printf("s1:%s\n", s1.str());
    printf("s2:%s\n", s2.str());
    printf("s3:%s\n", s3.str());
    printf("s4:%s\n", s4.str());
    printf("s5:%s\n", s5.str());
    printf("s6:%s\n", s6.str());
    printf("s7:%s\n", s7.str());
    printf("tmp:%s\n", tmp.str());
#endif

    EXPECT_EQ(s1.value<int16_t>(), 0);
    EXPECT_FLOAT_EQ(s1.value<float>(), 0.0f);
    EXPECT_EQ(s2.value<int16_t>(), 123);
    EXPECT_FLOAT_EQ(s2.value<float>(), 123.0f);
    EXPECT_EQ(s3.value<int16_t>(), -123);
    EXPECT_FLOAT_EQ(s3.value<float>(), -123.453125f);

    EXPECT_TRUE(s4==s2);
    EXPECT_TRUE(s5==s3);
    EXPECT_TRUE(s6==s2);
    EXPECT_TRUE(s7==s3);
    EXPECT_TRUE(s1!=s2);
    EXPECT_TRUE(tmp == tmp2);

    EXPECT_FALSE(static_cast<bool>(s1));
    EXPECT_TRUE(static_cast<bool>(s2));
    EXPECT_TRUE(static_cast<bool>(s3));
    EXPECT_TRUE(static_cast<bool>(tmp));
    
    EXPECT_TRUE(s3 < s1);
    EXPECT_TRUE(s2 > s1);
    EXPECT_TRUE(s3 <= s3 && s3 <= s1);
    EXPECT_TRUE(s2 >= s2 && s2 >= s1);
    
    EXPECT_EQ(s1.raw(), 0);
    EXPECT_EQ(s2.raw(), 7872);  
    EXPECT_EQ(s3.raw(), -7901); 
    EXPECT_EQ(tmp.raw(), -31604); 

    auto ua0 = -s3;
    EXPECT_EQ(ua0.raw(), 7901);
    auto ua1 = +s3;
    EXPECT_EQ(ua1.raw(), -7901);
}

TEST(FixedPointNumber, Arithmetic)
{
    constexpr fx16 s1(123.456f); // 0001111011.011101 => 123.4315
    constexpr fx16 s2(-123.456f);
    constexpr fx16 s3(1.414f);  // 1.011010 => 1.40625
    constexpr fx16 s4(-0.987f); // 1111111111.000001 => -0.984375
    fx16 r,r2;
    
    // +,+=
    r = s1 + s2;  // 0
    r2 = s1;
    r2 += s2;
    EXPECT_EQ(r.value<int16_t>(), 0);
    EXPECT_FLOAT_EQ(r.value<float>(), 0.0f);
    EXPECT_EQ(r.raw(), 0);
    EXPECT_EQ(r, r2);

    r = s1 + 200; // 0101000011.011101
    r2 = s1;
    r2 += 200;
    EXPECT_EQ(r.value<int16_t>(), 323);
    EXPECT_FLOAT_EQ(r.value<float>(), 323.453125f);
    EXPECT_EQ(r.raw(), 20701);
    EXPECT_EQ(r, r2);

    r = 200 + s1; // 0101000011.011101
    EXPECT_EQ(r.value<int16_t>(), 323);
    EXPECT_FLOAT_EQ(r.value<float>(), 323.453125f);
    EXPECT_EQ(r.raw(), 20701);

    r = s1 + 1.5f;
    r2 = s1;
    r2 += 1.5f;
    EXPECT_EQ(r.value<int16_t>(), 124);
    EXPECT_FLOAT_EQ(r.value<float>(), 124.953125f);
    EXPECT_EQ(r, r2);

    r = 1.5f + s1;
    EXPECT_EQ(r.value<int16_t>(), 124);
    EXPECT_FLOAT_EQ(r.value<float>(), 124.953125f);
    
    // -,-=
    r = s1 - s2;  // 0011110110.111010
    r2 = s1;
    r2 -= s2;
    EXPECT_EQ(r.value<int16_t>(), 246);
    EXPECT_EQ(r.value<float>(), 246.90625f);
    EXPECT_EQ(r.raw(), 15802);
    EXPECT_EQ(r, r2);

    r = s1 - 200; // 1110110011.011101
    r2 = s1;
    r2 -= 200;
    EXPECT_EQ(r.value<int16_t>(), -76);
    EXPECT_FLOAT_EQ(r.value<float>(), -76.546875f);
    EXPECT_EQ(r.raw(), -4899);
    EXPECT_EQ(r, r2);

    r = 200 - s1; // 0001001100.100011
    EXPECT_EQ(r.value<int16_t>(), 76);
    EXPECT_FLOAT_EQ(r.value<float>(), 76.546875f);
    EXPECT_EQ(r.raw(), 4899);

    r = s1 - 0.5f;
    r2 = s1;
    r2 -= 0.5f;
    EXPECT_EQ(r.value<int16_t>(), 122);
    EXPECT_FLOAT_EQ(r.value<float>(), 122.953125f);
    EXPECT_EQ(r, r2);

    r = 0.5f - s1;
    EXPECT_EQ(r.value<int16_t>(), -122);
    EXPECT_FLOAT_EQ(r.value<float>(), -122.953125f);
    
    // *,*=
    r = s1 * s3; // 0010101101.100111
    r2 = s1;
    r2 *= s3;
    EXPECT_EQ(r.value<int16_t>(), 173);
    EXPECT_EQ(r.value<float>(), 173.609375f);
    EXPECT_EQ(r.raw(), 11111);
    EXPECT_EQ(r, r2);

    r = s1 * 2;  // 0011110110.111010
    r2 = s1;
    r2 *= 2;
    EXPECT_EQ(r.value<int16_t>(), 246);
    EXPECT_FLOAT_EQ(r.value<float>(), 246.90625f);
    EXPECT_EQ(r.raw(), 15802);
    EXPECT_EQ(r, r2);

    r = -2 * s1; // 1100001001.000110
    EXPECT_EQ(r.value<int16_t>(), -246);
    EXPECT_FLOAT_EQ(r.value<float>(), -246.90625f);
    EXPECT_EQ(r.raw(), -15802);

    r = s1 * 0.5f;
    r2 = s1;
    r2 *= 0.5f;
    EXPECT_EQ(r.value<int16_t>(), 61);
    //EXPECT_FLOAT_EQ(r.value<float>(), 61.734375f);
    EXPECT_FLOAT_EQ(r.value<float>(), 61.71875f);
    EXPECT_EQ(r, r2);

    r = 0.5f * s1;
    EXPECT_EQ(r.value<int16_t>(), 61);
    //EXPECT_FLOAT_EQ(r.value<float>(), 61.734375f);
    EXPECT_FLOAT_EQ(r.value<float>(), 61.71875f);
    
    // /,/=
    r = s1 / s4;  // 1111111101.011010
    r2 = s1;
    r2 /= s4;
    EXPECT_EQ(r.value<int16_t>(), -125);
    EXPECT_EQ(r.value<float>(), -125.40625f);
    EXPECT_EQ(r.raw(), -8026);
    EXPECT_EQ(r, r2);

    r = s1 / 4;   // 0000011110.110111
    r2 = s1;
    r2 /= 4;
    EXPECT_EQ(r.value<int16_t>(), 30);
    EXPECT_EQ(r.value<float>(), 30.859375f);
    EXPECT_EQ(r.raw(), 1975);
    EXPECT_EQ(r, r2);

    r = 128 / s1; // 0000000001.000010
    EXPECT_EQ(r.value<int16_t>(), 1);
    EXPECT_EQ(r.value<float>(), 1.03125f);
    EXPECT_EQ(r.raw(), 66);

    r = s1 / 0.5f;
    r2 = s1;
    r2 /= 0.5f;
    EXPECT_EQ(r.value<int16_t>(), 246);
    EXPECT_FLOAT_EQ(r.value<float>(), 246.90625f);
    EXPECT_EQ(r, r2);

    r = 123.453125f / s1;
    EXPECT_EQ(r.value<int16_t>(), 1);
    EXPECT_FLOAT_EQ(r.value<float>(), 1.0f);
}

//
TEST(FixedPointNumber, math)
{
    fx16 a(100.123f), b(-100.123f);
    fx16 c = a;
    fx16 d = b;

    EXPECT_TRUE(goblib::math::equal(a, c));
    EXPECT_TRUE(goblib::math::equal(b, d));
    EXPECT_FALSE(goblib::math::equal(a, b));

    EXPECT_FALSE(goblib::math::not_equal(a, c));
    EXPECT_FALSE(goblib::math::not_equal(b, d));
    EXPECT_TRUE(goblib::math::not_equal(a, b));
    
    EXPECT_EQ(goblib::math::fmin(a,b), b);
    EXPECT_EQ(goblib::math::fmax(a,b), a);

    EXPECT_EQ(goblib::clamp(fx16(100.123f), b, a).raw(), a.raw());
    EXPECT_EQ(goblib::clamp(fx16(-100.123f), b, a).raw(), b.raw());
    EXPECT_EQ(goblib::clamp(fx16(300.123f), b, a).raw(), a.raw());
    EXPECT_EQ(goblib::clamp(fx16(-300.123f), b, a).raw(), b.raw());

    EXPECT_EQ(goblib::math::fabs(a), a);
    EXPECT_EQ(goblib::math::fabs(b), a);
    EXPECT_EQ(goblib::math::fabs(fx16(0)), fx16(0));
    
    constexpr fx16 array[] =
    {
        fx16(2.0f), fx16(2.1f), fx16(2.5f), fx16(2.9f),
        fx16(-2.0f), fx16(-2.1f), fx16(-2.5f), fx16(-2.9f),
    };
    constexpr fx16 round_a[] =
    {
        fx16(2), fx16(2), fx16(3), fx16(3),
        fx16(-2), fx16(-2), fx16(-3), fx16(-3), 
    };
    constexpr fx16 floor_a[] =
    {
        fx16(2), fx16(2), fx16(2), fx16(2),
        fx16(-2), fx16(-3), fx16(-3), fx16(-3), 
    };        
    constexpr fx16 ceil_a[] =
    {
        fx16(2), fx16(3), fx16(3), fx16(3),
        fx16(-2), fx16(-2), fx16(-2), fx16(-2), 
    };        

    for(int i = 0; i < goblib::size(array); ++i)
    {
        //        printf(">>[%d] %s:%f => %f\n",i, array[i].str(), array[i].value<float>(), goblib::math::round(array[i]).value<float>());
        EXPECT_EQ(goblib::math::round(array[i]).raw(), round_a[i].raw());
    }

    for(int i = 0; i < goblib::size(array); ++i)
    {
        //        printf(">>[%d] %s:%f => %f\n",i, array[i].str(), array[i].value<float>(), goblib::math::floor(array[i]).value<float>());
        EXPECT_EQ(goblib::math::floor(array[i]).raw(), floor_a[i].raw());
    }

    for(int i = 0; i < goblib::size(array); ++i)
    {
        //        printf(">>[%d] %s:%f => %f\n",i, array[i].str(), array[i].value<float>(), goblib::math::ceil(array[i]).value<float>());
        EXPECT_EQ(goblib::math::ceil(array[i]).raw(), ceil_a[i].raw());
    }

}

//
using PosFx = goblib::shape2d::Point<fx16>;
using LineFx= goblib::shape2d::LineSegment<fx16>;
using RectFx = goblib::shape2d::Rectangle<fx16>;
using CircleFx = goblib::shape2d::Circle<fx16>;
using EllipseFx = goblib::shape2d::Ellipse<fx16>;

TEST(FixedPointNumber, Shape2d)
{
    /* Check default constructor using fx */
    {
        PosFx pz;
        LineFx lz;
        RectFx rz;
    }
    
    PosFx pf1(fx16(1.41421f), fx16(-1.73205f));
    EXPECT_FLOAT_EQ(pf1.angle(), std::atan2(-1.71875f, 1.40625f));

    LineFx lf1 = LineFx(PosFx(), pf1);
    EXPECT_FLOAT_EQ(lf1.angle(), std::atan2(-1.71875f, 1.40625f));

    RectFx rf1;
    EXPECT_FALSE(rf1.valid());
    EXPECT_TRUE(rf1.empty());
    RectFx rf2(fx16(1), fx16(2), fx16(100), fx16(200));
    EXPECT_TRUE(rf2.valid());
    EXPECT_TRUE(rf2.right() == fx16(100));
    EXPECT_TRUE(rf2.bottom() == fx16(201));

    PosFx lt(fx16(10), fx16(20));
    PosFx rb(fx16(29), fx16(39));
    RectFx rf3(lt, rb);
    EXPECT_TRUE(rf3.width() == fx16(20));
    EXPECT_TRUE(rf3.height() == fx16(20));

    PosFx cp(fx16(100), fx16(100));
    CircleFx cfx(cp, fx16(50));
    goblib::shape2d::Circle<float> cf(100,100,50);

    for(int i=0;i<36;++i)
    {
        float rad = goblib::math::deg2rad(i * 10.0f);
        auto posf = cf.pos(rad);
        auto pos16 = cfx.pos(rad);

        //        printf("[%d] (%e,%e) (%d,%d) (%e,%e)\n", i*10, posf.x(), posf.y(), (int)pos16.x(), (int)pos16.y(), (float)pos16.x(), (float)pos16.y());

        EXPECT_NEAR(posf.x(), pos16.x().value<float>(), 0.015625f);
        EXPECT_NEAR(posf.y(), pos16.y().value<float>(), 0.015625f);
    }

    EllipseFx efx(cp, fx16(10), fx16(20));
    goblib::shape2d::Ellipse<float> ef(100,100, 10, 20);

    for(int i=0;i<36;++i)
    {
        float rot = goblib::math::deg2rad(i * 10.f);
        ef.setRotate(rot);
        efx.setRotate(rot);

        for(int j=0;j<36;j++)
        {
            float rad = goblib::math::deg2rad(j * 10.f);
            auto posf = ef.pos(rad);
            auto pos16 = efx.pos(rad);

            //            printf("[%d][%d] (%e,%e) (%d,%d) (%e,%e)\n", i*10, j*10,
            //                   posf.x(), posf.y(), (int)pos16.x(), (int)pos16.y(), (float)pos16.x(), (float)pos16.y());

            EXPECT_NEAR(posf.x(), pos16.x().value<float>(), 0.027f);
            EXPECT_NEAR(posf.y(), pos16.y().value<float>(), 0.027f);
        }
    }
}

#if 0
// print numeric_limits
template <typename LIMITS> struct LimitsPrint
{
    static const char* denorm_str(std::float_denorm_style s)
    {
        switch(s)
        {
        case std::denorm_indeterminate: return "denorm_indeterminate";
        case std::denorm_absent: return "denorm_absent";
        case std::denorm_present: return "denorm_present";
        }
        return "unknown";
    }

    static const char* round_style_str(std::float_round_style s)
    {
        switch(s)
        {
        case std::round_indeterminate: return "round_indeterminate";
        case std::round_toward_zero: return "round_indeterminate";
        case std::round_to_nearest: return "round_to_nearest";
        case std::round_toward_infinity: return "round_toward_infinity";
        case std::round_toward_neg_infinity: return "round_toward_neg_infinity";
        }
        return "unknown";
    }

    static void print()
    {
        auto vmin = LIMITS::min();
        auto vlow = LIMITS::lowest();
        auto vmax = LIMITS::max();
        auto vep = LIMITS::epsilon();
        auto vre= LIMITS::round_error();
        auto vdmin = LIMITS::denorm_min();
        
        printf("min:%d:%d:%e\n", vmin.raw(), vmin.template value<int>(), vmin.template value<float>());
        printf("max:%d:%d:%e\n", vmax.raw(), vmax.template value<int>(), vmax.template value<float>());
        printf("lowest:%d:%d:%e\n", vlow.raw(), vlow.template value<int>(), vlow.template value<float>());

        printf("digits:%d digits10:%d max_digits10:%d\n",
               LIMITS::digits, LIMITS::digits10, LIMITS::max_digits10);
        printf("min_exponent:%d min_exponent10:%d max_exponent:%d max_exponent10:%d\n",
               LIMITS::min_exponent, LIMITS::min_exponent10, LIMITS::max_exponent, LIMITS::max_exponent10);

        printf("is_signed:%d is_integer:%d is_exact:%d radix:%d\n",
               LIMITS::is_signed, LIMITS::is_integer, LIMITS::is_exact, LIMITS::radix);

        printf("epsilon:%d:%e\n", vep.raw(), vep.template value<float>());
        printf("round_error:%d:%e\n", vre.raw(), vre.template value<float>());

        printf("has_infinity:%d has_quiet_NaN:%d has_signaling_NaN:%d has_denorm:%s has_denorm_loss:%d\n",
               LIMITS::has_infinity, LIMITS::has_quiet_NaN, LIMITS::has_signaling_NaN, denorm_str(LIMITS::has_denorm), LIMITS::has_denorm_loss);

        printf("denorm_min:%d:%e\n", vdmin.raw(), vdmin.template value<float>());

        printf("is_iec559:%d is_bounded:%d is_modulo:%d traps:%d tinyness_before:%d round_style:%s\n",
               LIMITS::is_iec559, LIMITS::is_bounded, LIMITS::is_modulo, LIMITS::traps, LIMITS::tinyness_before,
               round_style_str(LIMITS::round_style));

    }
};

TEST(FixedPointNumber, Limits)
{
    using limits_fx16 = std::numeric_limits<fx16>;
    using limits_fx32 = std::numeric_limits<fx32>;
    using fx32_24 = goblib::FixedPointNumber<std::int32_t, 24>; // Q24
    using limits_fx32_24 = std::numeric_limits<fx32_24>;

    printf("fx16----\n");
    LimitsPrint<limits_fx16>::print();
    printf("fx32----\n");
    LimitsPrint<limits_fx32>::print();
    printf("fx32_24----\n");
    LimitsPrint<limits_fx32_24>::print();
}

#endif
