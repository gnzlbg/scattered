#include <algorithm>
#include <boost/range/algorithm.hpp>
#include <boost/fusion/adapted/struct/adapt_assoc_struct.hpp>
#include <stdio.h>
#include "scattered/vector.hpp"


/// T is a struct; k contains keys to access the struct elements:
struct T {
  float x; double y; int i; bool b;
  struct k { struct x {}; struct y {}; struct i {}; struct b {}; };
};

// This adapts the class as an associative fusion sequence
BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    T, (float, x, T::k::x)(double, y, T::k::y)
       (int  , i, T::k::i)(bool  , b, T::k::b))

auto print_t(T t)
{ printf("(x = %2g, y = %2g, i = %2i, b = %i)\n", t.x, t.y, t.i, t.b); }

template<class C> auto print(C&& c) {
  printf("start:\n");
  for(T t : c) { print_t(t); }
  printf("end:\n");
}

int main() {
  using scattered::get;
  using k = T::k;

  scattered::vector<T> vec(10);

  int count = 0;
  for (auto i : vec) {
    get<k::x>(i) = static_cast<float>(count);
    get<k::y>(i) = static_cast<double>(count);
    get<k::i>(i) = count;
    get<k::b>(i) = count % 2 == 0;
    ++count;
  }
  print(vec);

  /// Boost and STL algorithms work out of the box
  boost::stable_sort(vec, [](auto i, auto j) {
    return get<k::x>(i) > get<k::x>(j);
  });
  print(vec);

  /// from_type/to_type convert from/to the original type:
  boost::transform(vec, std::begin(vec), [](T i) {
      i.x *= i.x; i.y *= i.y; i.i *= i.i; i.b *= i.b;
     return i;
  });
  print(vec);

  T tmp = {4.0, 3.0, 2, false};
  vec.push_back(tmp);
  print(vec);
}
