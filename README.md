#### Introduction

Scattered container store each type's data member sequentially for all
objects. That is, the first data member of all objects is stored contiguosly in
memory, then the second data member, and so on as shown in the following figure: 

![Memory layout of standard and scattered
 containers](https://rawgithub.com/gnzlbg/scattered/master/docs/img/memory_layout.svg
 "Memory layout of standard and scattered containers for a type containing an
 int, a bool, and a double as data members.")

This improves cache line utilization when iterating sequentially over a
container without accessing all object's data members. They also allow
asynchronous processing of object's data member without false sharing.

The following containers are available:
  - `scattered::vector<T>` (analogous to `std::vector<T>`).

Scattered is a [Boost Software License](http://www.boost.org/LICENSE_1_0.txt)'d
header only C++1y library and is tested with Boost 1.54 (1.55 not supported yet,
see issue tracker) and trunk clang/libc++. It depends on [Boost.MPL]() and
[Boost.Fusion]().

##### Example: `scattered::vector<T>`

```c++
#include <algorithm>
#include <boost/range/algorithm.hpp>
#include <boost/fusion/adapted/struct/adapt_assoc_struct.hpp>
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

  /// Cache lines contain only "y" data-members:
  for (auto i : vec) { get<k::y>(i) += get<k::y>(i); }

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
 mode (debug/release/sanitizers are provided, see `./configure,sh -h`).
 - `make` compiles the tests, `ctest` launches all tests.
 - `make docs` builds the documentation.
 - `make update-format` reformats the code with clang-format.
 - `make bench` runs all benchmarks.

##### Caveats

Scattered containers use proxy reference types and, as a consequence, have the
following caveats similar to those of `std::vector<bool>`:

```c++
// i's type = scattered::vector<T>::reference, not scattered::vector<T>::value_type
auto  i = *scatteredVector.begin();

// j's type = scattered::vector<T>::reference&
auto& j = *scatteredVector.begin();

// this modifies scatteredVector:
i = T{};

// to get a value you need to use value_type (or T)
T value = *scatteredVector.begin();

// This fails because T& cannot bind to scattered::vector<T>::reference&
T& ref = *catteredVector.begin();  // Compilation Error
```

##### Main idea behind implementation

Scattered relies on
[BOOST_FUSION_ADAPT_ASSOC_X](http://www.boost.org/doc/libs/1_55_0/libs/fusion/doc/html/fusion/adapted.html)
to adapt classes as associative Boost.Fusion sequences. The
`scattered::get<key>(reference_proxy)` function returns a reference to the
object's data member associated with the `key`.

#### Benchmarks

The aim of the library is to maximize memory bandwidth usage for algorithms that
iterate *sequentially* over the container. The following benchmarks are located
in the `benchmarks/` directory and can be run with `make bench`.

#### Todo:

See the [roadmaps](https://github.com/gnzlbg/scattered/issues) page in the issue
list.

- Finish: `scattered::vector` will be provided.
- Other containers: `scattered::flat_set` and `scattered::unordered_map`.

#### Acknowledgments

The Scattered library resulted from efforts to improve the cache performance of
numerical fluid mechanics codes at the Institute of Aerodynamics, Aachen. I'd
like to thank Georg Geiser for introducing me to [What Every Programmer Should
Know About
Memory](http://people.freebsd.org/~lstewart/articles/cpumemory.pdf). Furthermore,
I want to thank the guests of the
[LoungeC++](http://chat.stackoverflow.com/rooms/10/loungec) for their
discussions, company, and help. In particular, to Evgeny Panasyuk who motivated
me to write this library.
