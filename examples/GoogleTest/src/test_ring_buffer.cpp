/*
  Unitest for gob_ringbuffer
*/
#include <cstdio>

#include <gob_ring_buffer.hpp>
#include <gob_template_helper.hpp>
using goblib::RingBuffer;
#include "gtest/gtest.h"
#include <algorithm>

namespace
{
constexpr int val(int x) { return x; }
constexpr auto table = goblib::template_helper::table::generator<100>(val); // std::array 0...99
}


TEST(RingBuffer, Basic)
{
    RingBuffer<int, 4> rbuf;

    EXPECT_TRUE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.capacity(), 4);
    EXPECT_EQ(rbuf.size(), 0);

    // H
    // |   |   |   | 1 | 
    //             T    
    rbuf.push_front(1);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 1);
    EXPECT_EQ(rbuf[0], 1);

    // H  
    // |   |   |   |   | 
    // T
    int v = rbuf.front();
    rbuf.pop_front();
    EXPECT_EQ(v, 1);
    EXPECT_TRUE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 0);

    //     H  
    // | 2 |   |   |   | 
    // T
    rbuf.push_back(2);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 1);
    EXPECT_EQ(rbuf[0], 2);

    // H  
    // |   |   |   |   | 
    // T
    v = rbuf.back();
    rbuf.pop_back();
    EXPECT_EQ(v, 2);
    EXPECT_TRUE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 0);

    //         H  
    // | 4 | 5 | 6 | 3 |
    //         T
    rbuf.push_front(3);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 1);

    rbuf.push_back(4);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 2);

    rbuf.push_back(5);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 3);

    rbuf.push_front(6);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_TRUE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 4);
    EXPECT_EQ(rbuf[0], 6);
    EXPECT_EQ(rbuf[1], 3);
    EXPECT_EQ(rbuf[2], 4);
    EXPECT_EQ(rbuf[3], 5);

    //     H  
    // | 4 | 7 | 6 | 3 |
    //     T
    rbuf.push_front(7);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_TRUE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 4);
    EXPECT_EQ(rbuf[0], 7);

    //         H  
    // | 4 | 8 | 6 | 3 |
    //         T
    rbuf.push_back(8);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_TRUE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 4);
    EXPECT_EQ(rbuf[0], 6);
    EXPECT_EQ(rbuf[1], 3);
    EXPECT_EQ(rbuf[2], 4);
    EXPECT_EQ(rbuf[3], 8);

    //         H  
    // |   |   |   |   |
    //         T
    rbuf.pop_back();
    EXPECT_EQ(rbuf.back(), 4);
    rbuf.pop_back();
    EXPECT_EQ(rbuf.back(), 3);
    rbuf.pop_back();
    EXPECT_EQ(rbuf.back(), 6);
    rbuf.pop_back();
    EXPECT_TRUE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 0);

    //             H  
    // | 11| 12| 13| 10 |
    //             T
    rbuf.push_back(9);
    rbuf.push_back(10);
    rbuf.push_back(11);
    rbuf.push_back(12);
    rbuf.push_back(13);
    EXPECT_FALSE(rbuf.empty());
    EXPECT_TRUE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 4);
    EXPECT_EQ(rbuf[0], 10);
    EXPECT_EQ(rbuf[1], 11);
    EXPECT_EQ(rbuf[2], 12);
    EXPECT_EQ(rbuf[3], 13);

    //             H  
    // |   |   | 13|   |
    //         T        
    EXPECT_EQ(rbuf.front(), 10);
    rbuf.pop_front();
    EXPECT_EQ(rbuf.front(), 11);
    rbuf.pop_front();
    EXPECT_EQ(rbuf.front(), 12);
    rbuf.pop_front();
    EXPECT_FALSE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 1);

    //         H  
    // |   |   |   |   |
    //         T
    rbuf.clear();
    EXPECT_TRUE(rbuf.empty());
    EXPECT_FALSE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 0);

    // H  
    // |111|111|111|111|
    // T
    rbuf.fill(111);
    EXPECT_TRUE(rbuf.full());
    EXPECT_EQ(rbuf.size(), 4);
    EXPECT_EQ(rbuf.front(), 111);
    EXPECT_EQ(rbuf.back(), 111);
}

