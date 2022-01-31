/*
  Unittest for gob_shape2d
*/

#include <gob_shape2d.hpp>
#include <gob_line2d.hpp>
#include <gob_rect2d.hpp>
#include <gob_ellipse2d.hpp>

using namespace goblib;

#include "gtest/gtest.h"
#ifdef ARDUINO_M5Stack_Core_ESP32
#include <M5Stack.h>
#endif

using Pos2i = shape2d::Point<std::int32_t>;
using Pos2f = shape2d::Point<float>;
using Line2f = shape2d::LineSegment<float>;

TEST(Shape2d, Point)
{
    Pos2i pi0;
    constexpr Pos2i pi1(123,-456);
    Pos2i pi2(-123, 456);
    constexpr Pos2i pi3(pi1); // pi3(123, -56)
    Pos2i pi4 = pi2; // pi4(-123,456)

    constexpr auto xi = pi1.x();
    constexpr auto yi = pi1.y();
    EXPECT_EQ(xi, 123);
    EXPECT_EQ(yi, -456);
    
    EXPECT_TRUE(pi0 != pi1);
    constexpr auto bi = pi1 == pi3;
    EXPECT_TRUE(bi);
    EXPECT_TRUE(pi2 == pi4);

    pi2.offset(123*2, -456*2); // pi2(123, -456)
    EXPECT_TRUE(pi1 == pi2);

    pi0.move(123, -456); // pi0(123,-456)
    EXPECT_TRUE(pi0 == pi2);

    Pos2i pi5 = pi0 + pi1;  // pi5(246, -912)
    EXPECT_EQ(pi5.x(), 246);
    EXPECT_EQ(pi5.y(), -912);
    Pos2i pi6 = pi0 - pi1; // pi6(0,0)
    EXPECT_EQ(pi6.x(), 0);
    EXPECT_EQ(pi6.y(), 0);

    pi6 = -pi2; // (-123, +456)
    EXPECT_EQ(pi6.x(), -123);
    EXPECT_EQ(pi6.y(), 456);

    pi2 = 10 * pi1;
    pi4 = pi1 * 10;
    EXPECT_EQ(pi2, pi4);
    EXPECT_EQ(pi2.x(), 1230);
    EXPECT_EQ(pi2.y(), -4560);

    EXPECT_TRUE(pi1 == pi1);
    EXPECT_TRUE(pi1 != pi2);
    EXPECT_TRUE(pi1 < pi2);
    EXPECT_TRUE(pi1 <= pi1);
    EXPECT_TRUE(pi1 <= pi2);
    EXPECT_TRUE(pi2 > pi1);
    EXPECT_TRUE(pi1 >= pi1);
    EXPECT_TRUE(pi2 >= pi1);

    EXPECT_EQ(123*123+(-456 * -456), pi1.lengthSq());
    EXPECT_EQ((int32_t)std::sqrt(123*123+(-456 * -456)), pi1.length());
    
    //
    Pos2f pf0;
    constexpr Pos2f pf1(123.f,-456.f);
    Pos2f pf2(-123.f, 456.f);
    constexpr Pos2f pf3(pf1); // pf3(123, -56)
    Pos2f pf4 = pf2; // pf4(-123,456)

    constexpr auto xf = pf1.x();
    constexpr auto yf = pf1.y();
    EXPECT_FLOAT_EQ(xf, 123.f);
    EXPECT_FLOAT_EQ(yf, -456.f);
    
    EXPECT_TRUE(pf0 != pf1);
    constexpr auto bf = pf1 == pf3;
    EXPECT_TRUE(bf);
    EXPECT_TRUE(pf2 == pf4);

    pf2.offset(123.f*2, -456.f*2); // pf2(123, -456)
    EXPECT_TRUE(pf1 == pf2);

    pf0.move(123.f, -456.f); // pf0(123,-456)
    EXPECT_TRUE(pf0 == pf2);

    Pos2f pf5 = pf0 + pf1;  // pf5(246, -912)
    EXPECT_FLOAT_EQ(pf5.x(), 246.f);
    EXPECT_FLOAT_EQ(pf5.y(), -912.f);
    Pos2f pf6 = pf0 - pf1; // pf6(0,0)
    EXPECT_FLOAT_EQ(pf6.x(), 0.f);
    EXPECT_FLOAT_EQ(pf6.y(), 0.f);

    pf6 = -pf2; // (-123, +456)
    EXPECT_FLOAT_EQ(pf6.x(), -123.f);
    EXPECT_FLOAT_EQ(pf6.y(), 456.f);

    pf2 = 100.f * pf1;
    pf4 = pf1 * 100.f;
    EXPECT_EQ(pf2, pf4);
    EXPECT_FLOAT_EQ(pf2.x(), 12300.f);
    EXPECT_FLOAT_EQ(pf2.y(), -45600.f);

    EXPECT_TRUE(pf1 == pf1);
    EXPECT_TRUE(pf1 != pf2);
    EXPECT_TRUE(pf1 < pf2);
    EXPECT_TRUE(pf1 <= pf1);
    EXPECT_TRUE(pf1 <= pf2);
    EXPECT_TRUE(pf2 > pf1);
    EXPECT_TRUE(pf1 >= pf1);
    EXPECT_TRUE(pf2 >= pf1);

    EXPECT_FLOAT_EQ(123.f*123.f+(-456.f * -456.f), pf1.lengthSq());
    EXPECT_FLOAT_EQ(std::sqrt(123.f*123.f+(-456.f * -456.f)), pf1.length());

    EXPECT_FLOAT_EQ(pf1.angle(), std::atan2(pf1.y(), pf1.x()));
}

