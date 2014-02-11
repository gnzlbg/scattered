// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_ACCESS_PATTERNS_HPP)
#define SCATTERED_BENCHMARK_ACCESS_PATTERNS_HPP

////////////////////////////////////////////////////////////////////////////////

struct sequential {
  template<class C, class F>
  [[gnu::flatten, gnu::hot]] inline void operator()(C&& container, F&& operation) const {
    for (auto&& i : container) {
      operation(i);
    }
  }
};

auto name(sequential) RETURNS(std::string{"sequential"});

////////////////////////////////////////////////////////////////////////////////

template<std::size_t stride> struct strided {
  template<class C, class F>
  [[gnu::flatten, gnu::hot]] inline void operator()(C&& container, F&& operation) const {
    for (std::size_t i = 0, e = container.size(); i < e; i += stride) {
      operation(container[i]);
    }
  }
};

template<std::size_t stride>
auto name(strided<stride>) RETURNS("strided_" + std::to_string(stride));

////////////////////////////////////////////////////////////////////////////////

using access_patterns = boost::mpl::vector<sequential
                                           // , strided<2>
                                           // , strided<4>
                                           >;

#endif  // SCATTERED_BENCHMARK_ACCESS_PATTERNS_HPP
