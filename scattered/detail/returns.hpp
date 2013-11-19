// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \brief Returns macro

#if !defined(SCATTERED_DETAIL_RETURNS_HPP)
#define SCATTERED_DETAIL_RETURNS_HPP

/// \brief RETURNS() is used to avoid writing boilerplate
/// "->decltype(x) { return x; }" phrases.
//
/// USAGE: auto function(<arguments>) RETURNS(<some-expression>);
///
/// Note: we end with a unique typedef so the function can be followed
/// by a semicolon.  If we omit the semicolon, editors get confused and
/// think we haven't completed the function declaration.
///
/// Author: Dave Abrahams, see
/// https://groups.google.com/forum/#!msg/boost-devel-archive/OzJ5Ft3pSnU\
/// /b_Ter9bgNqAJ .
///
/// \todo Eric Niebler discusses how to improve errors messages when combining
/// the RETURNS macro with SFINAE for expressions here:
/// https://groups.google.com/forum/#!topic/boost-developers-archive\
/// /Ipn1bF24STc%5B1-25-false%5D .
///
#define RETURNS(...)                                                \
  noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__)))) \
      ->decltype(__VA_ARGS__) {                                     \
    return (__VA_ARGS__);                                           \
  }                                                                 \
  using RETURNS_CAT(RETURNS_, __LINE__) = int
// Standard PP concatenation formula
#define RETURNS_CAT_0(x, y) x##y
#define RETURNS_CAT(x, y) RETURNS_CAT_0(x, y)

#endif  // SCATTERED_DETAIL_RETURNS_HPP
