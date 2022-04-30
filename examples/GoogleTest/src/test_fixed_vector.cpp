

#include <gob_fixed_vector.hpp>
using goblib::FixedVector;

#include "gtest/gtest.h"
#include <numeric>
#include <memory>


#if defined(ARDUINO_M5Stack_Core_ESP32)
#include <esp_system.h>
#endif

namespace
{

constexpr std::size_t sz = 10;

GOBLIB_MAYBE_UNUSED void print_v(const FixedVector<int, sz>& v)
{
    for(auto& e : v)
    {
        printf("%d,", e);
    }
    printf("%c", '\n');
}
}


TEST(FixedVector, Basic)
{
#if defined(ARDUINO_M5Stack_Core_ESP32)
    auto heap = esp_get_free_heap_size();
#endif

    {
        //  printf("---vn\n");
        FixedVector<int,sz> vn(5);
        EXPECT_EQ(vn.size(), 5);
        EXPECT_EQ(vn.max_size(), sz);
        EXPECT_EQ(vn.capacity(), sz);
        EXPECT_EQ(std::accumulate(vn.begin(), vn.end(), 0), 0);

        //  printf("---vnt\n");
        FixedVector<int,sz> vnt(4U, 8);
        EXPECT_EQ(vnt.size(), 4);
        EXPECT_EQ(vnt.max_size(), sz);
        EXPECT_EQ(vnt.capacity(), sz);
        EXPECT_EQ(std::accumulate(vnt.begin(), vnt.end(), 0), 32);

        //  printf("---fc\n");
        FixedVector<int,sz> fc({3,3,4});
        EXPECT_EQ(fc.size(), 3);
        EXPECT_EQ(fc.max_size(), sz);
        EXPECT_EQ(fc.capacity(), sz);
        EXPECT_EQ(std::accumulate(fc.begin(), fc.end(), 0), 10);

        //  printf("---cc = fc\n");
        FixedVector<int,sz> cc = fc;
        EXPECT_EQ(cc.size(), 3);
        EXPECT_EQ(cc.max_size(), sz);
        EXPECT_EQ(cc.capacity(), sz);
        EXPECT_EQ(std::accumulate(cc.begin(), cc.end(), 0), 10);

        //  printf("---eq\n");
        FixedVector<int,sz> eq;
        eq = fc;
        EXPECT_EQ(eq.size(), 3);
        EXPECT_EQ(eq.max_size(), sz);
        EXPECT_EQ(eq.capacity(), sz);
        EXPECT_EQ(std::accumulate(eq.begin(), eq.end(), 0), 10);

        //  printf("---eq move vnt)\n");
        eq = std::move(vnt);
        EXPECT_EQ(vnt.data(), nullptr);
        EXPECT_TRUE(vnt.empty());
        EXPECT_EQ(eq.size(), 4);
        EXPECT_EQ(eq.max_size(), sz);
        EXPECT_EQ(eq.capacity(), sz);
        EXPECT_EQ(std::accumulate(eq.begin(), eq.end(), 0), 32);
        //  printf("---eq2\n");
        FixedVector<int,sz> eq2 = std::move(eq);
        EXPECT_EQ(eq.data(), nullptr);
        EXPECT_TRUE(eq.empty());
        EXPECT_EQ(eq2.size(), 4);
        EXPECT_EQ(eq2.max_size(), sz);
        EXPECT_EQ(eq2.capacity(), sz);
        EXPECT_EQ(std::accumulate(eq2.begin(), eq2.end(), 0), 32);
        //#if 0

        FixedVector<int,sz> v;

        EXPECT_TRUE(v.empty());
        EXPECT_EQ(v.size(), 0);
        EXPECT_EQ(v.max_size(), sz);
        EXPECT_EQ(v.capacity(), sz);

        int x = 1;
        v.push_back(x);
        EXPECT_FALSE(v.empty());
        EXPECT_EQ(v.size(), 1);
        EXPECT_EQ(v.max_size(), sz);
        EXPECT_EQ(v.capacity(), sz);
        EXPECT_EQ(v.back(), 1);
        EXPECT_EQ(v.front(), 1);

        v.pop_back();
        EXPECT_TRUE(v.empty());
        EXPECT_EQ(v.size(), 0);
        EXPECT_EQ(v.max_size(), sz);
        EXPECT_EQ(v.capacity(), sz);

        for(int i=0; i < sz/2; ++i)
        {
            v.push_back(i+1);
        }
        for(int i=0;i<sz/2;++i)
        {
            EXPECT_EQ(v[i], i+1);
            EXPECT_EQ(v.at(i), i+1);

        }
        EXPECT_FALSE(v.empty());
        EXPECT_EQ(v.size(), sz/2);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), (sz/2 * (sz/2+1)) /2 );

        auto pos = v.insert(v.end(), {6,7,8,9,10});
        EXPECT_EQ(*pos, 6);
        EXPECT_EQ(v.size(), sz);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), (sz * (sz+1)) /2 );

        v.clear();
        EXPECT_TRUE(v.empty());
        EXPECT_EQ(v.size(), 0);
    
        pos = v.insert(v.begin(), 3U, 10);
        EXPECT_EQ(*pos, 10);
        EXPECT_EQ(v.size(), 3);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 30);

        pos = v.insert(v.begin() + 1, 5);
        EXPECT_EQ(*pos, v[1]);
        EXPECT_EQ(v.size(), 4);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 35);

    
        v.assign(6U, 11);
        EXPECT_EQ(v.size(), 6);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 66);
    
        v.assign({1,9});
        EXPECT_EQ(v.size(), 2);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 10);
    
        auto& ref = v.front();
        EXPECT_EQ(&ref, v.data());

        v.resize(6U, 52);
        EXPECT_EQ(v.size(), 6);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 10+52*4);
        v.resize(3U);
        EXPECT_EQ(v.size(), 3);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 10+52);
        v.erase(v.end());
        EXPECT_EQ(v.size(), 2);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 10);
        v.erase(v.begin() + 1);
        EXPECT_EQ(v.size(), 1);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 1);

        pos = v.emplace(v.begin(), 234);
        EXPECT_EQ(*pos, 234);
        EXPECT_EQ(v.back(), 1);
        EXPECT_EQ(v.size(), 2);
        EXPECT_EQ(v.front(), 234);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 235);
        v.emplace_back(65);
        EXPECT_EQ(v.size(), 3);
        EXPECT_EQ(v.back(), 65);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 300);

        v.assign({0,1,0,2,3,0,4,5,6,0});
        EXPECT_EQ(v.size(), 10);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 21);
        auto rm = std::remove(v.begin(), v.end(), 0);
        EXPECT_EQ(v.size(), 10);
        v.erase(rm, v.end());
        EXPECT_EQ(v.size(), 6);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 21);

        int val = 6;
        std::for_each(v.crbegin(), v.crend(), [&val](int e) { EXPECT_EQ(val, e); --val;});
        EXPECT_EQ(v.back(), *v.rbegin());
        EXPECT_EQ(v.front(), *(v.rend() - 1));

        FixedVector<int,sz> sv;
        EXPECT_TRUE(sv.empty());
        sv.swap(v);
        EXPECT_TRUE(v.empty());
        EXPECT_EQ(sv.size(), 6);
        EXPECT_EQ(std::accumulate(sv.begin(), sv.end(), 0), 21);

        std::swap(v,sv);
        EXPECT_TRUE(sv.empty());
        EXPECT_EQ(v.size(), 6);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 21);

        EXPECT_EQ(v[0], 1);
        v[0] = 111;
        EXPECT_EQ(v[0], 111);
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0), 20+111);
    }
    
