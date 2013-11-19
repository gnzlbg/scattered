// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Enables swap for fusion sequences

#if !defined(SCATTERED_DETAIL_FUSION_SWAP_HPP)
#define SCATTERED_DETAIL_FUSION_SWAP_HPP

#include <type_traits>

namespace boost {

namespace fusion {

/// \brief Swap function for fusion types
template <class T> inline void swap(T&& lhs, T&& rhs) noexcept {
  using std::swap;
  std::remove_reference_t<T> tmp = lhs;
  lhs = std::move(rhs);
  rhs = std::move(tmp);
}

}  // namespace fusion

}  // namespace boost

#endif
