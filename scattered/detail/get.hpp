// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief get element at key

#if !defined(SCATTERED_DETAIL_GET_HPP)
#define SCATTERED_DETAIL_GET_HPP

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include "returns.hpp"
#include "unqualified.hpp"

namespace scattered {

namespace get_detail {

struct is_iterator_test {
  template <class U>
  static auto test(U* p) -> decltype(p -> it(), std::true_type());
  template <class> static auto test(...) -> std::false_type;
};

template <class T>
struct is_iterator
    : decltype(is_iterator_test::test<detail::unqualified_t<T>>(0)) {};

struct is_adapted_iterator_test {
  template <class U>
  static auto test(U* p) -> decltype(p -> base(), std::true_type());
  template <class> static auto test(...) -> std::false_type;
};

template <class T>
struct is_adapted_iterator
    : decltype(is_adapted_iterator_test::test<detail::unqualified_t<T>>(0)) {};

}  // namespace get_detail

/// \brief Get element at key
template <class K, class C,
          std::enable_if_t<!get_detail::is_iterator<C>::value
                           && !get_detail::is_adapted_iterator<C>::value,
                           int> = 0>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]] inline
auto get(C&& c) RETURNS(boost::fusion::at_key<K>(std::forward<C>(c)));

/// \brief Get element at key for an iterator
template <class K, class C,
          std::enable_if_t<get_detail::is_iterator<C>::value, int> = 0>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]] inline
auto get(C&& c) noexcept {
  return get<K>(std::forward<decltype(c.it())>(c.it()));
}

/// \brief Get element at key for an adapted iterator
template <class K, class C,
          std::enable_if_t<get_detail::is_adapted_iterator<C>::value, int> = 0>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]] inline
auto get(C&& c) {
  return get<K>(std::forward<decltype(c.base())>(c.base()));
}

}  // namespace scattered

#endif  // SCATTERED_DETAIL_GET_HPP
