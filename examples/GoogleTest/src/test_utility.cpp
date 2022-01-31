/*
  Unittest for gob_utility
*/
#include <gob_utility.hpp>
#include "helper_constants.hpp"
using namespace test::constants;

#include "gtest/gtest.h"

#include <vector>
#include <stack>
#include <map>
#include <limits>

#include "helper_constants.hpp"


class TestC
{
  public:
    TestC() { _value = _count; ++_count; }
    ~TestC() { --_count; }

    static std::int32_t count() { return _count; }
    std::int32_t value() const { return _value; }

  private:
    std::int32_t _value;

    static std::int32_t _count;
};
std::int32_t TestC::_count = 0;


TEST(Utility, safeDelete)
{
    TestC* ptr = nullptr;

    EXPECT_EQ(0, TestC::count());

    // Single
    ptr = new TestC();
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(0, ptr->value());
    EXPECT_EQ(1, TestC::count());

    goblib::safeDelete(ptr);
    EXPECT_TRUE(ptr == nullptr);
    EXPECT_EQ(0, TestC::count());

    // Array
    constexpr std::int32_t anum = 32;
    ptr = new TestC[anum];
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(anum, TestC::count());

    goblib::safeDeleteArray(ptr);
    EXPECT_TRUE(ptr == nullptr);
    EXPECT_EQ(0, TestC::count());
}

class A
{
  public:
    constexpr A() : _va(0){}
  private:
    std::int32_t _va;
};

class B : public A
{
  public:
    constexpr B() : A(),_vb(1){}
  private:
    std::int32_t _vb;
};

class C : public B
{
  public:
    constexpr C() : B(), _vc(2){}
  private:
    std::int32_t _vc;
};

TEST(Utility, size)
{
    constexpr const char a_str[] = "1234567890";
    constexpr auto a_str_size = goblib::size(a_str);
    EXPECT_EQ(11U, a_str_size);
    
    C c_array[12] = {};
    EXPECT_EQ(12U, goblib::size(c_array));

    constexpr std::array<C, 13> array_c;
    EXPECT_EQ(13U, goblib::size(array_c));

    std::vector<C> vector_c0;
    auto vsz = goblib::size(vector_c0);
    EXPECT_EQ(0U, vsz);
    vector_c0.push_back(C());
    vector_c0.push_back(C());
    vector_c0.pop_back();
    vector_c0.pop_back();
    EXPECT_EQ(0U, goblib::size(vector_c0));

    std::vector<C> vector_c1{ C(), C(), C(), C(), C() };
    EXPECT_EQ(5U, goblib::size(vector_c1));
    
    std::vector<C> vector_c2(14);
    EXPECT_EQ(14U, goblib::size(vector_c2));


    std::stack<C> stack_0;
    EXPECT_EQ(0U, goblib::size(stack_0));
    stack_0.push(C());
    stack_0.push(C());
    EXPECT_EQ(2U, goblib::size(stack_0));
    stack_0.pop();
    EXPECT_EQ(1U, goblib::size(stack_0));
    stack_0.pop();
    EXPECT_EQ(0U, goblib::size(stack_0));

    std::map<char, int> map_0;
    EXPECT_EQ(0U, goblib::size(map_0));
    map_0.insert(std::make_pair('a', 1000));
    EXPECT_EQ(1U, goblib::size(map_0));
    map_0.insert(std::make_pair('b', 10000));
    map_0.insert(std::make_pair('c', 100000));
    EXPECT_EQ(3U, goblib::size(map_0));
    map_0.insert(std::make_pair('c', 111111)); // Cannot insert if key is exists.
    EXPECT_EQ(3U, goblib::size(map_0));
}



