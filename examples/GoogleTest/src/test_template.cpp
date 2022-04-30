
#include <gob_template_helper.hpp>
using namespace goblib::template_helper;
#include "gob_math.hpp"
#include <gob_m5s_clock.hpp>
#include "gtest/gtest.h"
#include <chrono>
#include <array>

namespace
{
struct Empty {};
struct HasLess { bool operator<(const HasLess& o) { return true; } };
struct HasEqual { bool operator==(const HasEqual& o) { return true; } };
struct HasLessAndEqual{};
bool operator==(const HasLessAndEqual& a, const HasLessAndEqual& b) { return true; }
bool operator<(const HasLessAndEqual& a, const HasLessAndEqual& b) { return true; }
struct Base {};
struct Delivered : public Base {};


int foo(const char*, int, float = 1.0f, void* p = nullptr) { return 42; }
void bar(...) {}
const int& foobar(Base*) { static int aaa = 24; return aaa; }

struct functor
{
    int operator()(int a, int b) const { return a + b; }
    const char* operator()(const char* a, const char* b) const { return nullptr; }
};

auto lambda_func = [](float a, float b) { return a + b; };

template<typename T> auto temp_func(const T& v, const T& low, const T& high) -> typename std::enable_if<std::is_floating_point<T>::value, T>::type { return v; }

}

TEST(Template, Basic)
{
    //
    auto b = can_determine_less<int,int>::value;
    EXPECT_TRUE(b);
    b = can_determine_less<int,float>::value;
    EXPECT_TRUE(b);
    b = can_determine_less<int,Empty>::value;
    EXPECT_FALSE(b);
    b = can_determine_less<Empty,Empty>::value;
    EXPECT_FALSE(b);
    b = can_determine_less<HasLess, HasLess>::value;
    EXPECT_TRUE(b);
    b = can_determine_less<HasLessAndEqual, HasLessAndEqual>::value;
    EXPECT_TRUE(b);
    b = can_determine_less<HasLess, HasLessAndEqual>::value;
    EXPECT_FALSE(b);

    //
    b = can_determine_equal<int,int>::value;
    EXPECT_TRUE(b);
    b = can_determine_equal<int,float>::value;
    EXPECT_TRUE(b);
    b = can_determine_equal<int,Empty>::value;
    EXPECT_FALSE(b);
    b = can_determine_equal<Empty,Empty>::value;
    EXPECT_FALSE(b);
    b = can_determine_equal<HasEqual,HasLess>::value;
    EXPECT_FALSE(b);
    b = can_determine_equal<HasEqual,HasEqual>::value;
    EXPECT_TRUE(b);
    b = can_determine_equal<HasLessAndEqual,HasLessAndEqual>::value;
    EXPECT_TRUE(b);

    //
    b = is_callable<decltype(foo), const char*, int, float, void*>::value;
    EXPECT_TRUE(b);
    b = is_callable<decltype(foo), const char*, int>::value;
    EXPECT_FALSE(b);
    b = is_callable<decltype(foo), void>::value;
    EXPECT_FALSE(b);

    b = is_callable<decltype(bar), const char*, int, float, void*, HasLess&, HasEqual*>::value;
    EXPECT_TRUE(b);
    b = is_callable<decltype(bar), void>::value;
    EXPECT_TRUE(b);
    
    b = is_callable<decltype(foobar), Base*>::value;
    EXPECT_TRUE(b);
    b = is_callable<decltype(foobar), Delivered*>::value;
    EXPECT_TRUE(b);
    b = is_callable<decltype(foobar), HasLess*>::value;
    EXPECT_FALSE(b);

    b = is_callable<functor, int>::value;
    EXPECT_FALSE(b);
    b = is_callable<functor, int, int>::value;
    EXPECT_TRUE(b);
    b = is_callable<functor, int, int, int>::value;
    EXPECT_FALSE(b);
    b = is_callable<functor, int, Empty>::value;
    EXPECT_FALSE(b);
    b = is_callable<functor, const char*, const char*>::value;
    EXPECT_TRUE(b);
    b = is_callable<functor, char*, char*>::value;
    EXPECT_TRUE(b);

    b = is_callable<decltype(lambda_func), int>::value;
    EXPECT_FALSE(b);
    b = is_callable<decltype(lambda_func), int,int>::value;
    EXPECT_TRUE(b);
    b = is_callable<decltype(lambda_func), int,int,int>::value;
    EXPECT_FALSE(b);

    b = is_callable<decltype(temp_func<float>), float>::value;
    EXPECT_FALSE(b);
    b = is_callable<decltype(temp_func<float>), float,float,float>::value;
    EXPECT_TRUE(b);
    b = is_callable<decltype(temp_func<float>), float,float,float,int>::value;
    EXPECT_FALSE(b);

    //
    typedef bool (&PF1)();
    typedef short (*PF2)(long);

    struct S {
        operator PF2() const;
        double operator()(char, int&);
        void fn(long) const;
        char data;
    };

    typedef void (S::*PMF)(long) const;
    typedef char S::*PMD;

    b = is_return_type<short, S, int>::value;
    EXPECT_TRUE(b);
    b = is_return_type<void, S, int>::value;
    EXPECT_FALSE(b);
    b = is_return_type<double, S&, unsigned char, int&>::value;
    EXPECT_TRUE(b);
    b = is_return_type<int, S&, unsigned char, int&>::value;
    EXPECT_FALSE(b);
    b = is_return_type<bool, PF1>::value;
    EXPECT_TRUE(b);
    b = is_return_type<const char*, PF1>::value;
    EXPECT_FALSE(b);
    b = is_return_type<void, PMF, std::unique_ptr<S>, int>::value;
    EXPECT_TRUE(b);
    b = is_return_type<float, PMF, std::unique_ptr<S>, int>::value;
    EXPECT_FALSE(b);
    b = is_return_type<char&&, PMD, S>::value;
    EXPECT_TRUE(b);
    b = is_return_type<double&&, PMD, S>::value;
    EXPECT_FALSE(b);
    b = is_return_type<const char&, PMD, const S*>::value;
    EXPECT_TRUE(b);
    b = is_return_type<std::nullptr_t&, PMD, const S*>::value;
    EXPECT_FALSE(b);
}

