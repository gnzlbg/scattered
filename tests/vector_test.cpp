#include <iostream>
#include <algorithm>
#include <boost/range/algorithm.hpp>
#include <boost/tuple/tuple.hpp>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "scattered/vector.hpp"
#include "test_types.hpp"

TEST_CASE("Test scattered::vector<T>", "[scattered][vector]") {
  using k = TestType::k;  // Lets import the keys, we'll need them often.
  using scattered::get;

  /// pretty print:
  auto print = [](auto& o) {
    for (auto i : o) {
      std::cout << "(" << get<k::x>(i) << ", " << get<k::y>(i) << ", "
                << get<k::i>(i) << ", " << get<k::b>(i) << ")\n";
    }
  };

  /// We can make a vector from our Struct type
  scattered::vector<TestType> soa(10);

  /// Iterators/references/value_types are fusion::maps
  int count = 0;
  for (auto i : soa) {
    /// That is, i is a tuple of references, we access each member by key
    get<k::x>(i) = static_cast<float>(count);
    get<k::y>(i) = static_cast<double>(count);
    get<k::i>(i) = count;
    get<k::b>(i) = count % 2 == 0;
    ++count;
  }

  std::cout << "Initial values:\n";
  print(soa);

  /// Sort:
  boost::stable_sort(
      soa, [](auto i, auto j) { return get<k::x>(i) > get<k::x>(j); });

  std::cout << "After stable_sort:\n";
  print(soa);

  boost::transform(soa, std::begin(soa), [](auto i) {
    /// To create value_types, we need to pass key value pairs:
    /// (this can be simplified with a ::make_value function(...))
    return typename scattered::vector<TestType>::value_type{
        boost::fusion::make_pair<k::x>(get<k::x>(i) * get<k::x>(i)),
        boost::fusion::make_pair<k::y>(get<k::y>(i) * get<k::y>(i)),
        boost::fusion::make_pair<k::i>(get<k::i>(i) * get<k::i>(i)),
        boost::fusion::make_pair<k::b>(get<k::b>(i) * get<k::b>(i)), };
  });

  std::cout << "After transform:\n";
  print(soa);

  /// We can push back both: the fusion map representing the struct
  auto val = typename scattered::vector<TestType>::value_type{
      boost::fusion::make_pair<k::x>(static_cast<float>(1.0)),
      boost::fusion::make_pair<k::y>(static_cast<double>(2.0)),
      boost::fusion::make_pair<k::i>(static_cast<int>(3)),
      boost::fusion::make_pair<k::b>(true), };

  soa.push_back(val);
  std::cout << "After push_back of (1., 2., 3. true):\n";
  print(soa);

  /// And also the struct itself:
  TestType tmp;
  tmp.x = 4.0;
  tmp.y = 3.0;
  tmp.i = 2.0;
  tmp.b = false;

  soa.push_back(tmp);
  std::cout << "After push_back of Struct (4., 3., 2. false):\n";
  print(soa);

  /// Example of get_container
  std::cout << "elements of k::y container: (";
  for (auto i : soa.data<k::y>()) {
    std::cout << i << ", ";
  }
  std::cout << ")\n";
}
