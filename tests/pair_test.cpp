#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <type_traits>
#include "scattered/detail/pair.hpp"

/// \test scattered::pair tests
TEST_CASE("Test scattered::pair<Key,Value>", "[scattered][pair]") {
  struct Key1 {};
  auto p1 = scattered::make_pair<Key1>(1);
  static_assert(std::is_same<decltype(p1), scattered::pair<Key1, int>>::value, "");
  static_assert(scattered::is_pair<decltype(p1)>::value, "should be a pair!");
  static_assert(!scattered::is_pair<int>::value, "should not be a pair!");
  SECTION("scattered::make_pair") {
    p1.value = 2;
    REQUIRE(scattered::make_pair<Key1>(2) == p1);
    p1 = scattered::make_pair<Key1>(3);
    REQUIRE(p1.value == 3);
  }
  SECTION("relational opertors") {
    p1.value = 2;
    REQUIRE(scattered::make_pair<Key1>(2) == p1);
    REQUIRE(scattered::make_pair<Key1>(2) >= p1);
    REQUIRE(scattered::make_pair<Key1>(2) <= p1);
    REQUIRE(scattered::make_pair<Key1>(3) > p1);
    REQUIRE(scattered::make_pair<Key1>(3) >= p1);
    REQUIRE(scattered::make_pair<Key1>(1) < p1);
    REQUIRE(scattered::make_pair<Key1>(1) <= p1);
    REQUIRE(scattered::make_pair<Key1>(4) != p1);
  }
}
