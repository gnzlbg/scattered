// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Assert macro

#if !defined(SCATTERED_DETAIL_ASSERT_HPP)
#define SCATTERED_DETAIL_ASSERT_HPP

#include <cassert>
#include <stdio.h>

/// \brief Asserts a condition in DEBUG mode. Useful for checking
/// pre/postconditions and invariants.
///
/// \warning if NDEBUG is defined the condition in the if clause is always
/// false and ASSERT compiles to nothing! Still this happens after the
/// compiler has verified the correctness of the ASSERT code.
#ifndef NDEBUG
#define ASSERT(condition, message)                                      \
  do {                                                                  \
    if (!(condition)) {                                                 \
      fprintf(stderr, "\nAssertion %s failed in function: %s, in file: %s, at line: " \
              "%i.\nDiagnostic message: %s\n\n",                        \
              "" #condition "", __func__, __FILE__, __LINE__, message); \
      assert(false);                                                    \
    }                                                                   \
  } while (false)
#else
#define ASSERT(condition, message)                                      \
  do {                                                                  \
    if (0 && (condition)) {                                             \
      fprintf(stderr, "\nAssertion %s failed in function: %s, in file: %s, at line: " \
              "%i.\nDiagnostic message: %s\n\n",                        \
              "" #condition "", __func__, __FILE__, __LINE__, message); \
      assert(true);                                                     \
    }                                                                   \
  } while (false)
#endif  // NDEBUG

#endif  // SCATTERED_DETAIL_ASSERT_HPP
