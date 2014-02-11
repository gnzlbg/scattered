// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief as_fusion_map

#if !defined(SCATTERED_DETAIL_AS_FUSION_MAP_HPP)
#define SCATTERED_DETAIL_AS_FUSION_MAP_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/convert.hpp>

namespace scattered {

namespace detail {

/// The following as_fusion_map functionality was provided by Evgeny Panasyuk,
/// see: http://stackoverflow.com/questions/19657065/is-it-possible-to-generate\
/// -a-fusion-map-from-an-adapted-struct/19660249?noredirect=1#19660249
/// and the chat discussions of that day in LoungeC++.

template <class Vector, class First, class Last, class stop_iteration>
struct to_fusion_map_iter;

template <class Vector, class First, class Last>
struct to_fusion_map_iter<Vector, First, Last, boost::mpl::false_> {
  using Next = typename boost::fusion::result_of::next<First>::type;
  using type = typename boost::mpl::push_front
      <typename to_fusion_map_iter
       <Vector, Next, Last,
        typename boost::fusion::result_of::equal_to<Next, Last>::type>::type,
       boost::fusion::pair
       <typename boost::fusion::result_of::key_of<First>::type,
        typename boost::fusion::result_of::value_of_data<First>::type>>::type;
};
template <class Vector, class First, class Last>
struct to_fusion_map_iter<Vector, First, Last, boost::mpl::true_> {
  using type = Vector;
};

template <class FusionAssociativeSequence> struct as_fusion_map {
  using First = typename boost::fusion::result_of::begin
      <FusionAssociativeSequence>::type;
  using Last = typename boost::fusion::result_of::end
      <FusionAssociativeSequence>::type;
  using type = typename boost::fusion::result_of::as_map
      <typename to_fusion_map_iter<boost::mpl::vector<>, First, Last,
                                   typename boost::fusion::result_of::equal_to
                                   <First, Last>::type
                                   >::type
       >::type;
};

}  // namespace detail

}  // namespace scattered
#endif
