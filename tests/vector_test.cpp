#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/tuple/tuple.hpp>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "scattered/vector.hpp"
#include "test_types.hpp"

#define DEBUG_OUTPUT

/// Pretty print for debugging
///@{
#ifdef DEBUG_OUTPUT
template <class T>
void print_container(const std::string s, const scattered::vector<T>& vec) {
  std::cout << s << ":\n";
  using k = TestType::k;
  using scattered::get;
  for (auto i : vec) {
    std::cout << "(" << get<k::x>(i) << ", " << get<k::y>(i) << ", "
              << get<k::i>(i) << ", " << get<k::b>(i) << ")\n";
  }
}
/// Pretty print (for debugging)
template <class T>
void print_container(const std::string s, const std::vector<T>& vec) {
  std::cout << s << ":\n";
  using k = TestType::k;
  using scattered::get;
  for (auto i : vec) {
    std::cout << "(" << get<k::x>(i) << ", " << get<k::y>(i) << ", "
              << get<k::i>(i) << ", " << get<k::b>(i) << ")\n";
  }
}
#else
template <class T> void print_container(const std::string, T&&) {}
#endif
///@}

template <class T>
using container_t = scattered::vector<TestType>::container_type<T>;

/// \test scattered::vector tests
TEST_CASE("Test scattered::vector<T>", "[scattered][vector]") {
  using k = TestType::k;  // Lets import the keys, we'll need them often.
  using scattered::get;

  /// We can make a vector from our Struct type
  std::size_t ref_size = 10;
  scattered::vector<TestType> vec(ref_size);
  std::vector<TestType> ref(ref_size);

  /// Iterators/references/value_types are fusion::maps
  for (std::size_t i = 0; i != ref.size(); ++i) {
    get<k::x>(vec[i]) = static_cast<float>(i);
    get<k::y>(vec[i]) = static_cast<double>(i);
    get<k::i>(vec[i]) = i;
    get<k::b>(vec[i]) = i % 2 == 0;

    ref[i].x = static_cast<float>(i);
    ref[i].y = static_cast<double>(i);
    ref[i].i = i;
    ref[i].b = i % 2 == 0;
  }

  auto are_equal = [](auto vc, auto rf) {
    for (std::size_t i = 0; i != rf.size(); ++i) {
      REQUIRE(get<k::x>(vc[i]) == Approx(rf[i].x));
      REQUIRE(get<k::y>(vc[i]) == Approx(rf[i].y));
      REQUIRE(get<k::i>(vc[i]) == rf[i].i);
      REQUIRE(get<k::b>(vc[i]) == rf[i].b);
    }
  };

  SECTION("types and get<key>") {
    auto it = vec.begin();
    auto cit = vec.cbegin();
    scattered::vector<TestType>::value_type val = *it;
    const scattered::vector<TestType>::value_type cval = *cit;
    decltype(auto) reft = *it;
    decltype(auto) creft = *cit;

    static_assert(std::is_same
                  <decltype(it), scattered::vector<TestType>::iterator>::value,
                  "wrong iterator type");
    static_assert(
        std::is_same
        <decltype(cit), scattered::vector<TestType>::const_iterator>::value,
        "wrong const_iterator type");
    static_assert(
        std::is_same
        <decltype(val), scattered::vector<TestType>::value_type>::value,
        "wrong value_type");
    static_assert(
        std::is_same
        <decltype(cval), const scattered::vector<TestType>::value_type>::value,
        "wrong const value_type");
    static_assert(
        std::is_same
        <decltype(reft), scattered::vector<TestType>::reference>::value,
        "wrong reference type");
    static_assert(
        std::is_same
        <decltype(creft), scattered::vector<TestType>::const_reference>::value,
        "wrong const_reference type");

    decltype(auto) it_x = get<k::x>(it);
    decltype(auto) cit_x = get<k::x>(cit);
    decltype(auto) val_x = get<k::x>(val);
    decltype(auto) cval_x = get<k::x>(cval);
    decltype(auto) reft_x = get<k::x>(reft);
    decltype(auto) creft_x = get<k::x>(creft);

    static_assert(
        std::is_same
        <decltype(it_x), typename container_t<float>::iterator const&>::value,
        "wrong iterator_x type");
    static_assert(std::is_same<decltype(cit_x), typename container_t
                               <float>::const_iterator const&>::value,
                  "wrong const_iterator_x type");
    static_assert(
        std::is_same
        <decltype(val_x), typename container_t<float>::value_type&>::value,
        "wrong value_type_x");
    static_assert(std::is_same<decltype(cval_x), float const&>::value,
                  "wrong const value_type_x");
    /// \todo these static_asserts fail
    // static_assert(
    //     std::is_same
    //     <decltype(reft), typename container_t<float>::reference>::value,
    //     "wrong reference_x type");
    // static_assert(
    //     std::is_same
    //     <decltype(creft), typename container_t<float>::const_reference>::value,
    //     "wrong const_reference_x type");

    auto rit_x = get<k::x>(it);
    auto rcit_x = get<k::x>(cit);
    auto rval_x = get<k::x>(val);
    auto rcval_x = get<k::x>(cval);
    auto rreft_x = get<k::x>(reft);
    auto rcreft_x = get<k::x>(creft);

    /// \todo static_asserts missing
  }
  SECTION("Iterators: Arithmetic operators") {
    using difference_type = std::remove_reference_t
        <decltype(ref.begin())>::difference_type;
    difference_type value = 5;

    REQUIRE((vec.end() - vec.begin()) == (ref.end() - ref.begin()));
    REQUIRE((vec.end() - (vec.begin() + value))
            == (ref.end() - (ref.begin() + value)));

    scattered::vector<TestType> e_vec;
    std::vector<TestType> e_ref;
    REQUIRE((e_vec.end() - e_vec.begin()) == (e_ref.end() - e_ref.begin()));
    REQUIRE((e_vec.end() - e_vec.begin()) == difference_type{0});
  }
  SECTION("Iterators: conversion operator") {
    scattered::vector<TestType> e_vec;
    auto mb = e_vec.begin();
    static_assert(!mb.is_const, "error: its non const!");
    auto ib = e_vec.cbegin();
    scattered::vector<TestType>::const_iterator cmb = mb;
    REQUIRE(cmb == ib);
  }
  SECTION("Relational operators") {
    scattered::vector<TestType> new_vec;
    std::vector<TestType> new_ref;

    are_equal(new_vec, new_ref);
    print_container("empty vec:", new_vec);
  }
  SECTION("Member function: insert value") {
    std::vector<TestType> new_ref;
    TestType t = {1.0, 2.0, 1, true};
    new_ref.insert(new_ref.cbegin(), t);

    scattered::vector<TestType> new_vec;
    new_vec.insert(new_vec.cbegin(), t);
    REQUIRE(new_vec.size() == 1);
    are_equal(new_vec, new_ref);

    scattered::vector<TestType> new_vec2;
    new_vec2.insert(new_vec2.begin(), t);
    REQUIRE(new_vec2.size() == 1);
    are_equal(new_vec2, new_ref);
  }
  SECTION("Member function: insert range") {
    scattered::vector<TestType> new_vec;
    new_vec.insert(new_vec.cbegin(), vec.cbegin(), vec.cend());
    REQUIRE(new_vec.size() == ref_size);
    are_equal(new_vec, ref);
    are_equal(vec, ref);

    scattered::vector<TestType> new_vec2;
    new_vec2.insert(new_vec2.begin(), vec.begin(), vec.end());
    REQUIRE(new_vec2.size() == ref_size);
    are_equal(new_vec2, ref);

    scattered::vector<TestType> new_vec3;
    new_vec3.insert(new_vec3.begin(), vec.cbegin(), vec.cend());
    REQUIRE(new_vec3.size() == ref_size);
    are_equal(new_vec3, ref);

    scattered::vector<TestType> new_vec4;
    new_vec4.insert(new_vec4.cbegin(), vec.begin(), vec.end());
    REQUIRE(new_vec4.size() == ref_size);
    are_equal(new_vec4, ref);
    are_equal(vec, ref);
  }
  SECTION("Adaptors: reversed") {
    scattered::vector<TestType> e_ref;
    scattered::vector<TestType> e_vec;

    REQUIRE(e_vec == e_ref);

    auto result = (e_vec | boost::adaptors::reversed) == e_ref;
    REQUIRE(result);
  }
  SECTION("Algorithm: stable_sort") {
    print_container("Initial values", vec);
    boost::stable_sort(
        vec, [](auto i, auto j) { return get<k::x>(i) > get<k::x>(j); });

    boost::stable_sort(ref, [](auto i, auto j) { return i.x > j.x; });

    are_equal(vec, ref);

    print_container("After stable_sort", vec);
  }
  SECTION("Algorithm: transform") {
    print_container("Initial values", vec);
    boost::transform(vec, std::begin(vec), [](auto i) {
      auto tmp = scattered::vector<TestType>::to_type(i);
      tmp.x *= tmp.x;
      tmp.y *= tmp.y;
      tmp.i *= tmp.i;
      tmp.b *= tmp.b;
      return scattered::vector<TestType>::from_type(tmp);
    });
    boost::transform(ref, std::begin(ref), [](auto i) {
      i.x *= i.x;
      i.y *= i.y;
      i.i *= i.i;
      i.b *= i.b;
      return i;
    });

    are_equal(vec, ref);

    print_container("After transform", vec);
  }
  SECTION("Algorithm: push_back") {
    scattered::vector<TestType> new_vec;
    std::vector<TestType> new_ref;

    boost::push_back(new_vec, vec);
    boost::push_back(new_ref, ref);
    are_equal(new_vec, new_ref);
    are_equal(new_vec, ref);

    print_container("After push_back", vec);
  }
  SECTION("Algorithm: push_back | reversed ") {
    scattered::vector<TestType> new_vec;
    std::vector<TestType> new_ref;

    boost::push_back(new_ref, ref | boost::adaptors::reversed);
    auto result = new_ref == (ref | boost::adaptors::reversed);
    REQUIRE(result);

    // boost::push_back(new_vec, vec | boost::adaptors::reversed);
    // are_equal(new_vec, new_ref);

    print_container("After push_back | reversed", vec);
  }
  /// We can push back both: the fusion map representing the struct
  // auto val = typename scattered::vector<TestType>::value_type{
  //     boost::fusion::make_pair<k::x>(static_cast<float>(1.0)),
  //     boost::fusion::make_pair<k::y>(static_cast<double>(2.0)),
  //     boost::fusion::make_pair<k::i>(static_cast<int>(3)),
  //     boost::fusion::make_pair<k::b>(true), };

  // vec.push_back(val);
  // std::cout << "After push_back of (1., 2., 3. true):\n";
  // print_container(vec);

  // /// And also the struct itself:
  // TestType tmp;
  // tmp.x = 4.0;
  // tmp.y = 3.0;
  // tmp.i = 2.0;
  // tmp.b = false;

  // vec.push_back(tmp);
  // std::cout << "After push_back of Struct (4., 3., 2. false):\n";
  // print_container(vec);

  // /// Example of get_container
  // std::cout << "elements of k::y container: (";
  // for (auto i : vec.data<k::y>()) {
  //   std::cout << i << ", ";
  // }
  // std::cout << ")\n";
}
