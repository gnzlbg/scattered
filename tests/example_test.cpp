#include <algorithm>
#include <boost/range/algorithm.hpp>
#include <boost/fusion/adapted/struct/adapt_assoc_struct.hpp>
#include "scattered/vector.hpp"

/// A is a struct and k contains tags to be used as keys:
struct A {
  float x; double y; int i; bool b;
  struct k { struct x {}; struct y {}; struct i {}; struct b {}; };
};

// This adapts the class as an associative fusion sequence
BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    A, (float, x, A::k::x)(double, y, A::k::y)
       (int  , i, A::k::i)(bool  , b, A::k::b))

int main() {
  using scattered::get;
  using k = A::k;

  scattered::vector<A> vec(10);  ///< Make a vector of A with 10 elements

  /// We access each element with get<> + its key:
  int count = 0;
  for (auto i : vec) {
    get<k::x>(i) = static_cast<float>(count);
    get<k::y>(i) = static_cast<double>(count);
    get<k::i>(i) = count;
    get<k::b>(i) = count % 2 == 0;
    ++count;
  }

  /// All Boost and STL algorithms work out of the box
  boost::stable_sort(vec, [](auto i, auto j) {
    return get<k::x>(i) > get<k::x>(j);
  });

  /// reference/value_types with the original struct
  boost::transform(vec, std::begin(vec), [](auto i) {
      A tmp = scattered::vector<A>::to_type(i);
      tmp.x *= tmp.x; tmp.y *= tmp.y; tmp.i *= tmp.i; tmp.b *= tmp.b;
      return scattered::vector<A>::from_type(tmp);
  });

  A tmp = {4.0, 3.0, 2, false};
  vec.push_back(tmp);  ///< Or just: vec.push_back(A{{4.0, 3.0, 2.0, false}});
}
