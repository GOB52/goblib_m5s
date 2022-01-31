

#include <gob_m5s_mutex.hpp>
#include <gob_macro.hpp>
#include <mutex>
#include "gtest/gtest.h"


TEST(Mutex, Basic)
{
    goblib::m5s::Mutex mtx1;
    goblib::m5s::Mutex mtx2;

    EXPECT_TRUE(mtx1.native_handle() != nullptr);
    EXPECT_TRUE(mtx2.native_handle() != nullptr);

#if defined(GOBLIB_CPP17_OR_LATER)
    {
        std::scoped_lock lock(mtx1, mtx2);

        EXPECT_FALSE(mtx1.try_lock());
        EXPECT_FALSE(mtx2.try_lock());
    }
#else
    {
        std::lock_guard<decltype(mtx1)> lock1(mtx1);
        std::lock_guard<decltype(mtx2)> lock2(mtx2);
        EXPECT_FALSE(mtx1.try_lock());
        EXPECT_FALSE(mtx2.try_lock());

    }
#endif
    
    EXPECT_TRUE(mtx1.try_lock());
    EXPECT_TRUE(mtx2.try_lock());
    mtx1.unlock();
    mtx2.unlock();
}

TEST(Mutex, Recursive)
{
    goblib::m5s::RecursiveMutex mtx1;
    goblib::m5s::RecursiveMutex mtx2;

    EXPECT_TRUE(mtx1.native_handle() != nullptr);
    EXPECT_TRUE(mtx2.native_handle() != nullptr);

#if defined(GOBLIB_CPP17_OR_LATER)
    {
        std::scoped_lock lock(mtx1, mtx2);

        EXPECT_TRUE(mtx1.try_lock());
        EXPECT_TRUE(mtx2.try_lock());
        mtx1.unlock();
        mtx2.unlock();
    }
#else
    {
        std::lock_guard<decltype(mtx1)> lock1(mtx1);
        std::lock_guard<decltype(mtx2)> lock2(mtx2);
        EXPECT_TRUE(mtx1.try_lock());
        EXPECT_TRUE(mtx2.try_lock());
        mtx1.unlock();
        mtx2.unlock();
    }
#endif

    EXPECT_TRUE(mtx1.try_lock());
    EXPECT_TRUE(mtx2.try_lock());
    mtx1.unlock();
    mtx2.unlock();
}