TEST(Utility, underlying)
{
    enum Enum_C : char {  A =  1,B,C };
    enum Enum_L : long { LA = 10,LB,LC };
    enum class CEnum_C : char { A =  10,B,C };
    enum class CEnum_L : std::int_fast16_t { A = 100,B,C };

    Enum_C ec  = A;
    Enum_L el = LB;
    CEnum_C cec = CEnum_C::C;
    CEnum_L cel = CEnum_L::A;

    auto b0 = std::is_same<Enum_C, char>::value;
    auto b1 = std::is_same<CEnum_L, std::int_fast16_t>::value;
    EXPECT_FALSE(b0);
    EXPECT_FALSE(b1);
    
    auto ec_t = goblib::to_underlying(ec);
    auto same_ec = std::is_same<char, decltype(ec_t)>::value;
    EXPECT_TRUE(same_ec);

    auto el_t = goblib::to_underlying(el);
    auto same_el = std::is_same<long, decltype(el_t)>::value;
    EXPECT_TRUE(same_el);

    auto cec_t = goblib::to_underlying(cec);
    auto same_cec = std::is_same<char, decltype(cec_t)>::value;
    EXPECT_TRUE(same_cec);

    auto cel_t = goblib::to_underlying(cel);
    auto same_cel = std::is_same<std::int_fast16_t, decltype(cel_t)>::value;
    EXPECT_TRUE(same_cel);
    
}

TEST(Utility, roundTrip)
{
    std::int8_t cnt_i8 = 0;
    std::uint8_t cnt_u8 = 0;
    std::int32_t cnt_i32 = 0;

    //auto assert_check_0 = goblib::roundTrip<128>(cnt_i8);
    //auto assert_check_1 = goblib::roundTrip<111>(cnt_i8);
    //auto assert_check_2 = goblib::roundTrip<32>(3.4f);

#if 0
    for(int i=0;i<260;++i)
    {
        printf("%d=>%d\n", cnt_i8, goblib::roundTrip<64>(cnt_i8));
        ++cnt_i8;
    }
#endif

    auto r_i8 = goblib::roundTrip<4>(cnt_i8);
    auto b_i8 = std::is_same<decltype(r_i8), decltype(cnt_i8)>::value;
    EXPECT_TRUE(b_i8);
    
    // int8 counter
    cnt_i8 = 0;
    EXPECT_EQ(0, (int)goblib::roundTrip<16>(cnt_i8++)); // 0 => 0
    EXPECT_EQ(1, (int)goblib::roundTrip<16>(cnt_i8++)); // 1 => 1
    cnt_i8 = 15;;
    EXPECT_EQ(15, (int)goblib::roundTrip<16>(cnt_i8++)); // 15 => 15
    EXPECT_EQ(15, (int)goblib::roundTrip<16>(cnt_i8++)); // 16 => 15
    EXPECT_EQ(14, (int)goblib::roundTrip<16>(cnt_i8++)); // 17 => 14
    cnt_i8 = 126;
    EXPECT_EQ(1, (int)goblib::roundTrip<16>(cnt_i8++)); //  126 => 1
    EXPECT_EQ(0, (int)goblib::roundTrip<16>(cnt_i8++)); //  127 => 0
    EXPECT_EQ(0, (int)goblib::roundTrip<16>(cnt_i8++)); // -128 => 0
    EXPECT_EQ(1, (int)goblib::roundTrip<16>(cnt_i8++)); // -127 => 1

    // uint8 counter
    cnt_u8 = 0;
    EXPECT_EQ(0, (int)goblib::roundTrip<32>(cnt_u8++)); // 0 => 0
    EXPECT_EQ(1, (int)goblib::roundTrip<32>(cnt_u8++)); // 1 => 1
    cnt_u8 = 31;
    EXPECT_EQ(31, (int)goblib::roundTrip<32>(cnt_u8++)); // 31 => 31
    EXPECT_EQ(31, (int)goblib::roundTrip<32>(cnt_u8++)); // 32 => 31
    EXPECT_EQ(30, (int)goblib::roundTrip<32>(cnt_u8++)); // 33 => 30
    cnt_u8 = 254;
    EXPECT_EQ(1, (int)goblib::roundTrip<32>(cnt_u8++)); //  254 => 1
    EXPECT_EQ(0, (int)goblib::roundTrip<32>(cnt_u8++)); //  255 => 0
    EXPECT_EQ(0, (int)goblib::roundTrip<32>(cnt_u8++)); //    0 => 0
    EXPECT_EQ(1, (int)goblib::roundTrip<32>(cnt_u8++)); //    1 => 1

    // int32 counter.
    //    template<int W> using rt32 = goblib::roundTrip<W>;

    cnt_i32 = 0;
    EXPECT_EQ(0, goblib::roundTrip<256>(cnt_i32++)); // 0 => 0
    EXPECT_EQ(1, goblib::roundTrip<256>(cnt_i32++)); // 1 => 1
    cnt_i32 = 255;
    EXPECT_EQ(255, goblib::roundTrip<256>(cnt_i32++)); // 255 => 255
    EXPECT_EQ(255, goblib::roundTrip<256>(cnt_i32++)); // 256 => 255
    EXPECT_EQ(254, goblib::roundTrip<256>(cnt_i32++)); // 257 => 254

    EXPECT_EQ(1, goblib::roundTrip<256>(std::numeric_limits<std::int32_t>::max() - 1));
    EXPECT_EQ(0, goblib::roundTrip<256>(std::numeric_limits<std::int32_t>::max()));

    EXPECT_EQ(0, goblib::roundTrip<256>(std::numeric_limits<std::int32_t>::min()));
    EXPECT_EQ(1, goblib::roundTrip<256>(std::numeric_limits<std::int32_t>::min() + 1));

    EXPECT_EQ(1,goblib::roundTrip<1073741824>(std::numeric_limits<std::int32_t>::max() - 1));
    EXPECT_EQ(0,goblib::roundTrip<1073741824>(std::numeric_limits<std::int32_t>::max()));
    EXPECT_EQ(0,goblib::roundTrip<1073741824>(std::numeric_limits<std::int32_t>::min()));
    
}

