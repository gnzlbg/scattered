#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/tuple/tuple.hpp>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "test_types.hpp"
#include "scattered/vector.hpp"

//#define DEBUG_OUTPUT

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
  long ref_size = 10;
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

  SECTION("basic types and get<key>") {
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

    static_assert(
        std::is_same
        <scattered::detail::unqualified_t<decltype(get<k::x>(vec.begin()))>,
         scattered::detail::unqualified_t
         <decltype(get<k::x>(vec.begin().it()))>>::value,
        "get return value mismatch");

    static_assert(
        std::is_same
        <scattered::detail::unqualified_t<decltype(get<k::x>(vec.cbegin()))>,
         scattered::detail::unqualified_t
         <decltype(get<k::x>(vec.cbegin().it()))>>::value,
        "get return value mismatch");

    decltype(auto) it_x = get<k::x>(it);
    decltype(auto) cit_x = get<k::x>(cit);
    decltype(auto) val_x = get<k::x>(val);
    decltype(auto) cval_x = get<k::x>(cval);
    decltype(auto) reft_x = get<k::x>(reft);
    decltype(auto) creft_x = get<k::x>(creft);

    static_assert(
        std::is_same
        <decltype(it_x), typename container_t<float>::iterator>::value,
        "decltype(auto): wrong iterator_x type");
    static_assert(
        std::is_same
        <decltype(cit_x), typename container_t<float>::const_iterator>::value,
        "decltype(auto): wrong const_iterator_x type");
    static_assert(
        std::is_same
        <decltype(val_x), typename container_t<float>::value_type&>::value,
        "decltype(auto): wrong value_type_x");
    static_assert(std::is_same<decltype(cval_x), typename container_t
                               <float>::value_type const&>::value,
                  "decltype(auto): wrong const value_type_x");
    static_assert(
        std::is_same
        <decltype(reft_x), typename container_t<float>::reference>::value,
        "decltype(auto): wrong reference_x type");
    static_assert(std::is_same<decltype(creft_x), typename container_t
                               <float>::const_reference>::value,
                  "decltype(auto): wrong const_reference_x type");

    auto rit_x = get<k::x>(it);
    auto rcit_x = get<k::x>(cit);
    auto rval_x = get<k::x>(val);
    auto rcval_x = get<k::x>(cval);
    auto rreft_x = get<k::x>(reft);
    auto rcreft_x = get<k::x>(creft);

    static_assert(
        std::is_same
        <decltype(rit_x), typename container_t<float>::iterator>::value,
        "auto: wrong iterator_x type");
    static_assert(
        std::is_same
        <decltype(rcit_x), typename container_t<float>::const_iterator>::value,
        "auto: wrong const_iterator_x type");
    static_assert(
        std::is_same
        <decltype(rval_x), typename container_t<float>::value_type>::value,
        "auto: wrong value_type_x");
    static_assert(
        std::is_same
        <decltype(rcval_x), typename container_t<float>::value_type>::value,
        "auto: wrong const value_type_x");
    static_assert(
        std::is_same
        <decltype(rreft_x), typename container_t<float>::value_type>::value,
        "auto: wrong reference_x type");
    static_assert(
        std::is_same
        <decltype(rcreft_x), typename container_t<float>::value_type>::value,
        "auto: wrong const_reference_x type");

    auto b_x = get<k::x>(vec.begin());
    auto e_x = get<k::x>(vec.end());
    auto cb_x = get<k::x>(vec.cbegin());
    auto ce_x = get<k::x>(vec.cend());

    decltype(auto) rb_x = get<k::x>(vec.begin());
    decltype(auto) re_x = get<k::x>(vec.end());
    decltype(auto) rcb_x = get<k::x>(vec.cbegin());
    decltype(auto) rce_x = get<k::x>(vec.cend());

    REQUIRE((e_x - b_x) == ref_size);
    REQUIRE((ce_x - cb_x) == ref_size);
    REQUIRE((re_x - rb_x) == ref_size);
    REQUIRE((rce_x - rcb_x) == ref_size);

    auto b = vec.begin();
    auto e = vec.end();
    auto cb = vec.cbegin();
    auto ce = vec.cend();

    decltype(auto) rb = vec.begin();
    decltype(auto) re = vec.end();
    decltype(auto) rcb = vec.cbegin();
    decltype(auto) rce = vec.cend();

    REQUIRE((e - b) == ref_size);
    REQUIRE((ce - cb) == ref_size);
    REQUIRE((re - rb) == ref_size);
    REQUIRE((rce - rcb) == ref_size);

    REQUIRE((get<k::x>(e) - get<k::x>(b)) == ref_size);
    REQUIRE((get<k::x>(ce) - get<k::x>(cb)) == ref_size);
    REQUIRE((get<k::x>(re) - get<k::x>(rb)) == ref_size);
    REQUIRE((get<k::x>(rce) - get<k::x>(rcb)) == ref_size);

    REQUIRE((get<k::x>(vec.end().it()) - get<k::x>(vec.begin().it()))
            == ref_size);
    REQUIRE((get<k::y>(vec.end()) - get<k::y>(vec.begin())) == ref_size);
    REQUIRE((get<k::x>(vec.cend()) - get<k::x>(vec.cbegin())) == ref_size);
    REQUIRE((get<k::y>(vec.cend()) - get<k::y>(vec.cbegin())) == ref_size);
  }
  SECTION("Iterators: Arithmetic operators") {
    using difference_type = scattered::detail::unqualified_t
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
  SECTION("Iterators: conversions between const/non_const") {
    /// \todo
    scattered::vector<TestType> e_vec;
    auto mb = e_vec.begin();
    static_assert(!mb.is_const, "error: its non const!");
    auto ib = e_vec.cbegin();
    scattered::vector<TestType>::const_iterator cmb = mb;
    REQUIRE(cmb == ib);

    scattered::vector<TestType>::const_iterator cit = e_vec.cbegin();
    scattered::vector<TestType>::const_iterator cit2 = e_vec.begin();
    REQUIRE(cit == cit2);
    // this should fail to compile:
    // scattered::vector<TestType>::iterator cit3 = e_vec.cbegin();
  }
  SECTION("Iterators: references") {
    auto reference = *vec.begin();

    static_assert(
        std::is_same
        <decltype(reference), typename scattered::vector<TestType>::reference>::value,
        "wrong reference type");

    TestType test = { 5.0, 4.0, 1, false };
    reference = test;
    auto modified = scattered::vector<TestType>::to_type(reference);

    REQUIRE( modified == test );

    TestType other = *vec.begin();
    TestType cother = *vec.cbegin();
    REQUIRE (other == test);
    REQUIRE (cother == test);
  }
  SECTION("Iterators: general tests") {}
  SECTION("Relational operators") {
    /// \todo
    scattered::vector<TestType> new_vec;
    std::vector<TestType> new_ref;

    are_equal(new_vec, new_ref);
    print_container("empty vec:", new_vec);
  }
  SECTION("Member function: at") {
    /// \todo
  }
  SECTION("Member function: operator[]") {
    /// \todo
  }
  SECTION("Member function: front/back") {
    /// \todo
  }
  SECTION("Member function: capacity/empty/reserve/shrink_to_fit") {
    /// \todo
  }
  SECTION("Member function: clear") {
    /// \todo
  }
  SECTION("Member function: push_back/pop_back") {
    /// \todo pop_back
    scattered::vector<TestType> new_vec;

    TestType tmp_ref;
    tmp_ref.x = 4.0;
    tmp_ref.y = 3.0;
    tmp_ref.i = 2.0;
    tmp_ref.b = false;

    new_vec.push_back(tmp_ref);
    auto tmp = scattered::vector<TestType>::to_type(new_vec[0]);

    REQUIRE(tmp == tmp_ref);

    TestType tmp_ref2 = {4.0, 3.0, 2, false};

    new_vec.push_back(TestType{4.0, 3.0, 2, false});
    auto tmp2 = scattered::vector<TestType>::to_type(new_vec[1]);

    REQUIRE(tmp2 == tmp_ref2);
  }
  SECTION("Member function: resize") {
    /// \todo
  }
  SECTION("Member function: swap") {
    /// \todo
  }
  SECTION("Member function: data") {
    /// \todo data test
    // /// Example of get_container
    // std::cout << "elements of k::y container: (";
    // for (auto i : vec.data<k::y>()) {
    //   std::cout << i << ", ";
    // }
    // std::cout << ")\n";
  }
  SECTION("Member function: erase position") {
    /// \todo
  }
  SECTION("Member function: erase range") {
    /// \todo
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
    REQUIRE(( long )new_vec.size() == ref_size);
    are_equal(new_vec, ref);
    are_equal(vec, ref);

    scattered::vector<TestType> new_vec2;
    new_vec2.insert(new_vec2.begin(), vec.begin(), vec.end());
    REQUIRE(( long )new_vec2.size() == ref_size);
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

    for(auto i : vec | boost::adaptors::reversed) {
      std::cout << "(" << get<k::x>(i) << ", " << get<k::y>(i) << ", "
                << get<k::i>(i) << ", " << get<k::b>(i) << ")\n";
    }

    // boost::push_back(new_vec, vec | boost::adaptors::reversed);
    are_equal(new_vec, new_ref);

    print_container("After push_back | reversed", vec);
  }
}
