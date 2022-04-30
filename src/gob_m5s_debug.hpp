/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_debug.hpp
  @brief Debugging support for M5Stack
*/
#pragma once
#ifndef GOBLIB_M5S_DEBUG_HPP
#define GOBLIB_M5S_DEBUG_HPP

#include <cstdint>
#include <gob_macro.hpp>

namespace goblib { namespace m5s {

/// @cond
class ScopedLeakCheck
{
  public:
    ScopedLeakCheck(const char* str, const std::uint32_t line, bool abort = false);
    ~ScopedLeakCheck();

  private:
    const char* _str;
    std::uint32_t _size;
    const std::uint32_t _line;
    bool _abort;
};
/// @endcond

//
}}

/*! Detect memory leaks in scope. */
#define GOBLIB_SCOPED_LEAK_CHECK() goblib::m5s::ScopedLeakCheck GOBLIB_CONCAT(slc_, __LINE__)(__func__, __LINE__, false)
#define GOBLIB_SCOPED_LEAK_ABORT() goblib::m5s::ScopedLeakCheck GOBLIB_CONCAT(slc_, __LINE__)(__func__, __LINE__, true)

#else

#define GOBLIB_SCOPED_LEAK_CHECK() /* nop */
#define GOBLIB_SCOPED_LEAK_ABORT() /* nop */

#endif
