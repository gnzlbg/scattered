// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Implements Scattered Pair Container

#if !defined(SCATTERED_DETAIL_MAP2_HPP)
#define SCATTERED_DETAIL_MAP2_HPP

#include <tuple>
#include "pair.hpp"
#include "unqualified.hpp"
#include "returns.hpp"

namespace scattered {

template<class... Ps> struct map : Ps...  {
  map(const Ps&... ps) : Ps(ps)... {}
  map(Ps&&... ps)      : Ps(ps)... {}

  // using keys   = boost::mpl::vector<typename Ps::key...>;
  // using values = boost::mpl::vector<typename Ps::value_type...>;
  // using pairs  = boost::mpl::vector<Ps...>;
};

template<class... Ps> auto make_map(Ps&&... ps)
RETURNS(map<detail::unqualified_t<Ps>...> { std::forward<Ps>(ps)... });

template<class K, class V> auto const& get(const pair<K, V>& v) noexcept
{ return v.value; }
template<class K, class V> auto &      get(pair<K, V>& v)       noexcept
{ return v.value; }
template<class K, class V> auto &&     get(pair<K, V>&& v)      noexcept
{ return v.value; }

template<class... Args> inline auto swallow(Args...) {}

template<class F, class... Ps> void for_each(const map<Ps...>& m, F&& f) {
  auto dummy = { (f(static_cast<Ps>(m)), void(), 0)... };
}

}  // namespace scattered

#endif  // SCATTERED_DETAIL_MAP2_HPP
