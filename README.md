#### Introduction

In C++, object data members are layed out contiguously in memory. Scattered
containers store each object's data member sequentially in memory. This
significantly improves the cache performance of algorithms that iterate
sequentially over a container but access only a small subset of each object
data members in each pass.

Scattered containers is a header only library that depends on Boost.MPL and
Boost.Fusion. That is, there is no need to build the library in order to use it
in your project.

The following containers are provided:

- scattered::vector<T> (a.k.a. a Struct of Arrays or SOA)

#### Example: scattered::vector<T>

```c++
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

  /// from_type/to_t convert from/to the original type:
  boost::transform(vec, std::begin(vec), [](auto i) {
      A tmp = scattered::vector<A>::to_type(i);
      tmp.x *= tmp.x; tmp.y *= tmp.y; tmp.i *= tmp.i; tmp.b *= tmp.b;
      return scattered::vector<A>::from_type(tmp);
  });

  A tmp = {4.0, 3.0, 2, false};
  vec.push_back(tmp);  ///< Or just: vec.push_back(A{{4.0, 3.0, 2.0, false}});
}
```

#### Tutorial

Scattered works on associative Boost.Fusion sequences. You can use
[BOOST_FUSION_ADAPT_ASSOC_X](http://www.boost.org/doc/libs/1_55_0/libs/fusion/doc/html/fusion/adapted.html)
to adapt structs as associative Boost.Fusion sequences.

The keys of the associative sequences are then used to access the object
elements within the scattered containers by means of `scattered::at<key>`.
It is good practice to define the keys within the class itself. However,
sometimes one cannot modify the class. In this case, the keys can be defined
outside the class, e.g. inside a `class_name_keys` namespace.

#### Tests

The tests are located in the `/tests` subdirectory. They can be launched using
`make test`. The library has been successfully tested with clang 3.4 and Boost
1.54.

#### Benchmarks

The aim of the library is to maximize memory bandwidth usage by using cache
lines efficiently. Memory bandwitdth usage is one of the most important aspects
to consider when discussing performance of modern CPUs.

#### Todo:

Right now the priority is to finish `scattered::vector` before starting to
implement `scattered::flat_set` and `scattered::unordered_map`.

#### Acknowledgments

The Scattered library resulted from my efforts to improve the cache performance
of the numerical codes in use at the Institute of Aerodynamics, Aachen. This
probably would never have happened if Georg Geiser wouldn't have introduced me
to "What every computer scientist needs to know about memory". Furthermore, I
want to thank the guest of the LoungeC++ at stackoverflow.com for their fruitful
discussions. In particular, to Evgeny Panasyuk who motivated me to write this
library.