TEST(Shape2d, Line)
{
    constexpr float x = -123.456f;
    constexpr float y = 987.654f;
    constexpr Pos2f pos1(x,y);
    constexpr Pos2f pos2;
    
    Line2f line1;
    constexpr Line2f line2(x, y, 0.0f, 0.0f);
    Line2f line3(pos2, pos1);
    constexpr Line2f line3r(pos1, pos2);

    EXPECT_FALSE(line1);
    EXPECT_TRUE(line2);
    EXPECT_TRUE(line3);

    EXPECT_TRUE(line1 != line2);
    EXPECT_TRUE(line3r == line2);

    EXPECT_EQ(pos1, line2.start());
    EXPECT_EQ(pos2, line2.end());
    EXPECT_FLOAT_EQ(x, line3.ex());
    EXPECT_FLOAT_EQ(y, line3.ey());
    
    Line2f line4;
    line4 = line2;
    Line2f line5(line4);

    EXPECT_TRUE(line4 == line2);
    EXPECT_TRUE(line5 == line2);
    EXPECT_FLOAT_EQ( x*x + y*y, line2.lengthSq());
    EXPECT_FLOAT_EQ( std::sqrt(x*x + y*y), line2.length());
    EXPECT_FLOAT_EQ( pos1.length(), line2.length());
    EXPECT_FLOAT_EQ(std::atan2(-y,-x), line2.angle());

    Line2f line6(line3);
    line3.move(100.0f, -100.0f);
    EXPECT_FLOAT_EQ(100.0f, line3.sx());
    EXPECT_FLOAT_EQ(-100.0f, line3.sy());
    EXPECT_FLOAT_EQ(x + 100.0f, line3.ex());
    EXPECT_FLOAT_EQ(y + -100.0f, line3.ey());
    EXPECT_FLOAT_EQ(line3.angle(), line6.angle());

    line3.offset(-100.f, 100.f);
    EXPECT_EQ(line3, line6);

    line3.zero();
    EXPECT_FALSE(line3);
}

#ifdef ARDUINO_M5Stack_Core_ESP32

namespace
{
void draw(std::vector<Pos2i>& v, std::uint16_t clr = TFT_WHITE)
{
    if(v.empty()) { return; }

    auto end = v.cend() - 1;
    for (auto it = v.cbegin(); it != end; ++it)
    {
        auto s = *it;
        auto e = *(it+1);
        M5.Lcd.drawLine(s.x(), s.y(), e.x(), e.y(), clr);
    }
}
//
}

TEST(Shape2d, Circle)
{
    using Circle2i = shape2d::Circle<int16_t>;

    Pos2i cpos(320/2, 240/2);
    Circle2i c(cpos, 10);
    std::vector<Pos2i> v;
    
    for(int i=1; i <=10; ++i)
    {
        c.setRadius(i*10);
        v.clear();
        for(int j=0; j <= 36; ++j)
        {
            v.emplace_back(c.pos(goblib::math::deg2rad(j * 10.f)));
        }
        draw(v, TFT_GREEN);
    }
}


TEST(Shape2d, Ellipse)
{
    using Ellipse2i = shape2d::Ellipse<int16_t>;

    Pos2i cpos(320/2, 240/2);
    Ellipse2i e(cpos, 100, 25);
    std::vector<Pos2i> v;

    #if 0
    for(int i=1; i <=10; ++i)
    {
        c.setRadius(i*4, i*8);
        v.clear();
        for(int j=0; j <= 36; ++j)
        {
            v.emplace_back(c.pos(goblib::math::deg2rad(j * 10.f)));
        }
        draw(v, TFT_PINK);
    }
    #endif

    
    for(int i=0; i < 36; ++i)
    {
        auto rot = goblib::math::deg2rad(i * 10.0f);
        e.setRotate(rot);

        v.clear();
        for(int j=0; j<=36; ++j)
        {
            v.emplace_back(e.pos(goblib::math::deg2rad(j * 10.f)));
        }
        draw(v, TFT_PINK);
    }
}
#endif
