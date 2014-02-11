// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Implements Scattered Pair Container

#if !defined(SCATTERED_DETAIL_TYPE_LIST_HPP)
#define SCATTERED_DETAIL_TYPE_LIST_HPP

#include "pair.hpp"

namespace scattered {

template<class H, class... Ts>
struct type_list {
  using type = H;
  using tail = Ts;
};

}  // namespace scattered

#endif  // SCATTEValueED_DETAIL_TYPE_LIST_HPP
