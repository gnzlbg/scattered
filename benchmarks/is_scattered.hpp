// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_IS_SCATTERED_HPP)
#define SCATTERED_BENCHMARK_IS_SCATTERED_HPP

#include "scattered/detail/unqualified.hpp"

template <typename O> class is_scattered {
  using T = scattered::detail::unqualified_t<O>;

  template <typename C> static auto test(C&&) -> decltype(typename C::scattered{}, std::true_type{});
  template <typename C> static auto test(...) -> std::false_type;

public:
  using type = decltype(test<T>(std::declval<T>()));
  static const bool value = type::value;
};

template<class T, class R = void> using enable_if_scattered = std::enable_if_t<is_scattered<T>::value, R>;
template<class T, class R = void> using disable_if_scattered = std::enable_if_t<!is_scattered<T>::value, R>;

#endif  // SCATTERED_BENCHMARK_IS_SCATTERED_HPP
