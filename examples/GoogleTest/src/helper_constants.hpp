#ifndef HELPER_CONSTANTS_HPP
#define HELPER_CONSTANTS_HPP

#include <cstdint>
#include <limits>

namespace test { namespace constants {

constexpr std::int32_t I32_MAX = std::numeric_limits<std::int32_t>::max();
constexpr std::int32_t I32_MIN = std::numeric_limits<std::int32_t>::min();

constexpr float F_MAX = std::numeric_limits<float>::max();
constexpr float F_MIN = std::numeric_limits<float>::lowest(); // min(float) returns minimum positive value of flaot.
constexpr float F_INF = std::numeric_limits<float>::infinity();
constexpr float F_NINF = -std::numeric_limits<float>::infinity();
constexpr float F_NAN = std::numeric_limits<float>::quiet_NaN();
constexpr float F_EPS = std::numeric_limits<float>::epsilon();

//
}}
#endif