TEST(Template, is_clock)
{
    //
    auto b = goblib::template_helper::is_clock<std::chrono::system_clock>::value;
    EXPECT_TRUE(b);
    b = goblib::template_helper::is_clock<std::chrono::steady_clock>::value;
    EXPECT_TRUE(b);
    b = goblib::template_helper::is_clock<std::chrono::high_resolution_clock>::value;
    EXPECT_TRUE(b);
    b = goblib::template_helper::is_clock<goblib::m5s::arduino_clock>::value;
    EXPECT_TRUE(b);
    b = goblib::template_helper::is_clock<goblib::m5s::esp_clock>::value;
    EXPECT_TRUE(b);
    b = goblib::template_helper::is_clock<int>::value;
    EXPECT_FALSE(b);
    b = goblib::template_helper::is_clock<HasLess>::value;
    EXPECT_FALSE(b);

    // is_rng test see also test_random.cpp
}

namespace
{
void g(std::int16_t s0, std::int16_t s1, std::int16_t s2, std::int16_t s3)
{
    EXPECT_EQ(s0, 0);
    EXPECT_EQ(s1, 1);
    EXPECT_EQ(s2, 2);
    EXPECT_EQ(s3, 3);
}

void g(std::size_t s0, std::size_t s1, std::size_t s2, std::size_t s3)
{
    EXPECT_EQ(s0, 0);
    EXPECT_EQ(s1, 1);
    EXPECT_EQ(s2, 2);
    EXPECT_EQ(s3, 3);
}

template <class T, T... Seq> void f(goblib::template_helper::integer_sequence<T, Seq...>)
{
    g(Seq...);
}
//
}

