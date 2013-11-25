#### Introduction

Scattered containers store each object's data member sequentially in
memory. This improves cache line utilization when iterating sequentially over a
container without accessing all object's data members.

Scattered is a [Boost Software License](http://www.boost.org/LICENSE_1_0.txt)'d
header only C++1y library. It depends on Boost.MPL and Boost.Fusion, and is
tested with Boost 1.54 and trunk clang/libc++.

##### Example: scattered::vector<T>

```c++
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

/// This adapts the class as an associative fusion sequence
BOOST_FUSION_ADAPT_ASSOC_STRUCT(
    T, (float, x, T::k::x)(double, y, T::k::y)
       (int  , i, T::k::i)(bool  , b, T::k::b))

int main() {
  using scattered::get;
  using k = T::k;

  scattered::vector<T> vec(10);

  /// To modify the elements of the vector in place
  /// the keys are used on a reference proxy:
  int count = 0;
  for (auto i : vec) {
    get<k::x>(i) = static_cast<float>(count);
    get<k::y>(i) = static_cast<double>(count);
    get<k::i>(i) = count;
    get<k::b>(i) = count % 2 == 0;
    ++count;
  }

  /// Boost and STL algorithms work out of the box
  boost::stable_sort(vec, [](auto i, auto j) {
    return get<k::x>(i) > get<k::x>(j);
  });

  /// Reference proxy implicitly converts to T
  boost::transform(vec, std::begin(vec), [](T i) { i.y *= i.y; return i; });

  vec.push_back(T{4.0, 3.0, 2, false});
}
```

#### Getting started
 - `./configure.sh` is used to provide libc++'s path and select the compilation
 mode
 - `make` compiles the test, `ctest` launches all tests
 - `make docs` builds the documentation
 - `make check-format` / `make update-format` check/update the file formatting
    with clang-format.

#### Tutorial

The following containers are available:
  - `scattered::vector<T>` (analogous to `std::vector<T>`).

##### Caveats

Scattered containers use proxy reference types and, as a consequence, have the
following caveats:

```c++
// i's type = scattered::vector<T>::reference, not scattered::vector<T>::value_type
auto  i = *scatteredVector.begin();

// j's type = scattered::vector<T>::reference&
auto& j = *scatteredVector.begin();

// this modifies scatteredVector:
i = T{};

// to get a value you need to use value_type (or T)
T value = *scatteredVector.begin();
```

These are the same caveats of `std::vector<bool>`.

##### Main idea behind implementation

Scattered relies on
[BOOST_FUSION_ADAPT_ASSOC_X](http://www.boost.org/doc/libs/1_55_0/libs/fusion/doc/html/fusion/adapted.html)
to adapt classes as associative Boost.Fusion sequences. The
`scattered::get<key>(reference_proxy)` function returns a reference to the
object data member associated with the key.

#### Benchmarks

The aim of the library is to maximize memory bandwidth usage for algorithms that
iterate *sequentially* over the container.

#### Todo:

See the [roadmaps](https://github.com/gnzlbg/scattered/issues) page in the issue
list.

- Finish: `scattered::vector` will be provided.
- Provide other containers, e.g. `scattered::flat_set` and `scattered::unordered_map`.

#### Acknowledgments

The Scattered library resulted from efforts to improve the cache performance of
numerical fluid mechanics codes at the Institute of Aerodynamics, Aachen. I'd
like to thank Georg Geiser for introducing me to [What Every Programmer Should
Know About
Memory](http://people.freebsd.org/~lstewart/articles/cpumemory.pdf). Furthermore,
I want to thank the guests of the LoungeC++@stackoverflow.com for their
discussions, company, and help. In particular, to Evgeny Panasyuk who motivated
me to write this library.