TEST(Utility, clamp)
{
    constexpr std::int32_t ia = 100;
#if defined(GOBLIB_CPP14_OR_LATER)
    constexpr
#endif
    auto c0 = goblib::clamp(ia, -1000, 100);
    EXPECT_EQ(ia, c0);
    EXPECT_EQ(100, goblib::clamp(ia, 100, 200));
    EXPECT_EQ(100, goblib::clamp(ia, -100, 100));

    EXPECT_EQ(-50, goblib::clamp(ia, -100, -50));
    EXPECT_EQ(300, goblib::clamp(ia, 300, 400));

    EXPECT_EQ(ia, goblib::clamp(ia, I32_MIN, I32_MAX));

    constexpr float fa = 100.12345f;

    EXPECT_FLOAT_EQ(fa, goblib::clamp(fa, -1000.0f, 1000.0f));
    EXPECT_FLOAT_EQ(fa, goblib::clamp(fa, fa, 1000.0f));
    EXPECT_FLOAT_EQ(fa, goblib::clamp(fa, -1000.0f, fa));

    EXPECT_FLOAT_EQ(-50.0f, goblib::clamp(fa, -100.0f, -50.0f));
    EXPECT_FLOAT_EQ(400.0f, goblib::clamp(fa, 400.0f, 500.0f));

    EXPECT_FLOAT_EQ(-fa, goblib::clamp(-fa, F_MIN, F_MAX));

    EXPECT_FLOAT_EQ(fa, goblib::clamp(fa, F_NINF, F_INF));
    EXPECT_FLOAT_EQ(fa, goblib::clamp(fa, F_NAN, F_NAN));
    EXPECT_FLOAT_EQ(123.0f, goblib::clamp(fa, 123.0f, F_NAN));
    EXPECT_FLOAT_EQ(90.0f, goblib::clamp(fa, F_NAN, 90.0f));
}
