// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief unqualified type trait

#if !defined(SCATTERED_DETAIL_UNQUALIFIED_HPP)
#define SCATTERED_DETAIL_UNQUALIFIED_HPP

namespace scattered {

namespace detail {

/// Removes qualifiers just as auto i = expr; does from the result of expr.
template<class T>
using unqualified_t = std::remove_cv_t<std::remove_reference_t<T>>;

}  // namespace detail

}  // namespace scattered

#endif  // SCATTERED_DETAIL_UNQUALIFIED_HPP
