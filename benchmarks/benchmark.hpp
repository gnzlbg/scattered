// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_BENCHMARK_HPP)
#define SCATTERED_BENCHMARK_BENCHMARK_HPP

#include <sstream>
#include <iomanip>
#include "time_function.hpp"

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
  auto operator()() {
    return time_fn([&]() { AccessPattern{}(c, Operation{}); });
  }
  Container c;
};

template<class Type, class Operation, class AccessPattern, class Container,
         class Initializer>
auto name(Benchmark<Type, Operation, AccessPattern,
                 Container, Initializer>) {
  using std::setw; using std::left;
  auto n = std::string{"Benchmark: "} + name(Container{}) + "<"  + name(Type{}) + ">";
  auto op = std::string{"op: "} + name(Operation{});
  auto ap = std::string{"ap: "} + name(AccessPattern{});
  std::stringstream ss;
  ss << setw(50) << left << n;
  ss << setw(50) << left << op;
  ss << setw(50) << left << ap << "\n";
  return ss.str();
}

template<class Type, class Operation, class AccessPattern, class Container,
         class Initializer>
auto fname(Benchmark<Type, Operation, AccessPattern,
                 Container, Initializer>) {

  return name(Container{}) + std::string{"_"}  + name(Type{}) + "_"
      + name(Operation{}) + "_" + name(AccessPattern{});
}

#endif  // SCATTERED_BENCHMARK_BENCHMARK_HPP
