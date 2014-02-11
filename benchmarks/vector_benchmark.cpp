// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <vector>
#include <iostream>
#include <fstream>
#include "scattered/vector.hpp"
#include "benchmark.hpp"
#include "operations.hpp"
#include "access_patterns.hpp"
#include "size_sequences.hpp"
#include "initializers.hpp"
#include "cartesian_product.hpp"

template<class T> using std_vector       = std::vector<T>;
template<class T> auto name(std_vector<T>) RETURNS("std_vector");
template<class T> using scattered_vector = scattered::vector<T>;
template<class T> auto name(scattered_vector<T>) RETURNS("scattered_vector");

template<template <class> class Container> struct run_benchmark {
  template <typename Seq> void operator()(Seq) {
    using operation     = typename boost::mpl::at_c<Seq, 0>::type;
    using access_pattern = typename boost::mpl::at_c<Seq, 1>::type;
    using type          = typename boost::mpl::at_c<Seq, 2>::type;
    using container = Container<type>;

    using Bench = Benchmark<type, operation, access_pattern, container,
                            fill_with_random_values>;

    const int no_runs = 100;
    //auto sizes = log_sequence(1, 1e7);
    auto sizes = std::initializer_list<std::size_t>{static_cast<std::size_t>(2e7)};
    std::vector<std::tuple<std::size_t, long>> times; times.reserve(no_runs);
    for (auto size: sizes) {
      Bench b{size};
      long avg_time = 0;
      for (int i = 0; i < no_runs; ++i) {
        avg_time += b();
      }
      avg_time /= no_runs;
      times.emplace_back(std::make_tuple(size, avg_time));
    }
    {
      std::ofstream f;
      f.open (fname(Bench{}) + ".dat");
      std::cout << "# " << name(Bench{}) << "\n";
      f << "# " << name(Bench{}) << "\n";
      using std::setw;
      for (auto time : times) {
        std::cout << setw(20) << std::get<0>(time) << setw(20) << std::get<1>(time) << "\n";
        f << setw(20) << std::get<0>(time) << setw(20) << std::get<1>(time) << "\n";
      }
    }
  }
};

int main() {
  boost::mpl::cartesian_product<boost::mpl::vector<
                                  operations,
                                  access_patterns,
                                  test_types
                                  >>(run_benchmark<std_vector>());

  boost::mpl::cartesian_product<boost::mpl::vector<
                                  operations,
                                  access_patterns,
                                  test_types
                                  >>(run_benchmark<scattered_vector>());
  return 0;
}