TEST(RingBuffer, constructor)
{
    {
        RingBuffer<int, 8> rbuf(2U, 52);
        EXPECT_FALSE(rbuf.empty());
        EXPECT_FALSE(rbuf.full());
        EXPECT_EQ(rbuf.capacity(), 8);
        EXPECT_EQ(rbuf.size(), 2);
        EXPECT_EQ(rbuf[0], 52);
        EXPECT_EQ(rbuf[1], 52);
    }
    {
        RingBuffer<int, 8> rbuf(100U, 52);
        EXPECT_FALSE(rbuf.empty());
        EXPECT_TRUE(rbuf.full());
        EXPECT_EQ(rbuf.capacity(), 8);
        EXPECT_EQ(rbuf.size(), 8);
        EXPECT_EQ(rbuf.front(), 52);
        EXPECT_EQ(rbuf.back(), 52);
    }

    {
        RingBuffer<int, 10> rbuf2({ 9,8,7,6,5 });
        EXPECT_FALSE(rbuf2.empty());
        EXPECT_FALSE(rbuf2.full());
        EXPECT_EQ(rbuf2.capacity(), 10);
        EXPECT_EQ(rbuf2.size(), 5);
        EXPECT_EQ(rbuf2[0], 9);
        EXPECT_EQ(rbuf2[1], 8);
        EXPECT_EQ(rbuf2[2], 7);
        EXPECT_EQ(rbuf2[3], 6);
        EXPECT_EQ(rbuf2[4], 5);
    }
    {
        RingBuffer<int, 8> rbuf2(table.begin(), table.end());
        EXPECT_FALSE(rbuf2.empty());
        EXPECT_TRUE(rbuf2.full());
        EXPECT_EQ(rbuf2.capacity(), 8);
        EXPECT_EQ(rbuf2.size(), 8);
        EXPECT_EQ(rbuf2.front(), 100-8);
        EXPECT_EQ(rbuf2.back(), 99);
    }
    
    RingBuffer<float, 3> rbuf3 = { 1.1f, 2.2f, 3.3f };
    EXPECT_FALSE(rbuf3.empty());
    EXPECT_TRUE(rbuf3.full());
    EXPECT_EQ(rbuf3.capacity(), 3);
    EXPECT_EQ(rbuf3.size(), 3);
    EXPECT_FLOAT_EQ(rbuf3[0], 1.1f);
    EXPECT_FLOAT_EQ(rbuf3[1], 2.2f);
    EXPECT_FLOAT_EQ(rbuf3[2], 3.3f);

    //    rbuf3[1] = 123.456f;
    //    EXPECT_FLOAT_EQ(rbuf3[1], 123.456f);
}

TEST(RingBuffer, iterator)
{
    RingBuffer<int,4> rb = { 0,1,2 };
    RingBuffer<int,4> rb2;
    int c = 0;
    for(auto it = rb.cbegin(); it != rb.cend(); ++it) { EXPECT_EQ(*it, c++); }

    rb.clear();
    rb.push_back(9);
    rb.push_back(8);
    rb.push_back(7);
    rb.push_back(6);
    c = 9;
    for(auto&& e : rb) { EXPECT_EQ(e, c--); }
    c = 9;
    std::for_each(std::begin(rb), std::end(rb), [&c](const int& e) { EXPECT_EQ(e, c--); });

    c = 6;
    for(auto it = rb.cend(); it != rb.cbegin(); /**/)
    {
        --it;
        EXPECT_EQ(*it, c++);
    }
    #if 0
    {
        auto it = rb.begin();
        // *it = 1; // Compile error. it is const reference.
    }
    #endif

    using Iter = RingBuffer<int,4>::const_iterator;
    {
        Iter it = rb.begin();
        EXPECT_EQ(*it++, 9);
        EXPECT_EQ(*it--, 8);
        EXPECT_EQ(*++it, 8);
        EXPECT_EQ(*--it, 9);
    }

    {
        Iter it = rb.begin();
        it = it + 3;
        EXPECT_EQ(*it, 6);
        it = it - 2;
        EXPECT_EQ(*it, 8);
    }
    
    {
        Iter it_o = rb2.begin();
        Iter it0 = rb.begin() + 0;
        Iter it1 = rb.begin() + 1;
        Iter it2 = rb.begin() + 2;
        Iter it3 = rb.begin() + 3;
        Iter it11 = rb.begin();
        Iter it22 = rb.begin();
        Iter it33 = rb.begin();
        ++it11;
        it22++; ++it22;
        ++it33; it33++; ++it33;

        EXPECT_FALSE(it1 == it_o);
        EXPECT_TRUE(it1 == it11);
        EXPECT_TRUE(it1 != it2);
        EXPECT_TRUE(it0 < it1);
        EXPECT_TRUE(it2 > it0);
        EXPECT_TRUE(it2 <= it22);
        EXPECT_TRUE(it2 <= it33);
        EXPECT_TRUE(it3 >= it33);
        EXPECT_TRUE(it3 >= it11);

        EXPECT_EQ(rb.end() - it0, 4);
        EXPECT_EQ(it3 - it0, 3);
        EXPECT_EQ(it2 - it0, 2);
        EXPECT_EQ(it1 - it0, 1);
        EXPECT_EQ(it0 - rb.begin(), 0);
    }
}

