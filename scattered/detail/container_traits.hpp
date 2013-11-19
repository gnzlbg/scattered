// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Implements container traits

#if !defined(SCATTERED_DETAIL_CONTAINER_TRAITS_HPP)
#define SCATTERED_DETAIL_CONTAINER_TRAITS_HPP

#include <boost/mpl/transform.hpp>
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/convert.hpp>

namespace scattered {

namespace detail {

template <class column_types, class column_tags,
          template <class> class container_type>
struct container_traits {
  /// Data-member container types:
  ///@{
  using types = column_types;
  using tags = column_tags;
  template <class T> struct container {
    using type = container_type<T>;
  };
  template <class T> struct iterator {
    using type = typename container_type<T>::iterator;
  };
  template <class T> struct const_iterator {
    using type = typename container_type<T>::const_iterator;
  };
  template <class T> struct reverse_iterator {
    using type = typename container_type<T>::reverse_iterator;
  };
  template <class T> struct const_reverse_iterator {
    using type = typename container_type<T>::const_reverse_iterator;
  };
  template <class T> struct reference {
    using type = typename container_type<T>::reference;
  };
  template <class T> struct const_reference {
    using type = typename container_type<T>::const_reference;
  };
  template <class T> struct pointer {
    using type = typename container_type<T>::pointer;
  };
  template <class T> struct const_pointer {
    using type = typename container_type<T>::const_pointer;
  };
  template <class T> struct value {
    using type = typename container_type<T>::value_type;
  };
  template <class T> struct difference_type {
    using type = typename container_type<T>::difference_type;
  };
  template <class T> struct allocator_type {
    using type = typename container_type<T>::allocator_type;
  };
  template <class T> struct size_type {
    using type = typename container_type<T>::allocator_type;
  };
  ///@}

  /// \brief (column_tags, column_types) -> (column_tags, P(column_types))
  template <class tags_, class types_> struct transform_to_pair {
    template <template <class> class P>
    using transformed = typename boost::fusion::result_of::as_map
        <typename boost::mpl::transform
         <typename boost::mpl::transform<types_, P<boost::mpl::_1>>::type,
          tags_,
          boost::fusion::pair<boost::mpl::_2, boost::mpl::_1>>::type>::type;
  };

  /// Container Types:
  ///@{
  using value_type = typename transform_to_pair
      <tags, types>::template transformed<value>;
  using reference_type = typename transform_to_pair
      <tags, types>::template transformed<reference>;
  using pointer_type = typename transform_to_pair
      <tags, types>::template transformed<pointer>;
  using iterator_type = typename transform_to_pair
      <tags, types>::template transformed<iterator>;
  using const_iterator_type = typename transform_to_pair
      <tags, types>::template transformed<const_iterator>;
  using const_reference_type = typename transform_to_pair
      <tags, types>::template transformed<const_reference>;
  using const_pointer_type = typename transform_to_pair
      <tags, types>::template transformed<const_pointer>;

  ///@}
};

}  // namespace detail

}  // namespace scattered
#endif  // SCATTERED_DETAIL_CONTAINER_TRAITS_HPP
