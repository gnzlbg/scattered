// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Implements Scattered Pair Container

#if !defined(SCATTERED_DETAIL_PAIR_HPP)
#define SCATTERED_DETAIL_PAIR_HPP

namespace scattered {

template<class Key, class Value> struct pair {
  using key = Key;
  using value_type = Value;
  Value value;
};

template <class Key, class Value>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]]
inline bool operator==(scattered::pair<Key, Value> const& l,
                       scattered::pair<Key, Value> const& r) noexcept {
  return l.value == r.value;
}
template <class Key, class Value>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]]
inline bool operator<=(scattered::pair<Key, Value> const& l,
                       scattered::pair<Key, Value> const& r) noexcept {
  return l.value <= r.value;
}
template <class Key, class Value>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]]
inline bool operator>=(scattered::pair<Key, Value> const& l,
                       scattered::pair<Key, Value> const& r) noexcept {
  return l.value >= r.value;
}
template <class Key, class Value>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]]
inline bool operator!=(scattered::pair<Key, Value> const& l,
                       scattered::pair<Key, Value> const& r) noexcept {
  return !(l == r);
}
template <class Key, class Value>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]]
inline bool operator>(scattered::pair<Key, Value> const& l,
                      scattered::pair<Key, Value> const& r) noexcept {
  return !(l <= r);
}
template <class Key, class Value>
[[gnu::always_inline, gnu::hot, gnu::const, gnu::flatten]]
inline bool operator<(scattered::pair<Key, Value> const& l,
                      scattered::pair<Key, Value> const& r) noexcept {
  return !(l >= r);
}

template<class Key, class Value>
pair<Key, Value> make_pair(Value&& value) { return {value}; }

template<class T> struct is_pair {
  static const bool value = false;
  using type = std::false_type;
};
template<class Key, class Value> struct is_pair<scattered::pair<Key, Value>> {
  static const bool value = true;
  using type = std::true_type;
  using value_type = scattered::pair<Key, Value>;
};

}  // namespace scattered

#endif  // SCATTEValueED_DETAIL_PAIR_HPP