#if defined(ARDUINO_M5Stack_Core_ESP32)
    auto heap2 = esp_get_free_heap_size();
    EXPECT_EQ(heap, heap2);
#endif
    
}

TEST(FixedVector, operator)
{
    FixedVector<int, 4> v4(4);
    FixedVector<int, 4> v4_2(4);
    FixedVector<int, 4> v3(3);
    FixedVector<int, 4> v2(2);

    EXPECT_EQ(v4.size(), 4);
    EXPECT_EQ(v4_2.size(), 4);
    EXPECT_EQ(v3.size(), 3);
    EXPECT_EQ(v2.size(), 2);
    
    EXPECT_TRUE(v4 == v4);
    EXPECT_TRUE(v4 == v4_2);
    EXPECT_TRUE(v4 >= v4_2);
    EXPECT_TRUE(v4 <= v4_2);
    EXPECT_FALSE(v4 == v2);

    v4 = {1,2,3,4};
    v4_2 = { 1,2,-1,99 };

    EXPECT_TRUE(v4 > v4_2);
    EXPECT_TRUE(v4 >= v4_2);
    EXPECT_TRUE(v4 > v3);
    EXPECT_TRUE(v3 >= v2);

    EXPECT_TRUE(v4_2 <  v4);
    EXPECT_TRUE(v4_2 <= v4);
    EXPECT_TRUE(v3 < v4);
    EXPECT_TRUE(v2 <= v3);
}

