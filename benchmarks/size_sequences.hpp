// (C) Copyright Gonzalo Brito Gadeschi 2014
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#if !defined(SCATTERED_BENCHMARK_SIZE_SEQUENCES_HPP)
#define SCATTERED_BENCHMARK_SIZE_SEQUENCES_HPP

////////////////////////////////////////////////////////////////////////////////

auto log_sequence(std::size_t lower, std::size_t upper) {
  std::vector<std::size_t> seq;
  seq.reserve(100);
  assert(lower > 0 && upper > 0 && upper > lower
         && "invalid range: lower < upper !");
  while (lower < upper) {
    for (std::size_t i = 1; i < 10 && (i * lower) < upper; ++i) {
      seq.emplace_back(i * lower);
    }
    lower *= 10;
  }
  return seq;
}

////////////////////////////////////////////////////////////////////////////////

#endif  // SCATTERED_BENCHMARK_SIZE_SEQUENCES_HPP
