// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Implements Scattered Vector Container

#if !defined(SCATTERED_DETAIL_VECTOR_HPP)
#define SCATTERED_DETAIL_VECTOR_HPP

#include <stdexcept>
#include <type_traits>
#include <boost/container/vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include "fusion_swap.hpp"
#include "as_fusion_map.hpp"
#include "vector_iterator_base.hpp"
#include "get.hpp"

namespace scattered {

template <class T>
using default_vector_container = boost::container::vector<T, std::allocator<T>>;

/// \brief scattered vector
template <class T, template <class> class Container = default_vector_container>
class vector {
  /// \name Vector utilities
  ///@{
  /// T as a associative fusion map:
  using MemberMap = typename detail::as_fusion_map<T>::type;
  /// P -> P::key_type
  template <class P> struct key_of {
    using type = typename std::remove_reference_t<P>::first_type;
  };
  template <class P> using key_of_t = typename key_of<P>::type;
  /// P -> P::value_type
  template <class P> struct val_of {
    using type = typename P::second_type;
  };
  /// T -> (key_of<member0>..key_of<memberN>)
  using keys = typename boost::mpl::transform
      <MemberMap, key_of<boost::mpl::_1>>::type;
  /// T -> (val_of<member0>..val_of<memberN>)
  using values = typename boost::mpl::transform
      <MemberMap, val_of<boost::mpl::_1>>::type;
  /// P<key, val> -> P<key, Container<val>>
  template <class P> struct container_of {
    using type = boost::fusion::pair
        <typename P::first_type, Container<typename P::second_type>>;
  };
  /// T -> (Container<T.member0>..Container<T.memberN>)
  using data_type = typename boost::mpl::transform
      <MemberMap, container_of<boost::mpl::_1>>::type;
  ///@}

  data_type data_;

  struct const_tag {};

 public:
  /// Container traits
  ///{@
  template <class U> using container_type = Container<U>;
  using size_type = std::size_t;
  using iterator = typename detail::vector_iterator_base
      <const_tag, values, keys, Container>;
  using const_iterator = typename detail::vector_iterator_base
      <const const_tag, values, keys, Container>;
  using value_type = typename iterator::value_type;
  using reference = typename iterator::reference;
  using const_reference = typename const_iterator::reference;
  using difference_type = typename iterator::difference_type;
  using pointer = typename iterator::pointer;
  using const_pointer = typename const_iterator::pointer;
  using reverse_iterator = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
  ///@}

  /// Constructors
  ///@{
  explicit vector(size_type n = 0) { resize(n); }
  vector(const vector& other) : data_(other.data_) {}
  vector(vector&& other) : data_(std::move(other.data_)) {}

  vector& operator=(const vector& other) { data_(other.data_); }
  vector& operator=(vector&& other) { data_(std::move(other.data_)); }
  ///@}

  /// Iterators
  ///@{
 private:
  struct make_begin_it {
    template <class U> auto operator()(U& i) const {
      using key_type = typename key_of<U>::type;
      using val_type = typename val_of<U>::type;
      return boost::fusion::make_pair
          <key_type>(const_cast<val_type&>(i.second).begin());
    }
  };

  struct make_cbegin_it {
    template <class U> auto operator()(U& i) const {
      using key_type = typename key_of<U>::type;
      using val_type = typename val_of<U>::type;
      return boost::fusion::make_pair<key_type>((i.second).begin());
    }
  };

  struct make_end_it {
    template <class U> auto operator()(U& i) const {
      using key_type = typename key_of<U>::type;
      using val_type = typename val_of<U>::type;
      return boost::fusion::make_pair
          <key_type>(const_cast<val_type&>(i.second).end());
    }
  };

  struct make_cend_it {
    template <class U> auto operator()(U& i) const {
      using key_type = typename key_of<U>::type;
      using val_type = typename val_of<U>::type;
      return boost::fusion::make_pair<key_type>((i.second).end());
    }
  };

 public:
  inline iterator begin() {
    return iterator::from_map(boost::fusion::transform(data_, make_begin_it()));
  }
  inline iterator end() {
    return iterator::from_map(boost::fusion::transform(data_, make_end_it()));
  }
  inline const_iterator begin() const {
    return const_iterator::from_map(
        boost::fusion::transform(data_, make_cbegin_it()));
  }
  inline const_iterator end() const {
    return const_iterator::from_map(
        boost::fusion::transform(data_, make_cend_it()));
  }
  inline const_iterator cbegin() const { return begin(); }
  inline const_iterator cend() const { return end(); }

  reverse_iterator rbegin() { return end(); }
  reverse_iterator rend() { return begin(); }
  const_reverse_iterator rbegin() const { return cend(); }
  const_reverse_iterator rend() const { return cbegin(); }
  ///@}

