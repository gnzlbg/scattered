// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief get element at key

#if !defined(SCATTERED_GET_HPP)
#define SCATTERED_GET_HPP

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include "returns.hpp"

namespace scattered {

/// \brief Get element at key
template <class K, class C>
auto get(C&& c) RETURNS(boost::fusion::at_key<K>(c));
}

#endif  // SCATTERED_GET_HPP
