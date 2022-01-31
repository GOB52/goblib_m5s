/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_random.hpp
  @brief  Custom random engine using arduino functions.
*/
#pragma once
#ifndef GOBLIB_M5S_RANDOM_HPP
#define GOBLIB_M5S_RANDOM_HPP

#include <limits> // std::numeric_limits

namespace goblib { namespace m5s {
/*!
  arduino_engine
  @brief Random number generator compatible with std random engines.
  @note You can use it with std distributions.
  @code
  arduino_engine ae;
  std::uniform_int_distribution<> dist(0, 9);
  auto v = dist(ae);
  std::uniform_real_distribution<> real_dist(0.0, 1.0);
  auto rv = real_dist(ae);
  @endcode
  @warning This is not copyable because arduino random/randomSeed changes global state.
  @attention ESP32 uses hardware RNG in random(), so seed is ignored.
*/
class arduino_engine
{
  public:
    using result_type = long;
    constexpr static result_type default_seed = 52;

    arduino_engine() : arduino_engine(default_seed) {}
    explicit arduino_engine(result_type s){ seed(s); }
    ~arduino_engine(){}

    /*! Re-initializes the internal state sequence to pseudo-random values. (See also Attention) 
      @attention seed zero is ignored in randomSeed.
    */
    void seed(result_type s = default_seed);

    /*! Returns a new random number. */
    result_type operator()();
    /*! Advances the internal state by z notches, as if operator() was called z times, but without generating any numbers in the process. */
    void discard(unsigned long long z);

    /*! Returns the minimum value potentially returned by member operator(). */
    constexpr static result_type min() { return 0; }
    /*! Returns the maximum value potentially returned by member operator(). */
    constexpr static result_type max() { return std::numeric_limits<result_type>::max(); }

  private:
    arduino_engine(const arduino_engine&) = delete;
    arduino_engine& operator=(const arduino_engine&) = delete;
};

//
}}
#endif
