/*
  Unittest for rectangle 2d.
*/
#include <gob_rect2d.hpp>
using namespace goblib;

#include <cstdint>
using Rect32 = shape2d::Rectangle<std::int32_t>;

#include "gtest/gtest.h"

#include <limits>
static constexpr std::int32_t int32_max = std::numeric_limits<std::int32_t>::max();
static constexpr std::int32_t int32_min = std::numeric_limits<std::int32_t>::min();

GOBLIB_MAYBE_UNUSED static void print_rect(const Rect32& r)
{
    printf("(%d,%d)[%d,%d]\n", r.left(), r.top(), r.width(), r.height());
}

TEST(Shape2d, Rect32_Basic)
{
    using Pos2i = goblib::shape2d::Point<std::int32_t>;
    
    constexpr Rect32 r0;
    constexpr Rect32 r1(1,2,3,4);
    Rect32 r2(Rect32(10,20,30,40));
    constexpr Rect32 r22(Pos2i(10,20), Pos2i(39, 59));
    constexpr Rect32 max_r(Rect32(int32_min, int32_min, int32_max, int32_max));

    EXPECT_EQ(r2, r22);
    
    auto cp = r1;

    EXPECT_EQ(1, cp.left());
    EXPECT_EQ(2, cp.top());
    EXPECT_EQ(3, cp.right());
    EXPECT_EQ(5, cp.bottom());
    EXPECT_EQ(3, cp.width());
    EXPECT_EQ(4, cp.height());

    EXPECT_TRUE(cp == r1);
    EXPECT_FALSE(cp != r1);
    
    EXPECT_TRUE(r0.empty());
    EXPECT_FALSE(r0.valid());
    EXPECT_FALSE(max_r.empty());
    EXPECT_TRUE(max_r.valid());

    auto lt = r2.leftTop();
    auto ct = r2.centerTop();
    auto rt = r2.rightTop();
    auto lc = r2.leftCenter();
    auto cc = r2.center();
    auto rc = r2.rightCenter();
    auto lb = r2.leftBottom();
    auto cb = r2.centerBottom();
    auto rb = r2.rightBottom();

    EXPECT_EQ(r2.left(), 10);
    EXPECT_EQ(r2.top(), 20);
    EXPECT_EQ(r2.right(), 39);
    EXPECT_EQ(r2.bottom(), 59);
    EXPECT_EQ(r2.width(), 30);
    EXPECT_EQ(r2.height(), 40);

    EXPECT_EQ(lt.x(), r2.left());
    EXPECT_EQ(lt.y(), r2.top());
    EXPECT_EQ(ct.x(), 25);
    EXPECT_EQ(ct.y(), r2.top());
    EXPECT_EQ(rt.x(), r2.right());
    EXPECT_EQ(rt.y(), r2.top());

    EXPECT_EQ(lc.x(), r2.left());
    EXPECT_EQ(lc.y(), 40);
    EXPECT_EQ(cc.x(), 25);
    EXPECT_EQ(cc.y(), 40);
    EXPECT_EQ(rc.x(), r2.right());
    EXPECT_EQ(rc.y(), 40);

    EXPECT_EQ(lb.x(), r2.left());
    EXPECT_EQ(lb.y(), r2.bottom());
    EXPECT_EQ(cb.x(), 25);
    EXPECT_EQ(cb.y(), r2.bottom());
    EXPECT_EQ(rb.x(), r2.right());
    EXPECT_EQ(rb.y(), r2.bottom());

    Rect32 r3;
    r2.move(0,0);
    r3 = r2;
    lt = r2.leftTop();

    r2.move(1000,-2000);
    EXPECT_FALSE(r3 == r2);
    EXPECT_EQ(1000, r2.left());
    EXPECT_EQ(-2000, r2.top());
    EXPECT_EQ(r2.width(), 30);
    EXPECT_EQ(r2.height(), 40);

    r2.offset(-1000, 2000);
    EXPECT_TRUE(r3 == r2);

    r2 = r2 + Pos2i(1000,-2000);
    EXPECT_EQ(1000, r2.left());
    EXPECT_EQ(-2000, r2.top());
    EXPECT_EQ(r2.width(), 30);
    EXPECT_EQ(r2.height(), 40);

    r2 = r2 - Pos2i(1000,-2000);
    EXPECT_TRUE(r3 == r2);
    
    cp.zero();
    EXPECT_TRUE(cp.empty());
    EXPECT_FALSE(cp.valid());
    EXPECT_EQ(0, cp.left());
    EXPECT_EQ(0, cp.top());
    EXPECT_EQ(-1, cp.right());
    EXPECT_EQ(-1, cp.bottom());
    EXPECT_EQ(0, cp.width());
    EXPECT_EQ(0, cp.height());

    r3.inflate(0,0);
    EXPECT_FALSE(r3);
    EXPECT_TRUE(r3.empty());
    r3.inflate(123,456);
    EXPECT_TRUE(r3);
    EXPECT_FALSE(r3.empty());
    EXPECT_EQ(123, r3.width());
    r3.inflate(123,456);
    EXPECT_EQ(456, r3.height());
    EXPECT_EQ(122, r3.right());
    EXPECT_EQ(455, r3.bottom());
    
    // overflow x,y,xy
    constexpr Rect32 of_rx(1, 0, int32_max, int32_max);
    EXPECT_FALSE(of_rx);
    EXPECT_FALSE(of_rx.empty());
    
    constexpr Rect32 of_ry(0, 1, int32_max, int32_max);
    EXPECT_FALSE(of_ry);
    EXPECT_FALSE(of_ry.empty());

    constexpr Rect32 of_rxy(1, 1, int32_max, int32_max);
    EXPECT_FALSE(of_rxy);
    EXPECT_FALSE(of_rxy.empty());
}

