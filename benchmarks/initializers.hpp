// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_INITIALIZER_HPP)
#define SCATTERED_BENCHMARK_INITIALIZER_HPP

#include <random>
#include "scattered/detail/unqualified.hpp"
#include "is_scattered.hpp"

////////////////////////////////////////////////////////////////////////////////

struct fill_with_random_values {
  fill_with_random_values(int s = 1) { rng.seed(s); v = dist(rng); }
  using Engine = std::mt19937;
  using Dist = std::uniform_int_distribution<uint32_t>;
  Engine rng;
  Dist dist;
  uint v;

  template<class C>
  enable_if_scattered<C> operator()(C&& c) {
    for (auto&& i : c) {
      boost::fusion::for_each(i, [&](auto& o) {
          o.second = v;
        });
    }
  }

  template<class C>
  disable_if_scattered<C> operator()(C&& c) {
    for (auto&& i : c) {
      boost::fusion::for_each(i, [&](auto& o) {
          o = v;
      });
    }
  }


};

auto name(fill_with_random_values)
RETURNS(std::string{"fill_with_random_values"});

////////////////////////////////////////////////////////////////////////////////

#endif  // SCATTERED_BENCHMARK_INITIALIZER_HPP