TEST(FixedVector, iterator)
{
    using FV = FixedVector<int, 4>;
    FV v = { 0,1,2,3 };

    int c = 0;
    for(auto it = v.begin(); it < v.end(); ++it, ++c)
    {
        EXPECT_EQ(*it, c);
    }
    c = 0;
    for(FV::const_iterator it = v.begin(); it < v.end(); ++it, ++c)
    {
        EXPECT_EQ(*it, c);
    }
    c = 0;
    for(FV::const_iterator it = v.cbegin(); it < v.cend(); ++it, ++c)
    {
        EXPECT_EQ(*it, c);
    }


    c = 3;
    for(auto it = v.rbegin(); it < v.rend(); ++it, --c)
    {
        EXPECT_EQ(*it, c);
    }
    c = 3;
    for(FV::const_reverse_iterator it = v.rbegin(); it < v.rend(); ++it, --c)
    {
        EXPECT_EQ(*it, c);
    }
    c = 3;
    for(FV::const_reverse_iterator it = v.crbegin(); it < v.crend(); ++it, --c)
    {
        EXPECT_EQ(*it, c);
    }
}


namespace
{

struct Base
{
    Base(int x = 0) : _base_v(x) {}
    Base(const Base& x) : _base_v(x._base_v) {}
    Base(Base&& x) : _base_v(x._base_v) {}
    virtual ~Base(){}
    virtual int func() { return _base_v; }
    int _base_v;
};


struct Dummy : public Base
{
    Dummy() : Base(), _v(0) { ++cnt_c; }
    explicit Dummy(std::int32_t v) : Base(-v), _v(v)  { ++cnt_c; }
    Dummy(const Dummy& x) : Base(), _v(x._v) { ++cnt_c;}
    Dummy(Dummy&& x) : Base(), _v(x._v) { ++cnt_c;}

    virtual ~Dummy() { ++cnt_d; }
    virtual int func() override { return _v; }

    std::int32_t _v;

    static std::uint32_t cnt_c,cnt_d;
};
std::uint32_t Dummy::cnt_c = 0;
std::uint32_t Dummy::cnt_d = 0;
}


TEST(FixedVector, for_class)
{
#if defined(ARDUINO_M5Stack_Core_ESP32)
    auto heap = esp_get_free_heap_size();
#endif
    
    using Vec = FixedVector<Dummy, 8>;
    {
        Vec v(8, Dummy(10));
        EXPECT_EQ(std::accumulate(v.begin(), v.end(), 0, [](int acc,Dummy& e) { return acc + e.func(); }), 80 );
    }
    //printf("C:%d D:%d\n", Dummy::cnt_c, Dummy::cnt_d);
    EXPECT_EQ(Dummy::cnt_c, 8 + 1 ); /* 8elements + v(8,Dummy()) */
    EXPECT_EQ(Dummy::cnt_c, Dummy::cnt_d);

    
#if defined(ARDUINO_M5Stack_Core_ESP32)
    auto heap2 = esp_get_free_heap_size();
    EXPECT_EQ(heap, heap2);
#endif

}