TEST(Shape2d, Rect32_method)
{
    constexpr Rect32 r(-100,-200, 300, 400); // (-100,-200) ~ (199,199) 
    // zero(invalid)
    constexpr Rect32 zero;
    // boundary
    constexpr Rect32 b1( 150,  150, 50, 50);
    constexpr Rect32 b2(-100,  150, 50, 50);
    constexpr Rect32 b3(-100, -200, 50, 50);
    constexpr Rect32 b4( 150, -200, 50, 50);
    // inner
    constexpr Rect32 in1(150,  150, 25, 25);
    constexpr Rect32 in2(-50,  150, 25, 25);
    constexpr Rect32 in3(-50, -150, 25, 25);
    constexpr Rect32 in4(150, -150, 25, 25);
    constexpr Rect32 inc(-50,-50, 100, 100);
    // overlap
    constexpr Rect32 ov1( 150, 150, 100, 100);
    constexpr Rect32 ov2(-150, 150, 100, 100);
    constexpr Rect32 ov3(-150,-250, 100, 100);
    constexpr Rect32 ov4( 150,-250, 100, 100);
    constexpr Rect32 ovo(-150, -250, 500, 500);
    // outer
    constexpr Rect32 out1( 250, 250, 10, 10);
    constexpr Rect32 out2(-250, 250, 10, 10);
    constexpr Rect32 out3(-250,-250, 10, 10);
    constexpr Rect32 out4( 250,-250, 10, 10);

    
    // Contains point.
    constexpr auto result0 = r.contains(0,0);
    EXPECT_TRUE(result0);
    EXPECT_FALSE(r.contains(-101,-201));
    EXPECT_TRUE(r.contains(-100,-200));
    EXPECT_TRUE(r.contains(199,199));
    EXPECT_FALSE(r.contains(200, 200));
    
    // Contains rectangle.
    constexpr bool result1 = r.contains(b1);
    EXPECT_TRUE(result1);
    EXPECT_TRUE(r.contains(b2));
    EXPECT_TRUE(r.contains(b3));
    EXPECT_TRUE(r.contains(b4));

    EXPECT_TRUE(r.contains(in1));
    EXPECT_TRUE(r.contains(in2));
    EXPECT_TRUE(r.contains(in3));
    EXPECT_TRUE(r.contains(in4));
    EXPECT_TRUE(r.contains(inc));

    EXPECT_FALSE(r.contains(ov1));
    EXPECT_FALSE(r.contains(ov2));
    EXPECT_FALSE(r.contains(ov3));
    EXPECT_FALSE(r.contains(ov4));
    EXPECT_FALSE(r.contains(ovo));

    EXPECT_FALSE(r.contains(out1));
    EXPECT_FALSE(r.contains(out2));
    EXPECT_FALSE(r.contains(out3));
    EXPECT_FALSE(r.contains(out4));
    
    EXPECT_FALSE(zero.contains(zero));
    EXPECT_FALSE(zero.contains(b1));
    EXPECT_FALSE(b2.contains(zero));

    // Overlaps rectangle
    constexpr bool result2 = r.overlaps(b1);
    EXPECT_TRUE(result2);
    EXPECT_TRUE(r.overlaps(b2));
    EXPECT_TRUE(r.overlaps(b3));
    EXPECT_TRUE(r.overlaps(b4));

    EXPECT_TRUE(r.overlaps(in1));
    EXPECT_TRUE(r.overlaps(in2));
    EXPECT_TRUE(r.overlaps(in3));
    EXPECT_TRUE(r.overlaps(in4));
    EXPECT_TRUE(r.overlaps(inc));

    EXPECT_TRUE(r.overlaps(ov1));
    EXPECT_TRUE(r.overlaps(ov2));
    EXPECT_TRUE(r.overlaps(ov3));
    EXPECT_TRUE(r.overlaps(ov4));
    EXPECT_TRUE(r.overlaps(ovo));

    EXPECT_FALSE(r.overlaps(out1));
    EXPECT_FALSE(r.overlaps(out2));
    EXPECT_FALSE(r.overlaps(out3));
    EXPECT_FALSE(r.overlaps(out4));
    
    EXPECT_FALSE(zero.overlaps(zero));
    EXPECT_FALSE(zero.overlaps(b1));
    EXPECT_FALSE(b2.overlaps(zero));
}

