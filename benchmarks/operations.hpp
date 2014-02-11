// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_OPERATIONS_HPP)
#define SCATTERED_BENCHMARK_OPERATIONS_HPP

#include "types.hpp"
#include "is_fusion_pair.hpp"
#include "is_scattered.hpp"
#include "scattered/detail/unqualified.hpp"

////////////////////////////////////////////////////////////////////////////////

struct multiply_by_itself_all {
  struct impl {
    template<class T>
    [[gnu::always_inline]] inline enable_if_fusion_pair<T> operator()(T&& i) const {
      i.second *= i.second;
    }
    template<class T>
    [[gnu::always_inline]] inline disable_if_fusion_pair<T> operator()(T&& i) const {
      i *= i;
    }
  };

  template<class T>
  [[gnu::always_inline]] inline void operator()(T&& o) const {
    boost::fusion::for_each(o, impl{});
  }
};

auto name(multiply_by_itself_all) RETURNS(std::string{"multiply_by_itself_all"});

////////////////////////////////////////////////////////////////////////////////

struct multiply_by_itself_one {
  template<class T>
  [[gnu::always_inline]] inline disable_if_scattered<T> operator()(T&& o) const { o.d0 *= o.d0; }
  template<class T>
  [[gnu::always_inline]] inline enable_if_scattered<T> operator()(T&& o) const {
    scattered::get<k::d0>(o) *= scattered::get<k::d0>(o);
  }
};

auto name(multiply_by_itself_one) RETURNS(std::string{"multiply_by_itself_one"});

////////////////////////////////////////////////////////////////////////////////

using operations = boost::mpl::vector<
  //multiply_by_itself_all,
  multiply_by_itself_one
                                      >;

#endif  // SCATTERED_BENCHMARK_OPERATIONS_HPP