TEST(Template, index_sequence)
{
    f(goblib::template_helper::make_integer_sequence<std::int16_t, 4>());
    f(goblib::template_helper::make_index_sequence<4>());
}

namespace
{
constexpr float tsin(int x) { return std::sin(goblib::math::deg2rad(x*360.0f/32.0f)); }
constexpr auto sin_table = goblib::template_helper::table::generator<32>(tsin);

constexpr std::uint8_t tatan2(std::size_t v) { return goblib::math::round(goblib::math::rad2deg(std::atan2(v/20, v%20)) / (360/32.0f)); }
constexpr auto atan2_table = goblib::template_helper::table::generator<20*15>(tatan2);

std::uint8_t tatan2(std::int16_t y, std::int16_t x)
{
    if(y==x && y==0) { return 0; }
    x /= 16; y /= 16;
    auto v = atan2_table[ std::abs(y)*20 + std::abs(x) ];
    if(x<0) { v = 16 - v; }
    if(y<0) { v = 32 - v; }
    return v;
}
}

TEST(Template, table)
{
    EXPECT_EQ(sin_table.size(), 32);
    EXPECT_FLOAT_EQ(sin_table[0],   0.0f);
    EXPECT_FLOAT_EQ(sin_table[8],   1.0f);
    EXPECT_NEAR(sin_table[16],  -0.0f, 0.0001f);
    EXPECT_FLOAT_EQ(sin_table[24], -1.0f);

    //    printf("---> %f : %f\n", sin_table[16], std::sin(goblib::math::deg2rad(16*360.0f/32.0f)) );
    
    EXPECT_EQ(atan2_table[0], 0);
    EXPECT_EQ(atan2_table[20], 8);
    EXPECT_EQ(atan2_table[21], 4);
    EXPECT_EQ(atan2_table[22], 2);
    EXPECT_EQ(atan2_table[40], 8);
    EXPECT_EQ(atan2_table[41], 6);
    EXPECT_EQ(atan2_table[60], 8);
    EXPECT_EQ(atan2_table[61], 6);

    /*
    for(int y = 0; y < 15; ++y)
    {
        for(int x = 0; x < 20; ++x)
        {
            printf("%2d ", atan2_table[y*20+x]);
        }
        printf("\n");
    }
    */

    EXPECT_EQ( 4, tatan2( 16, 16));
    EXPECT_EQ(12, tatan2( 16,-16));
    EXPECT_EQ(20, tatan2(-16,-16));
    EXPECT_EQ(28, tatan2(-16, 16));
}


#if 0
TEST(Template, benchmark)
{
    using clock = goblib::m5s::esp_clock;
    //    using clock = std::chrono::high_resolution_clock;

    {
        float acc = 0;
        auto s = clock::now();
        for(int y = -239; y < 240; ++y)
        {
            for(int x = -319; x < 320; ++x)
            {
                acc += std::atan2(y,x);
            }
        }
        auto e = clock::now();
        auto d = e - s;
        printf("atan2 calc:%lld : %f\n", d.count(), acc);
    }

    {
        float acc = 0;
        auto s = clock::now();
        for(int y = -239; y < 240; ++y)
        {
            for(int x = -319; x < 320; ++x)
            {
                acc += tatan2(y,x);
            }
        }
        auto e = clock::now();
        auto d = e - s;
        printf("atan2 table:%lld : %f\n", d.count(), acc);
    }

    {
        auto r = goblib::math::deg2rad(32.0f);
        float acc = 0;
        auto s = clock::now();
        for(int i=0;i<1000;++i)
        {
            acc += std::sin(r * i);
        }
        auto e = clock::now();
        auto d = e - s;
        printf("sin calc:%lld : %f\n", d.count(), acc);
    }

    {
        float acc = 0;
        auto s = clock::now();
        for(int i=0;i<1000;++i)
        {
            acc += sin_table[i & 31];
        }
        auto e = clock::now();
        auto d = e - s;
        printf("sin table:%lld : %f\n", d.count(), acc);
    }
}
#endif

