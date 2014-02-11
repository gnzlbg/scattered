// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_TYPES_HPP)
#define SCATTERED_BENCHMARK_TYPES_HPP

#include <boost/fusion/adapted/struct/adapt_assoc_struct.hpp>

////////////////////////////////////////////////////////////////////////////////

struct k {
struct d0 {}; struct d1 {}; struct d2 {}; struct d3 {};
struct d4 {}; struct d5 {}; struct d6 {}; struct d7 {};
struct d8 {}; struct d9 {}; struct d10 {}; struct d11 {};
struct d12 {}; struct d13 {}; struct d14 {}; struct d15 {};
struct d16 {}; struct d17 {}; struct d18 {}; struct d19 {};
struct d20 {}; struct d21 {}; struct d22 {}; struct d23 {};
struct d24 {}; struct d25 {}; struct d26 {}; struct d27 {};
struct d28 {}; struct d29 {}; struct d30 {}; struct d31 {};
};

////////////////////////////////////////////////////////////////////////////////

struct very_small_object {  // size <= 64bytes (8doubles, 16 32bit ints)
  double d0, d1, d2, d3;  // cache line: 1
};

auto name(very_small_object) RETURNS(std::string{"very_small_object"});

BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    very_small_object,
    (double, d0, k::d0)
    (double, d1, k::d1)
    (double, d2, k::d2)
    (double, d3, k::d3)
)

////////////////////////////////////////////////////////////////////////////////

struct small_object {  // size <= 64bytes (8doubles, 16 32bit ints)
  double d0, d1, d2, d3, d4, d5, d6, d7;  // cache line: 1
};

auto name(small_object) RETURNS(std::string{"small_object"});

BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    small_object,
    (double, d0, k::d0)
    (double, d1, k::d1)
    (double, d2, k::d2)
    (double, d3, k::d3)
    (double, d4, k::d4)
    (double, d5, k::d5)
    (double, d6, k::d6)
    (double, d7, k::d7)
)

////////////////////////////////////////////////////////////////////////////////

struct medium_object {  // 64bytes < size <= 128bytes
  double d0, d1, d2, d3, d4, d5, d6, d7;        // cache line: 1
  double d8, d9, d10, d11, d12, d13, d14, d15;  // cache line: 2
};

auto name(medium_object)     RETURNS(std::string{"medium_object"});

BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    medium_object,
    (double, d0, k::d0)
    (double, d1, k::d1)
    (double, d2, k::d2)
    (double, d3, k::d3)
    (double, d4, k::d4)
    (double, d5, k::d5)
    (double, d6, k::d6)
    (double, d7, k::d7)
    (double, d8, k::d8)
    (double, d9, k::d9)
    (double, d10, k::d10)
    (double, d11, k::d11)
    (double, d12, k::d12)
    (double, d13, k::d13)
    (double, d14, k::d14)
    (double, d15, k::d15)
)

////////////////////////////////////////////////////////////////////////////////

struct large_object {  // size > 128bytes
  double d0, d1, d2, d3, d4, d5, d6, d7;         // cache line: 1
  double d8, d9, d10, d11, d12, d13, d14, d15;   // cache line: 2
  double d16, d17, d18, d19, d20, d21, d22, d23; // cache line: 3
  double d24, d25, d26, d27, d28, d29, d30, d31; // cache line: 4
};

auto name(large_object)      RETURNS(std::string{"large_object"});

BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    large_object,
    (double, d0, k::d0)
    (double, d1, k::d1)
    (double, d2, k::d2)
    (double, d3, k::d3)
    (double, d4, k::d4)
    (double, d5, k::d5)
    (double, d6, k::d6)
    (double, d7, k::d7)
    (double, d8, k::d8)
    (double, d9, k::d9)
    (double, d10, k::d10)
    (double, d11, k::d11)
    (double, d12, k::d12)
    (double, d13, k::d13)
    (double, d14, k::d14)
    (double, d15, k::d15)
    (double, d16, k::d16)
    (double, d17, k::d17)
    (double, d18, k::d18)
    (double, d19, k::d19)
    (double, d20, k::d20)
    (double, d21, k::d21)
    (double, d22, k::d22)
    (double, d23, k::d23)
    (double, d24, k::d24)
    (double, d25, k::d25)
    (double, d26, k::d26)
    (double, d27, k::d27)
    (double, d28, k::d28)
    (double, d29, k::d29)
    (double, d30, k::d30)
    (double, d31, k::d31)
)


////////////////////////////////////////////////////////////////////////////////

using test_types = boost::mpl::vector<//very_small_object
  small_object
  //  , medium_object
  //  ,
  // large_object
                                     >;

#endif  // SCATTERED_BENCHMARK_TYPES_HPP