TEST(Shape2d, Rect32_union)
{
    constexpr Rect32 r(-100,-200, 300, 400); // (-100,-200) ~ (200,200)
    // zero(invalid)
    constexpr Rect32 zero;
    // boundary
    constexpr Rect32 b1( 150,  150, 50, 50);
    constexpr Rect32 b2(-100,  150, 50, 50);
    constexpr Rect32 b3(-100, -200, 50, 50);
    constexpr Rect32 b4( 150, -200, 50, 50);
    // inner
    constexpr Rect32 in1(150,  150, 25, 25);
    constexpr Rect32 in2(-50,  150, 25, 25);
    constexpr Rect32 in3(-50, -150, 25, 25);
    constexpr Rect32 in4(150, -150, 25, 25);
    constexpr Rect32 inc(-50,-50, 100, 100);
    // overlap
    constexpr Rect32 ov1( 150, 150, 100, 100);
    constexpr Rect32 ov2(-150, 150, 100, 100);
    constexpr Rect32 ov3(-150,-250, 100, 100);
    constexpr Rect32 ov4( 150,-250, 100, 100);
    constexpr Rect32 ovo(-150, -250, 500, 500);
    // outer
    constexpr Rect32 out1( 250, 250, 10, 10);
    constexpr Rect32 out2(-250, 250, 10, 10);
    constexpr Rect32 out3(-250,-250, 10, 10);
    constexpr Rect32 out4( 250,-250, 10, 10);

    Rect32 rr = r;

    auto rz = rr | zero;
    EXPECT_EQ(rz, rr);
    auto rz2 = zero | rr;
    EXPECT_EQ(rz2, rr);

    auto rb1 = rr | b1;
    auto rb2 = b2 | rr;
    auto rb3 = rr | b3;
    auto rb4 = b4 | rr;
    
    EXPECT_EQ(rb1, rr);
    EXPECT_EQ(rb2, rr);
    EXPECT_EQ(rb3, rr);
    EXPECT_EQ(rb4, rr);

    auto ri1 = rr | in1;
    auto ri2 = in2 | rr;
    auto ri3 = rr | in3;
    auto ri4 = in4 | rr;
    auto ric = rr | inc;

    EXPECT_EQ(ri1, rr);
    EXPECT_EQ(ri2, rr);
    EXPECT_EQ(ri3, rr);
    EXPECT_EQ(ri4, rr);
    EXPECT_EQ(ric, rr);
    
    auto rov1 = r | ov1;
    auto rov2 = ov2 | r;
    auto rov3 = r | ov3;
    auto rov4 = ov4 | r;
    auto rovo = r | ovo;

    EXPECT_EQ(-100, rov1.left());
    EXPECT_EQ(-200, rov1.top());
    EXPECT_EQ( 249, rov1.right());
    EXPECT_EQ( 249, rov1.bottom());
    EXPECT_EQ( 350, rov1.width());
    EXPECT_EQ( 450, rov1.height());
    
    EXPECT_EQ(-150, rov2.left());
    EXPECT_EQ(-200, rov2.top());
    EXPECT_EQ( 199, rov2.right());
    EXPECT_EQ( 249, rov2.bottom());
    EXPECT_EQ( 350, rov2.width());
    EXPECT_EQ( 450, rov2.height());
    
    EXPECT_EQ(-150, rov3.left());
    EXPECT_EQ(-250, rov3.top());
    EXPECT_EQ( 199, rov3.right());
    EXPECT_EQ( 199, rov3.bottom());
    EXPECT_EQ( 350, rov3.width());
    EXPECT_EQ( 450, rov3.height());
    
    EXPECT_EQ(-100, rov4.left());
    EXPECT_EQ(-250, rov4.top());
    EXPECT_EQ( 249, rov4.right());
    EXPECT_EQ( 199, rov4.bottom());
    EXPECT_EQ( 350, rov4.width());
    EXPECT_EQ( 450, rov4.height());

    EXPECT_EQ(rovo, ovo);

    auto rou1 = rr | out1;
    auto rou2 = out2 | rr;
    auto rou3 = rr | out3;;
    auto rou4 = out4 | rr;

    EXPECT_EQ(-100, rou1.left());
    EXPECT_EQ(-200, rou1.top());
    EXPECT_EQ( 259, rou1.right());
    EXPECT_EQ( 259, rou1.bottom());
    EXPECT_EQ( 360, rou1.width());
    EXPECT_EQ( 460, rou1.height());
    
    EXPECT_EQ(-250, rou2.left());
    EXPECT_EQ(-200, rou2.top());
    EXPECT_EQ( 199, rou2.right());
    EXPECT_EQ( 259, rou2.bottom());
    EXPECT_EQ( 450, rou2.width());
    EXPECT_EQ( 460, rou2.height());
    
    EXPECT_EQ(-250, rou3.left());
    EXPECT_EQ(-250, rou3.top());
    EXPECT_EQ( 199, rou3.right());
    EXPECT_EQ( 199, rou3.bottom());
    EXPECT_EQ( 450, rou3.width());
    EXPECT_EQ( 450, rou3.height());
    
    EXPECT_EQ(-100, rou4.left());
    EXPECT_EQ(-250, rou4.top());
    EXPECT_EQ( 259, rou4.right());
    EXPECT_EQ( 199, rou4.bottom());
    EXPECT_EQ( 360, rou4.width());
    EXPECT_EQ( 450, rou4.height());
}


