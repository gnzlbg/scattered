// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief map mutation helpers

#if !defined(SCATTERED_DETAIL_MAP_MUTATION_HPP)
#define SCATTERED_DETAIL_MAP_MUTATION_HPP

#include <boost/fusion/sequence/intrinsic/at_key.hpp>

namespace scattered {

namespace detail {

////////////////////////////////////////////////////////////////////////////////
/// The idea behind map mutator was provided by cv_and_he
/// see: http://stackoverflow.com/questions/19540291/assigning-to-a-map-of-\
/// references-from-a-map-of-values-references

template <class Output, class Predicate> struct map_mutator {
  map_mutator(Output& map) : map_out(map) {}
  template <class InputPair>
  [[gnu::always_inline, gnu::hot, gnu::flatten]] inline 
  void operator()(const InputPair& inputPair) const {
    Predicate()(inputPair, map_out);
  }
  Output& map_out;
};

struct assign {
  template <class Input, class Output>
  [[gnu::always_inline, gnu::hot, gnu::flatten]] inline 
  void operator()(const Input& i, Output& o) const {
    boost::fusion::at_key<typename Input::first_type>(o) = i.second;
  }
};

struct assign_dereference {
  template <class Input, class Output>
  [[gnu::always_inline, gnu::hot, gnu::flatten]] inline 
  void operator()(const Input& i, Output& o) const {
    boost::fusion::at_key<typename Input::first_type>(o) = *i.second;
  }
};

template <class Output> using map_assigner = map_mutator<Output, assign>;
template <class Output>
using map_dereferencer = map_mutator<Output, assign_dereference>;

}  // namespace detail

}  // namespace scattered
#endif
