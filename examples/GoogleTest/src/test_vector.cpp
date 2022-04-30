/*
  Unittest for Vector2<float>
*/
#include <gob_vector2d.hpp>
#include <vector>
#include <limits>
#include "gtest/gtest.h"
#include "helper_constants.hpp"
using namespace test::constants;

using Vec2f = goblib::shape2d::Vector2<float>;

namespace
{
std::vector<Vec2f> create4(float value)
{
    std::vector<Vec2f> vec;
    vec.push_back( Vec2f(0.0f, 0.0f) );
    vec.push_back( Vec2f(value, 0.0f) );
    vec.push_back( Vec2f(0.0f, value) );
    vec.push_back( Vec2f(value, value) );
    return vec;
}

GOBLIB_MAYBE_UNUSED void print_v(const Vec2f& v, const char* tag = "")
{
    printf("%s:(%f,%f)\n", tag, v.x(), v.y());
}

#define PRINTV(v) do { printv((v), #v); }while(0)
//
}

TEST(Vector2, Basic)
{
    constexpr Vec2f v1 = Vec2f(1.0f, 2.0f);
    constexpr Vec2f v2(-2.0f, -1.0f);
    constexpr Vec2f v3 = { 3333.f, -0.3333f };
    constexpr Vec2f v4 = Vec2f() + Vec2f(44.0f, -0.44f);
    constexpr Vec2f v5 = Vec2f{ 500, -5 };
    constexpr Vec2f vz;
    constexpr Vec2f v1n(-1.0f,-2.0f);
    constexpr Vec2f vc(goblib::shape2d::Vector2<double>(-1234.567, 9876.543));
    
    EXPECT_FLOAT_EQ(v1.x(), 1.0f);
    EXPECT_FLOAT_EQ(v1.y(), 2.0f);
    EXPECT_FLOAT_EQ(v2.x(), -2.0f);
    EXPECT_FLOAT_EQ(v2.y(), -1.0f);
    EXPECT_FLOAT_EQ(v3.x(), 3333.f);
    EXPECT_FLOAT_EQ(v3.y(), -0.3333f);
    EXPECT_FLOAT_EQ(v4.x(), 44.0f);
    EXPECT_FLOAT_EQ(v4.y(), -0.44f);
    EXPECT_FLOAT_EQ(v5.x(), 500.0f);
    EXPECT_FLOAT_EQ(v5.y(), -5.0f);
    EXPECT_FLOAT_EQ(vz.x(), 0.0f);
    EXPECT_FLOAT_EQ(vz.y(), 0.0f);
    EXPECT_FLOAT_EQ(vc.x(), -1234.567f);
    EXPECT_FLOAT_EQ(vc.y(), 9876.543f);

    auto a = -v1;
    EXPECT_FLOAT_EQ(a.x(), -1.0f);
    EXPECT_FLOAT_EQ(a.y(), -2.0f);
    a = v1 + v2;
    auto b = v1;
    b += v2;
    EXPECT_FLOAT_EQ(a.x(), -1.0f);
    EXPECT_FLOAT_EQ(a.y(), 1.0f);
    EXPECT_TRUE(a==b);
    a = v1 - v2;
    b = v1;
    b -= v2;
    EXPECT_FLOAT_EQ(a.x(), 3.0f);
    EXPECT_FLOAT_EQ(a.y(), 3.0f);
    EXPECT_TRUE(a==b);
    a = v1 * 3.5f;
    b = v1;
    b *= 3.5f;
    EXPECT_FLOAT_EQ(a.x(), 3.5f);
    EXPECT_FLOAT_EQ(a.y(), 7.0f);
    EXPECT_TRUE(a==b);
    a = 3.5f * v1;
    EXPECT_FLOAT_EQ(a.x(), 3.5f);
    EXPECT_FLOAT_EQ(a.y(), 7.0f);
    EXPECT_TRUE(a==b);
    a = v1 * v3;
    b = v1;
    b *= v3;
    EXPECT_FLOAT_EQ(a.x(), 3333.0f);
    EXPECT_FLOAT_EQ(a.y(), -0.6666f);
    EXPECT_TRUE(a==b);
    a = v3 / 10.0f;
    b = v3;
    b /= 10.0f;
    EXPECT_FLOAT_EQ(a.x(), 333.3f);
    EXPECT_FLOAT_EQ(a.y(), -0.03333f);
    EXPECT_TRUE(a==b);
    a = v4 / v2;
    b = v4;
    b /= v2;
    EXPECT_FLOAT_EQ(a.x(), -22.0f);
    EXPECT_FLOAT_EQ(a.y(), 0.44f);

    constexpr auto x = v1.x();
    constexpr auto y = v1.y();
    constexpr auto neg = -v1;
    constexpr auto isvalid = v1.valid();
    constexpr auto lensq = v1.lengthSq();
    constexpr auto angle = v1.angle();
    constexpr auto cross = v1.cross(v2);
    constexpr auto dot = v1.dot(v1);
   
    EXPECT_FLOAT_EQ(x, 1.0f);
    EXPECT_FLOAT_EQ(y, 2.0f);
    EXPECT_TRUE(neg == v1n);
    EXPECT_TRUE(isvalid);
    EXPECT_FLOAT_EQ(lensq, 5.0f);
    EXPECT_FLOAT_EQ(lensq, dot);
    EXPECT_FLOAT_EQ(angle, 1.1071487177f);
    EXPECT_FLOAT_EQ(cross, 3.0f);
}

