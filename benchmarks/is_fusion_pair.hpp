// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_IS_FUSION_PAIR_HPP)
#define SCATTERED_BENCHMARK_IS_FUSION_PAIR_HPP

#include "scattered/detail/unqualified.hpp"

template<class T> struct is_fusion_pair_impl_ {
  static const bool value = false;
};

template<class T, class U> struct is_fusion_pair_impl_<boost::fusion::pair<T, U>> {
  static const bool value = true;
};

template<class T> struct is_fusion_pair {
  static const bool value = is_fusion_pair_impl_<scattered::detail::unqualified_t<T>>::value;
};

template<class T> using enable_if_fusion_pair = std::enable_if_t<is_fusion_pair<T>::value, void>;
template<class T> using disable_if_fusion_pair = std::enable_if_t<!is_fusion_pair<T>::value, void>;

#endif  // SCATTERED_BENCHMARK_IS_FUSION_PAIR_HPP
