// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_TIME_FUNCTION_HPP)
#define SCATTERED_BENCHMARK_TIME_FUNCTION_HPP

#include <chrono>

using Time      = std::chrono::nanoseconds;
using Clock     = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

template<class F>
auto time_fn(F&& f) {
  const auto start = Clock::now();
  f();
  const auto end = Clock::now();
  return std::chrono::duration_cast<Time>(end - start).count();
}

#endif  // SCATTERED_BENCHMARK_TIME_FUNCTION_HPP