TEST(Vector2, infinity)
{
    auto vec = create4(F_INF);

    EXPECT_FALSE(vec[1].valid());
    EXPECT_FALSE(vec[2].valid());
    EXPECT_FALSE(vec[3].valid());

    EXPECT_FALSE(vec[1] == vec[0]);
    EXPECT_FALSE(vec[2] == vec[0]);
    EXPECT_FALSE(vec[3] == vec[0]);
    
    EXPECT_FALSE(vec[1] == vec[1]);
    EXPECT_FALSE(vec[2] == vec[2]);
    EXPECT_FALSE(vec[3] == vec[3]);

    EXPECT_FALSE(vec[1] == vec[2]);
    EXPECT_FALSE(vec[1] == vec[3]);
    EXPECT_FALSE(vec[2] == vec[3]);
}


TEST(Vector2, negative_infinity)
{
    auto vec = create4(F_NINF);

    EXPECT_FALSE(vec[1].valid());
    EXPECT_FALSE(vec[2].valid());
    EXPECT_FALSE(vec[3].valid());

    EXPECT_FALSE(vec[1] == vec[0]);
    EXPECT_FALSE(vec[2] == vec[0]);
    EXPECT_FALSE(vec[3] == vec[0]);
    
    EXPECT_FALSE(vec[1] == vec[1]);
    EXPECT_FALSE(vec[2] == vec[2]);
    EXPECT_FALSE(vec[3] == vec[3]);

    EXPECT_FALSE(vec[1] == vec[2]);
    EXPECT_FALSE(vec[1] == vec[3]);
    EXPECT_FALSE(vec[2] == vec[3]);
}


TEST(Vector2, nan)
{
    auto vec = create4(F_NAN);

    EXPECT_FALSE(vec[1].valid());
    EXPECT_FALSE(vec[2].valid());
    EXPECT_FALSE(vec[3].valid());

    EXPECT_FALSE(vec[1] == vec[0]);
    EXPECT_FALSE(vec[2] == vec[0]);
    EXPECT_FALSE(vec[3] == vec[0]);
    
    EXPECT_FALSE(vec[1] == vec[1]);
    EXPECT_FALSE(vec[2] == vec[2]);
    EXPECT_FALSE(vec[3] == vec[3]);

    EXPECT_FALSE(vec[1] == vec[2]);
    EXPECT_FALSE(vec[1] == vec[3]);
    EXPECT_FALSE(vec[2] == vec[3]);
}


