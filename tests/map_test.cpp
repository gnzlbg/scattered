#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <type_traits>
#include "scattered/detail/map.hpp"

struct K1 {}; struct K2 {}; struct K3 {};

/// \test scattered::pair tests
TEST_CASE("Test scattered::map<Pairs...>", "[scattered][map]") {

  auto p1 = scattered::make_pair<K1>(1);
  auto p2 = scattered::make_pair<K2>(true);
  auto p3 = scattered::make_pair<K3>(20l);

  auto mm = scattered::make_map(p1);
  const auto cm = scattered::make_map(p1);
  auto m1 = scattered::make_map(p1, p2, p3);
  const auto cm1 = scattered::make_map(p1, p2, p3);

  SECTION("for_each") {
    scattered::for_each(cm1, [](auto i) { std::cout << i.value << " - "; });
  }
  SECTION("types") {
    static_assert(std::is_same<scattered::map<scattered::pair<K1,int>>,
                  decltype((scattered::make_map(p1)))>::value,
                  "make_map should returns the wrong type!");
  }
  SECTION("get types") {

    REQUIRE( scattered::get<K1>(mm) == 1 );
    static_assert(std::is_same<
                  decltype(scattered::get<K1>(mm)),
                  int&>::value, "should be a ref to an int");
    static_assert(std::is_same<
                  decltype(scattered::get<K1>(cm)),
                  int const&>::value, "should be a cref to an int");
    scattered::get<K1>(scattered::make_map(p1)) = 3;
    REQUIRE( scattered::get<K1>(m1) == 1 );
    REQUIRE( scattered::get<K2>(m1) == true );
    REQUIRE( scattered::get<K3>(m1) == 20l );

    static_assert(std::is_same<
                  decltype(scattered::get<K1>(m1)),
                  int&>::value, "should be a ref to an int");
    static_assert(std::is_same<
                  decltype(scattered::get<K1>(cm1)),
                  int const&>::value, "should be a cref to an int");
    static_assert(std::is_same<
                  decltype(scattered::get<K2>(m1)),
                  bool&>::value, "should be a ref to an bool");
    static_assert(std::is_same<
                  decltype(scattered::get<K2>(cm1)),
                  bool const&>::value, "should be a cref to an bool");
    static_assert(std::is_same<
                  decltype(scattered::get<K3>(m1)),
                  long&>::value, "should be a ref to an long");
    static_assert(std::is_same<
                  decltype(scattered::get<K3>(cm1)),
                  long const&>::value, "should be a cref to an long");
  }

  SECTION("get mutation") {
    scattered::get<K1>(mm) = 2;
    REQUIRE( scattered::get<K1>(mm) == 2 );

    scattered::get<K1>(m1) = 2;
    scattered::get<K2>(m1) = false;
    scattered::get<K3>(m1) = 10l;
    REQUIRE( scattered::get<K1>(m1) == 2 );
    REQUIRE( scattered::get<K2>(m1) == false );
    REQUIRE( scattered::get<K3>(m1) == 10l );
  }

}