  /// Element Access
  ///@{
  reference at(size_type pos) {
    if (!(pos < size())) {
      throw std::out_of_range("scattered::vector::at(" + std::to_string(pos)
                              + ") is out of bounds [0,"
                              + std::to_string(size()) + ")");
    } else {
      return (*this)[pos];
    }
  }
  constexpr const_reference at(size_type pos) const {
    if (!(pos < size())) {
      throw std::out_of_range("scattered::vector::at(" + std::to_string(pos)
                              + ") is out of bounds [0,"
                              + std::to_string(size()) + ")");

    } else {
      return (*this)[pos];
    }
  }
  inline reference operator[](size_type pos) { return begin()[pos]; }
  inline const_reference operator[](size_type pos) const {
    return cbegin()[pos];
  }
  reference front() { return *begin(); }
  constexpr const_reference front() const { return *cbegin(); }
  reference back() { return *end(); }
  constexpr const_reference back() const { return *cend(); }
  data_type& data() { return data_; }
  data_type const& data() const { return data_; }
  template <class K> auto& data() { return get<K>(data_); }
  template <class K> auto const& data() const { return get<K>(data_); }
  ///@}

  /// Capacity
  ///@{
  inline bool empty() const {
    return boost::fusion::all(data_, [](auto i) { return i.second.empty(); });
  }
  size_type capacity() const {
    auto cap = boost::fusion::at_c<0>(data_).second.capacity();
    return cap;
  }
  size_type size() const {
    auto size_ = boost::fusion::at_c<0>(data_).second.size();
    return size_;
  }
  // reserve
  struct reserve_ {
    reserve_(std::size_t n) : n_(n) {}
    template <class U> void operator()(U&& i) const { i.second.reserve(n_); }
    const std::size_t n_;
  };
  void reserve(std::size_t n) {
    boost::fusion::for_each(data_, [&](auto i) { i.second.reserve(n); });
  }
  void shrink_to_fit() {
    boost::fusion::for_each(data_, [](auto i) { i.second.shrink_to_fit(); });
  }
  void max_size() {
    boost::fusion::for_each(data_, [](auto i) { i.second.max_size(); });
  }
  ///@}

  /// Modifiers
  ///@{
  void clear() {
    boost::fusion::for_each(data_, [](auto i) { i.second.clear(); });
  }
  iterator insert(const_iterator pos, const T& value) {
    const auto offset = pos - cbegin();
    boost::fusion::for_each(boost::fusion::zip(MemberMap{}, value),
                            [&](auto&& i) {
      using key = key_of_t<decltype(boost::fusion::at_c<0>(i))>;
      auto local_pos = get<key>(data_).cbegin() + offset;
      get<key>(data_).insert(local_pos, boost::fusion::at_c<1>(i));
    });
    return begin() + offset;
  }
  // iterator insert( const_iterator pos, size_type count, const T& value );
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    std::cerr << "range insert: " << last - first << "\n";
    boost::fusion::for_each(zip(first.it(), last.it()), [](auto&& i) {
      std::cerr << "rc: l - f: " << &*boost::fusion::at_c<1>(i).second << " - "
                << &*boost::fusion::at_c<0>(i).second << " = "
                << boost::fusion::at_c<1>(i).second - boost::fusion::at_c
                                                      <0>(i).second << "\n";
    });
    const auto offset = pos - cbegin();
    if (first != last) {
      boost::fusion::for_each(data_, [&](auto&& i) {
        using key = key_of_t
            <std::remove_cv_t<std::remove_reference_t<decltype(i)>>>;
        using local_const_iterator = typename std::remove_cv_t
            <std::remove_reference_t<decltype(i.second)>>::const_iterator;
        auto local_pos = i.second.cbegin() + offset;
        // auto fIt = boost::fusion::at_key<key>(first.it());
        // auto lIt = boost::fusion::at_key<key>(last.it());
        // auto fIt = get<key>(first.it());
        // auto lIt = get<key>(last.it());
        auto fIt = get<key>(first);
        auto lIt = get<key>(last);
        std::cerr << "l - f: " << &*lIt << " - " << &*fIt << " = " << lIt - fIt
                  << "\n";
        i.second.insert(local_pos, fIt, lIt);
      });
    }
    return begin() + offset;
  };

