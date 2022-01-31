/*
  Unittest for gob_singleton
*/
#include <gob_singleton.hpp>
#include "gtest/gtest.h"
#include "helper.hpp"
#include <cstring>

// Simple singleton.
class Foo : public goblib::Singleton<Foo>
{
  public:
    virtual const char* whoami() const { return "Foo"; }
    virtual ~Foo(){}

    static std::int32_t count() { return _cntFoo; }

  protected:
    friend class goblib::Singleton<Foo>;
    Foo() : Singleton<Foo>() { ++_cntFoo; }
    static std::int32_t _cntFoo;
};
std::int32_t Foo::_cntFoo = 0;

// Inherit from simple singleton.
class Bar : public Foo, public goblib::Singleton<Bar>
{
  public:
    // Resolve ambiguities.
    using PointerType = std::unique_ptr<Bar>;
    using goblib::Singleton<Bar>::instance;
    using goblib::Singleton<Bar>::create;

    virtual const char* whoami() const override { return "Bar"; }
    virtual ~Bar(){}

    static std::int32_t count() { return _cntBar; }

  protected:
    friend class goblib::Singleton<Bar>;
    Bar() : Foo(), Singleton<Bar>() { ++_cntBar; }
    static std::int32_t _cntBar;
};
std::int32_t Bar::_cntBar = 0;


TEST(Singleton, Basic)
{
    (void)Foo::instance();
    (void)Foo::instance().whoami();
    
    auto bf = std::is_same<Foo&, decltype(Foo::instance()) >::value;
    EXPECT_TRUE(bf);

    (void)Bar::instance().whoami();
    auto bb = std::is_same<Bar&, decltype(Bar::instance()) >::value;
    EXPECT_TRUE(bb);

    EXPECT_TRUE(std::strcmp("Foo", Foo::instance().whoami()) == 0);
    EXPECT_TRUE(std::strcmp("Bar", Bar::instance().whoami()) == 0);

    EXPECT_EQ(2, Foo::count()); // Foo/Bar
    EXPECT_EQ(1, Bar::count()); // Bar

    /* Cannot compile! */
#if 0
    Foo f = Foo::instance();
    Foo f2 = Bar::instance();
    Bar b = Bar::instance();
#endif
}
