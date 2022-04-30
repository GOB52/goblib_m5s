/*
  Unittest for gob_easing
*/

#include <gob_easing.hpp>
#include "gtest/gtest.h"
#ifdef ARDUINO_M5Stack_Core_ESP32
#include <M5Stack.h>
#endif
#include "helper.hpp"

using namespace goblib::easing;

TEST(Easing, Basic)
{
    EaseLerp<std::int32_t> e;
    std::vector<std::int32_t> v;
    e.start(-10,10, 20);
    while(e.busy())
    {
        e.pump();
        v.push_back(e.value());
    }
    EXPECT_EQ(20, v.size());
    EXPECT_EQ(10, v.back());

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::linear(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::linear(1.0f));
    
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quadratic_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quadratic_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quadratic_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quadratic_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quadratic_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quadratic_inout(1.0f));
    
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::cubic_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::cubic_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::cubic_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::cubic_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::cubic_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::cubic_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quartic_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quartic_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quartic_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quartic_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quartic_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quartic_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quintic_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quintic_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quintic_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quintic_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::quintic_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::quintic_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::sinusoidal_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::sinusoidal_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::sinusoidal_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::sinusoidal_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::sinusoidal_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::sinusoidal_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::exponential_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::exponential_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::exponential_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::exponential_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::exponential_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::exponential_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::circular_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::circular_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::circular_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::circular_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::circular_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::circular_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::back_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::back_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::back_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::back_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::back_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::back_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::elastic_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::elastic_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::elastic_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::elastic_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::elastic_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::elastic_inout(1.0f));

    EXPECT_FLOAT_EQ(0.0f, goblib::easing::bounce_in(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::bounce_in(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::bounce_out(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::bounce_out(1.0f));
    EXPECT_FLOAT_EQ(0.0f, goblib::easing::bounce_inout(0.0f));
    EXPECT_FLOAT_EQ(1.0f, goblib::easing::bounce_inout(1.0f));


    //constexpr test
    GOBLIB_MAYBE_UNUSED constexpr auto t00 = goblib::easing::linear(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t01 = goblib::easing::quadratic_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t02 = goblib::easing::quadratic_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t03 = goblib::easing::quadratic_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t04 = goblib::easing::cubic_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t05 = goblib::easing::cubic_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t06 = goblib::easing::cubic_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t07 = goblib::easing::quartic_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t08 = goblib::easing::quartic_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t09 = goblib::easing::quartic_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t10 = goblib::easing::quintic_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t11 = goblib::easing::quintic_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t12 = goblib::easing::quintic_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t13 = goblib::easing::sinusoidal_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t14 = goblib::easing::sinusoidal_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t15 = goblib::easing::sinusoidal_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t16 = goblib::easing::exponential_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t17 = goblib::easing::exponential_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t18 = goblib::easing::exponential_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t19 = goblib::easing::circular_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t20 = goblib::easing::circular_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t21 = goblib::easing::circular_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t22 = goblib::easing::back_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t23 = goblib::easing::back_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t24 = goblib::easing::back_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t25 = goblib::easing::elastic_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t26 = goblib::easing::elastic_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t27 = goblib::easing::elastic_inout(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t28 = goblib::easing::bounce_in(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t29 = goblib::easing::bounce_out(0.5f);
    GOBLIB_MAYBE_UNUSED constexpr auto t30 = goblib::easing::bounce_inout(0.5f);
}




#ifdef ARDUINO_M5Stack_Core_ESP32

namespace
{
constexpr std::int16_t WIDTH = 256;
constexpr std::int16_t HEIGHT = 120;
constexpr std::int16_t dx = (320 - WIDTH) / 2;
constexpr std::int16_t dy = (240 - HEIGHT) / 2;

void drawGraph(std::vector<std::int16_t>& v, std::uint16_t clr = TFT_WHITE)
{
    if(v.empty()) { return; }

    std::int16_t i = 0;
    auto end = v.cend() - 1;
    for (auto it = v.cbegin(); it != end; ++it, ++i)
    {
        M5.Lcd.drawLine(dx + i, dy + HEIGHT - *it, dx + i + 1, dy + HEIGHT - *(it+1), clr);
    }
}
//
}

template<class E> void easing(std::uint16_t clr = TFT_WHITE)
{
    //    printf("%s:%s\n", __func__, helper::TypeName<E>());

    E e;
    std::vector<std::int16_t> v;
    e.start(0, HEIGHT, WIDTH);
    while(e.busy())
    {
        e.pump();
        v.push_back(e.value());
    }
    EXPECT_EQ(v.back(), HEIGHT);
    drawGraph(v, clr);
}


TEST(Easing, M5Stack)
{
    M5.Lcd.drawRect(dx, dy, WIDTH, HEIGHT, TFT_WHITE);

    easing< EaseLerp<std::int16_t> >();

    easing< EaseInQuad<std::int16_t> >(RED);
    easing< EaseOutQuad<std::int16_t> >(BLUE);
    easing< EaseInOutQuad<std::int16_t> >(YELLOW);

    easing< EaseInCubic<std::int16_t> >(RED);
    easing< EaseOutCubic<std::int16_t> >(BLUE);
    easing< EaseInOutCubic<std::int16_t> >(YELLOW);

    easing< EaseInQuart<std::int16_t> >(RED);
    easing< EaseOutQuart<std::int16_t> >(BLUE);
    easing< EaseInOutQuart<std::int16_t> >(YELLOW);

    easing< EaseInQuint<std::int16_t> >(RED);
    easing< EaseOutQuint<std::int16_t> >(BLUE);
    easing< EaseInOutQuint<std::int16_t> >(YELLOW);

    easing< EaseInSine<std::int16_t> >(RED);
    easing< EaseOutSine<std::int16_t> >(BLUE);
    easing< EaseInOutSine<std::int16_t> >(YELLOW);

    easing< EaseInExpo<std::int16_t> >(RED);
    easing< EaseOutExpo<std::int16_t> >(BLUE);
    easing< EaseInOutExpo<std::int16_t> >(YELLOW);

    easing< EaseInCirc<std::int16_t> >(RED);
    easing< EaseOutCirc<std::int16_t> >(BLUE);
    easing< EaseInOutCirc<std::int16_t> >(YELLOW);

    easing< EaseInBack<std::int16_t> >(RED);
    easing< EaseOutBack<std::int16_t> >(BLUE);
    easing< EaseInOutBack<std::int16_t> >(YELLOW);

    easing< EaseInElastic<std::int16_t> >(RED);
    easing< EaseOutElastic<std::int16_t> >(BLUE);
    easing< EaseInOutElastic<std::int16_t> >(YELLOW);

    easing< EaseInBounce<std::int16_t> >(RED);
    easing< EaseOutBounce<std::int16_t> >(BLUE);
    easing< EaseInOutBounce<std::int16_t> >(YELLOW);
}

#endif