#if defined(GOBLIB_CPP17_OR_LATER)
TEST(RingBuffer, optional)
{
    RingBuffer<int,4> rb;

    auto o = rb.front_optional();
    EXPECT_FALSE(o);
    o = rb.back_optional();
    EXPECT_FALSE(o);

    //     H
    // | 1 |   |   | -10|
    //             T
    rb.push_back(1);
    rb.push_front(-10);

    o = rb.front_optional();
    EXPECT_TRUE(o);
    EXPECT_EQ(o.value(), -10);
    EXPECT_EQ(o.value(), rb.front());

    o = rb.back_optional();
    EXPECT_TRUE(o);
    EXPECT_EQ(o.value(), 1);
    EXPECT_EQ(o.value(), rb.back());

    o = rb.at_optional(0);
    EXPECT_TRUE(o);
    EXPECT_EQ(o.value(), -10);
    o = rb.at_optional(1);
    EXPECT_TRUE(o);
    EXPECT_EQ(o.value(), 1);
    o = rb.at_optional(100);
    EXPECT_FALSE(o);
}
#endif


TEST(RingBuffer, read)
{
    RingBuffer<int,128> rb;
    int buf[256] = {0,};
    std::size_t rcnt = 0;

    //
    for(int i=0;i<128+128/2;++i) { rb.push_back(i); }

    //    printf("F:%d H:%zu T:%zu\n", rb._full, rb._head, rb._tail);

    EXPECT_EQ(rb.size(), 128);

    rcnt = rb.read(buf,64);
    EXPECT_EQ(rcnt, 64);
    EXPECT_EQ(rb.size(), 64);
    EXPECT_EQ(rb.front(), 128);
    EXPECT_EQ(rb.back(), 191);
    for(int i=0;i<64;++i) { EXPECT_EQ(buf[i], 64 + i); }

    rcnt = rb.read(buf, 64);
    EXPECT_EQ(rcnt, 64);
    EXPECT_TRUE(rb.empty());
    for(int i=0;i<64;++i) { EXPECT_EQ(buf[i], 128 + i); }

    //
    for(int i=0;i<32;++i) { rb.push_back(i); }
    EXPECT_EQ(rb.size(), 32);
    EXPECT_EQ(rb.front(), 0);
    EXPECT_EQ(rb.back(), 31);
    
    rcnt = rb.read(buf, 128);
    EXPECT_EQ(rcnt, 32);
    EXPECT_TRUE(rb.empty());
    for(int i=0;i<32;++i) { EXPECT_EQ(buf[i], i); }
    
    //    printf("F:%d H:%zu T:%zu\n", rb._full, rb._head, rb._tail);

    //
    for(int i=0;i<128;++i) { rb.push_back(i); }
    EXPECT_EQ(rb.size(), 128);
    EXPECT_EQ(rb.front(), 0);
    EXPECT_EQ(rb.back(), 127);

    rcnt = rb.read(buf, 256);

    //    printf("F:%d H:%zu T:%zu\n", rb._full, rb._head, rb._tail);

    EXPECT_EQ(rcnt, 128);
    EXPECT_TRUE(rb.empty());
    for(int i=0;i<128;++i) { EXPECT_EQ(buf[i], i); }

    //
    for(int i=0;i<128;++i) { rb.push_back(i); }
    EXPECT_TRUE(rb.full());
    for(int i=0;i<128;++i)
    {
        rb.read(buf, 1);
        EXPECT_EQ(buf[0], i);
        EXPECT_EQ(rb.size(), 128 - (i+1) );
    }
}


TEST(RingBuffer, write)
{

    RingBuffer<int,64> rb;
    RingBuffer<int,64> rb2;
    
    //    printf("F:%d H:%zu T:%zu\n", rb._full, rb._head, rb._tail);
    for(int i=0; i<6; ++i)
    {
        rb.write(table.data() + (i * 10), 10);
        EXPECT_EQ(rb.size(), 10 * (i + 1));
        EXPECT_EQ(rb.front(), 0);
        EXPECT_EQ(rb.back(), (i + 1) * 10 - 1);

        for(int j=0;j<10;++j) { rb2.push_back(j+(i*10)); }
        //        printf("[%d] F:%d H:%zu T:%zu\n", i, rb._full, rb._head, rb._tail);
        //        printf("[%d] F:%d H:%zu T:%zu\n", i, rb2._full, rb2._head, rb2._tail);
    }
    //    printf("rb:%zu rb2:%zu\n", rb.capacity() - rb.size(), rb2.capacity() - rb2.size());
    
    rb.write(table.data() + 60, 10);
    for(int j=0;j<10;++j) { rb2.push_back(j+60); }

    EXPECT_TRUE(rb.full());
    EXPECT_EQ(rb.size(), 64);
    EXPECT_EQ(rb.front(), 6);
    EXPECT_EQ(rb.back(), 69);
    EXPECT_EQ(rb.front(), rb2.front());
    EXPECT_EQ(rb.back(), rb2.back());
                
    //    printf("F:%d H:%zu T:%zu\n", rb._full, rb._head, rb._tail);
    //    printf("F:%d H:%zu T:%zu\n", rb2._full, rb2._head, rb2._tail);
    
    EXPECT_TRUE(rb.full());
    EXPECT_EQ(rb.size(), 64);
    EXPECT_EQ(rb.front(), 6);
    EXPECT_EQ(rb.back(), 69);

}
