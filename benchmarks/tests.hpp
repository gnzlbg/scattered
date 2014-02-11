// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_BENCHMARK_HPP)
#define SCATTERED_BENCHMARK_BENCHMARK_HPP

template<class Type,
         class Operation,
         class AccessPattern,
         class Container,
         class Initializer>
struct Benchmark {
  Benchmark() {}
  Benchmark(std::size_t size) : c(size) {
    Initializer{}(c);
  }
  void Benchmark() {
    AccessPattern{}(c, Operation{});
  }
  Container c;
};

static auto name(Benchmark<Type, Operation, AccessPattern,
                 Container, Initializer>) {
  return "Benchmark: operation" + name(Operation{})
      + ", access: " + name(AccessPattern{})
      + ", on type: " + name(Type{})
      + ", stored in container: " + name(Container{})
      + ", initialized with: " name(Initializer{});
}

#endif  // SCATTERED_BENCHMARK_BENCHMARK_HPP