TEST(Shape2d, Rect32_intersect)
{
    constexpr Rect32 r(-100,-200, 300, 400); // (-100,-200) ~ (200,200)
    // zero(invalid)
    constexpr Rect32 zero;
    // boundary
    constexpr Rect32 b1( 150,  150, 50, 50);
    constexpr Rect32 b2(-100,  150, 50, 50);
    constexpr Rect32 b3(-100, -200, 50, 50);
    constexpr Rect32 b4( 150, -200, 50, 50);
    // inner
    constexpr Rect32 in1(150,  150, 25, 25);
    constexpr Rect32 in2(-50,  150, 25, 25);
    constexpr Rect32 in3(-50, -150, 25, 25);
    constexpr Rect32 in4(150, -150, 25, 25);
    constexpr Rect32 inc(-50,-50, 100, 100);
    // overlap
    constexpr Rect32 ov1( 150, 150, 100, 100);
    constexpr Rect32 ov2(-150, 150, 100, 100);
    constexpr Rect32 ov3(-150,-250, 100, 100);
    constexpr Rect32 ov4( 150,-250, 100, 100);
    constexpr Rect32 ovo(-150, -250, 500, 500);
    // outer
    constexpr Rect32 out1( 250, 250, 10, 10);
    constexpr Rect32 out2(-250, 250, 10, 10);
    constexpr Rect32 out3(-250,-250, 10, 10);
    constexpr Rect32 out4( 250,-250, 10, 10);

    Rect32 rr = r;

    auto rz = rr & zero;
    EXPECT_TRUE(rz.empty());
    EXPECT_FALSE(rz.valid());
    auto rz2 = zero & r;
    EXPECT_TRUE(rz2.empty());
    EXPECT_FALSE(rz2.valid());

    auto rb1 = rr & b1;
    auto rb2 = b2 & rr;
    auto rb3 = rr & b3;
    auto rb4 = b4 & rr;
    
    EXPECT_TRUE(rb1 == b1);
    EXPECT_TRUE(rb2 == b2);
    EXPECT_TRUE(rb3 == b3);
    EXPECT_TRUE(rb4 == b4);

    auto ri1 = rr & in1;
    auto ri2 = in2 & rr;
    auto ri3 = rr & in3;
    auto ri4 = in4 & rr;
    auto ric = rr & inc;

    EXPECT_TRUE(ri1 == in1);
    EXPECT_TRUE(ri2 == in2);
    EXPECT_TRUE(ri3 == in3);
    EXPECT_TRUE(ri4 == in4);
    EXPECT_TRUE(ric == inc);

    auto rov1 = r & ov1;
    auto rov2 = ov2 & r;
    auto rov3 = r & ov3;
    auto rov4 = ov4 & r;
    auto rovo = r & ovo;

    EXPECT_EQ( 150, rov1.left());
    EXPECT_EQ( 150, rov1.top());
    EXPECT_EQ( 199, rov1.right());
    EXPECT_EQ( 199, rov1.bottom());
    EXPECT_EQ( 50, rov1.width());
    EXPECT_EQ( 50, rov1.height());

    EXPECT_EQ(-100, rov2.left());
    EXPECT_EQ( 150, rov2.top());
    EXPECT_EQ( -51, rov2.right());
    EXPECT_EQ( 199, rov2.bottom());
    EXPECT_EQ(  50, rov2.width());
    EXPECT_EQ(  50, rov2.height());

    EXPECT_EQ(-100, rov3.left());
    EXPECT_EQ(-200, rov3.top());
    EXPECT_EQ( -51, rov3.right());
    EXPECT_EQ(-151, rov3.bottom());
    EXPECT_EQ(  50, rov3.width());
    EXPECT_EQ(  50, rov3.height());

    EXPECT_EQ( 150, rov4.left());
    EXPECT_EQ(-200, rov4.top());
    EXPECT_EQ( 199, rov4.right());
    EXPECT_EQ(-151, rov4.bottom());
    EXPECT_EQ(  50, rov4.width());
    EXPECT_EQ(  50, rov4.height());

    EXPECT_TRUE(r == rovo);

    auto rou1 = rr & out1;
    auto rou2 = out2 & rr;
    auto rou3 = rr & out3;;
    auto rou4 = out4 & rr;

    EXPECT_TRUE(rou1.empty());
    EXPECT_TRUE(rou2.empty());
    EXPECT_TRUE(rou3.empty());
    EXPECT_TRUE(rou4.empty());
}