TEST(Vector2, calc)
{
    const Vec2f v1(123.0f, -456.0f);
    const Vec2f v2(456.0f, 123.0f);
    const Vec2f v3(123.0f * 2, -456.0f * 2);  // 246, -912
    const Vec2f v4(456.0f/2.5f, 123.0f/2.5f); // 182.4, 49.2
    Vec2f a,b;

    EXPECT_FLOAT_EQ(v1.length(), 472.297575f);
    EXPECT_FLOAT_EQ(v1.lengthSq(), 223065.f);
    EXPECT_FLOAT_EQ(v1.distance(v2), 667.929637f);
    EXPECT_FLOAT_EQ(v1.distanceSq(v2), 446130.f);
    EXPECT_FLOAT_EQ(v1.angle(), goblib::math::deg2rad(-74.904479972f));
    EXPECT_FLOAT_EQ(v1.angle(v2), goblib::math::deg2rad(90.f));

    a = v1;
    a.abs();
    b = v1.absV();
    EXPECT_FLOAT_EQ(a.x(), 123.0f);
    EXPECT_FLOAT_EQ(a.y(), 456.0f);
    EXPECT_TRUE(a==b);

    auto dot1 = v1.dot(v1);
    auto dot2 = v1.dot(v2);
    auto dot3 = v1.dot(v3);
    auto dot4 = v1.dot(v4);
    EXPECT_FLOAT_EQ(dot1, 223065.f);
    EXPECT_FLOAT_EQ(dot2, 0.0f);
    EXPECT_FLOAT_EQ(dot3, 446130.f);
    EXPECT_NEAR(dot4, 0.0f, 0.002f);
    EXPECT_FALSE(v1.perpendicular(v1));
    EXPECT_TRUE(v1.perpendicular(v2));
    EXPECT_FALSE(v1.perpendicular(v3));
    EXPECT_TRUE(v1.perpendicular(v4));

    auto cross1 = v1.cross(v1);
    auto cross2 = v1.cross(v2);
    auto cross3 = v1.cross(v3);
    auto cross4 = v1.cross(v4);
    EXPECT_FLOAT_EQ(cross1, 0.0f);
    EXPECT_FLOAT_EQ(cross2, 223065.f);
    EXPECT_FLOAT_EQ(cross3, 0.0f);
    EXPECT_FLOAT_EQ(cross4, 89226.f);
    EXPECT_TRUE(v1.parallel(v1));
    EXPECT_FALSE(v1.parallel(v2));
    EXPECT_TRUE(v1.parallel(v3));
    EXPECT_FALSE(v1.parallel(v4));
    
    a = v1;
    a.normalize();
    b = v1.normalizeV();
    EXPECT_FLOAT_EQ(a.x(),  0.260429f);
    EXPECT_FLOAT_EQ(a.y(), -0.965493f);
    EXPECT_TRUE(a==b);

    a = v1;
    a.perpendicular();
    b = v1.perpendicularV();
    EXPECT_FLOAT_EQ(a.x(), 456.0f);
    EXPECT_FLOAT_EQ(a.y(), 123.0f);
    EXPECT_TRUE(a==b);
    
    a = v1;
    a.truncate(100.f);
    b = v1.truncateV(100.f);
    EXPECT_FLOAT_EQ(a.x(),  26.042906f);
    EXPECT_FLOAT_EQ(a.y(), -96.549294f);
    EXPECT_TRUE(a==b);

    a = v1;
    a.clamp(10.0f, 100.0f);
    b = v1.clampV(10.0f, 100.0f);
    EXPECT_FLOAT_EQ(a.x(),  26.042906f);
    EXPECT_FLOAT_EQ(a.y(), -96.549294f);
    EXPECT_TRUE(a==b);

    a = v1;
    a.clamp(1000.0f, 10000.0f);
    b = v1.clampV(1000.0f, 10000.0f);
    EXPECT_FLOAT_EQ(a.x(),  260.429f);
    EXPECT_FLOAT_EQ(a.y(), -965.493f);
    EXPECT_TRUE(a==b);
}