  iterator erase(iterator pos) {
    const auto offset = pos - begin();
    boost::fusion::for_each(data_, [&](auto&& i) {
      auto local_pos = i.second.cbegin() + offset;
      i.second.erase(local_pos);
    });
  }
  iterator erase(iterator first, iterator last) {
    const auto first_offset = first - begin();
    const auto last_offset = last - begin();
    boost::fusion::for_each(data_, [&](auto&& i) {
      auto local_first = i.second.cbegin() + first_offset;
      auto local_last = i.second.cbegin() + last_offset;
      i.second.erase(first, last);
    });
  }
  iterator erase(const_iterator pos) {
    const auto offset = pos - cbegin();
    boost::fusion::for_each(data_, [&](auto&& i) {
      auto local_pos = i.second.cbegin() + offset;
      i.second.erase(local_pos);
    });
  }
  iterator erase(const_iterator first, const_iterator last) {
    const auto first_offset = first - cbegin();
    const auto last_offset = last - cbegin();
    boost::fusion::for_each(data_, [&](auto&& i) {
      auto local_first = i.second.cbegin() + first_offset;
      auto local_last = i.second.cbegin() + last_offset;
      i.second.erase(first, last);
    });
  }
  void push_back(const T& value) {
    boost::fusion::for_each(boost::fusion::zip(MemberMap{}, value),
                            [&](auto&& i) {
      using key = key_of_t<decltype(boost::fusion::at_c<0>(i))>;
      get<key>(data_).push_back(boost::fusion::at_c<1>(i));
    });
  };
  void push_back(T&& value) {
    boost::fusion::for_each(boost::fusion::zip(MemberMap{}, value),
                            [&](auto&& i) {
      using key = key_of_t<decltype(boost::fusion::at_c<0>(i))>;
      get<key>(data_).emplace_back(boost::fusion::at_c<1>(std::move(i)));
    });
  };
  void push_back(const value_type& value) {
    boost::fusion::for_each(value, [&](auto&& i) {
      using key = key_of_t<decltype(i)>;
      get<key>(data_).push_back(i.second);
    });
  };
  void push_back(value_type&& value) {
    boost::fusion::for_each(std::move(value), [&](auto&& i) {
      using key = key_of_t<decltype(i)>;
      get<key>(data_).emplace_back(std::move(i.second));
    });
  };
  void pop_back() {
    boost::fusion::for_each(data_, [&](auto&& i) { i.second.pop_back(); });
  }
  void resize(std::size_t n) {
    boost::fusion::for_each(data_, [&](auto&& i) { i.second.resize(n); });
  }
  ///@}

  /// Non-member functions
  ///@{
  inline friend void swap(vector&& a, vector&& b) {
    data_type tmp = std::move(a.data_);
    a.data_ = std::move(b.data_);
    b.data_ = std::move(tmp);
  }

  friend bool operator==(const vector& lhs, const vector& rhs) {
    return boost::fusion::all(boost::fusion::zip(lhs.data_, rhs.data_),
                              [](auto&& i) {
      return boost::fusion::at_c<0>(i) == boost::fusion::at_c<1>(i);
    });
  }
  friend bool operator<=(const vector& lhs, const vector& rhs) {
    return boost::fusion::all(boost::fusion::zip(lhs.data_, rhs.data_),
                              [](auto&& i) {
      return boost::fusion::at_c<0>(i) <= boost::fusion::at_c<1>(i);
    });
  }
  friend bool operator>=(const vector& lhs, const vector& rhs) {
    return boost::fusion::all(boost::fusion::zip(lhs.data_, rhs.data_),
                              [](auto&& i) {
      return boost::fusion::at_c<0>(i) >= boost::fusion::at_c<1>(i);
    });
  }

  friend bool operator!=(const vector& lhs, const vector& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const vector& lhs, const vector& rhs) {
    return !(lhs == rhs && lhs >= rhs);
  }
  friend bool operator>(const vector& lhs, const vector& rhs) {
    return !(lhs == rhs && lhs <= rhs);
  }
  ///@}

  static T to_type(reference ref) {
    T tmp;
    boost::fusion::for_each(ref, [&](auto&& i) {
      using key = key_of_t<decltype(i)>;
      get<key>(tmp) = i.second;
    });
    return tmp;
  }
  static T& to_type(T& t) { return t; }
  static T to_type(T&& t) { return t; }
  static T to_type(const_reference ref) {
    T tmp;
    boost::fusion::for_each(ref, [&](auto&& i) {
      using key = key_of_t<decltype(i)>;
      get<key>(tmp) = i.second;
    });
    return tmp;
  }
  static value_type& from_type(value_type& value) { return value; }
  static value_type from_type(value_type&& value) { return value; }
  static reference from_type(reference&& value) { return value; }
  static reference& from_type(reference& value) { return value; }
  static value_type from_type(T& value) {
    value_type tmp;
    boost::fusion::for_each(boost::fusion::zip(MemberMap{}, value),
                            [&](auto&& i) {
      using key = key_of_t<decltype(boost::fusion::at_c<0>(i))>;
      get<key>(tmp) = boost::fusion::at_c<1>(i);
    });
    return tmp;
  }
};

// relational operators

}  // namespace scattered

#endif  // SCATTERED_DETAIL_VECTOR_HPP