TEST(Vector2, calc2)
{
    const Vec2f v1(123.0f, -456.0f);
    const Vec2f v2(456.0f, 123.0f);
    const Vec2f v3(123.0f * 2, -456.0f * 2);  // 246, -912
    const Vec2f v4(456.0f/2.5f, 123.0f/2.5f); // 182.4, 49.2
    Vec2f a,b;
    
    auto pro1 = v1.projectionV(v1);
    auto pro2 = v1.projectionV(v2);
    auto pro3 = v1.projectionV(v3);
    auto pro4 = v1.projectionV(v4);
    auto prox = v1.projectionV(Vec2f::X_AXIS);
    auto proy = v1.projectionV(Vec2f::Y_AXIS);
    auto proxx = Vec2f::X_AXIS.projectionV(v1);
    auto proyy = Vec2f::Y_AXIS.projectionV(v1);
    EXPECT_TRUE(pro1 == v1);
    EXPECT_TRUE(pro2 == Vec2f::ZERO_VECTOR);
    EXPECT_TRUE(pro3 == v1);
    EXPECT_TRUE(pro4.near(Vec2f::ZERO_VECTOR));
    EXPECT_TRUE(prox == Vec2f(123.f, 0));
    EXPECT_TRUE(proy == Vec2f(0, -456.f));
    EXPECT_TRUE(proxx == Vec2f(0.067823283f, -0.2514424f));
    EXPECT_TRUE(proyy == Vec2f(-0.2514424f, 0.93217671f));

    auto rej1 = v1.rejectionV(v1);
    auto rej2 = v1.rejectionV(v2);
    auto rej3 = v1.rejectionV(v3);
    auto rej4 = v1.rejectionV(v4);
    auto rejx = v1.rejectionV(Vec2f::X_AXIS);
    auto rejy = v1.rejectionV(Vec2f::Y_AXIS);
    auto rejxx = Vec2f::X_AXIS.rejectionV(v1);
    auto rejyy = Vec2f::Y_AXIS.rejectionV(v1);
    EXPECT_TRUE(rej1 == Vec2f::ZERO_VECTOR);
    EXPECT_TRUE(rej2 == Vec2f(123.f, -456.f));
    EXPECT_TRUE(rej2.perpendicular(v2));
    EXPECT_TRUE(rej3 == Vec2f::ZERO_VECTOR);
    EXPECT_TRUE(rej4.near(Vec2f(123.f, -456.f)));
    EXPECT_TRUE(rej4.perpendicular(v4));
    EXPECT_TRUE(rejx == Vec2f(0, -456.0f));
    EXPECT_TRUE(rejx.perpendicular(Vec2f::X_AXIS));
    EXPECT_TRUE(rejy == Vec2f(123.f, 0));
    EXPECT_TRUE(rejy.perpendicular(Vec2f::Y_AXIS));
    EXPECT_TRUE(rejxx == Vec2f(0.93217671f, 0.2514424f));
    EXPECT_TRUE(rejxx.perpendicular(v1));
    EXPECT_TRUE(rejyy == Vec2f(0.2514424f, 0.067823283f));
    EXPECT_TRUE(rejyy.perpendicular(v1));

    auto refx = v1.reflectionV(Vec2f::X_AXIS);
    auto refy = v1.reflectionV(Vec2f::Y_AXIS);
    auto ref1 = v1.reflectionV(v1.normalizeV());
    auto ref2 = v1.reflectionV(v2.normalizeV());
    EXPECT_TRUE(refx == Vec2f(-123.f, -456.f));
    EXPECT_TRUE(refy == Vec2f( 123.f,  456.f));
    EXPECT_TRUE(ref1.near(Vec2f(-123.f,  456.f)));
    EXPECT_TRUE(ref2.near(Vec2f( 123.f, -456.f)));
}


