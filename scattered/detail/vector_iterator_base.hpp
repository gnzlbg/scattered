// (C) Copyright Gonzalo Brito Gadeschi 2013
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

/// \file \brief Random Access Iterator for scattered vector

#if !defined(SCATTERED_DETAIL_VECTOR_ITERATOR_BASE_HPP)
#define SCATTERED_DETAIL_VECTOR_ITERATOR_BASE_HPP

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/algorithm/iteration/accumulate.hpp>
#include <boost/fusion/algorithm/transformation/zip.hpp>
#include "returns.hpp"
#include "map_mutation.hpp"
#include "container_traits.hpp"

namespace scattered {

namespace detail {

/// \brief RandomAccessIterator for scattered::vector container
/// If const_tag is const -> const_iterator, otherwise -> iterator
template <class const_tag, class column_types, class column_tags,
          template <class> class container_type>
class vector_iterator_base {
  /// \name Utility aliases
  ///@{
  using types = column_types;
  using tags = column_tags;
  using container = container_traits<types, tags, container_type>;
  using This = vector_iterator_base<const_tag, types, tags, container_type>;
  static const constexpr bool is_const = std::is_const<const_tag>::value;
  ///@}

  /// \name Map types
  ///@{
  using iterator_map = std::conditional_t
      <!is_const, typename container::iterator_type,
       typename container::const_iterator_type>;
  using reference_map = std::conditional_t
      <!is_const, typename container::reference_type,
       typename container::const_reference_type>;
  ///@}

  template <class From, class To>
  static inline To& assign_(From&& from, To&& to) noexcept {
    boost::fusion::for_each(from, map_assigner<To>(to));
    return to;
  }

 public:
  /// \name Iterator traits
  ///@{
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = typename container::value_type;
  using pointer = std::conditional_t
      <!is_const, typename container::pointer_type,
       typename container::const_pointer_type>;

  struct reference : reference_map {
    reference(const reference& r) noexcept : reference_map(r) {};
    reference(const reference_map& r) noexcept : reference_map(r) {};
    reference(const value_type& v) noexcept : reference_map(ref_from_val(v)) {}
    reference(reference&& r) noexcept : reference_map(std::move(r)) {};
    reference(reference_map&& r) noexcept : reference_map(std::move(r)) {};
    reference(value_type& v) noexcept : reference_map(ref_from_val(v)) {}

    auto operator=(const value_type& other) RETURNS(assign_(other, *this));
    auto operator=(const reference_map& other) RETURNS(assign_(other, *this));
    auto operator=(const reference& other) RETURNS(assign_(other, *this));
    auto operator=(value_type&& other)
        RETURNS(assign_(std::move(other), *this));
    auto operator=(reference_map&& other)
        RETURNS(assign_(std::move(other), *this));
    auto operator=(reference&& other) RETURNS(assign_(std::move(other), *this));
  };
  ///@}

  /// DefaultConstructible/CopyConstructible/MoveConstructible
  /// @{
  vector_iterator_base() = default;
  vector_iterator_base(iterator_map&& it) noexcept : it_(std::move(it)) {}
  vector_iterator_base(iterator_map const& it) noexcept : it_(it) {}
  vector_iterator_base(vector_iterator_base&& it) noexcept
      : it_(std::move(it.it_)) {}
  vector_iterator_base(vector_iterator_base const& it) noexcept : it_(it.it_) {}

  vector_iterator_base& operator=(iterator_map const& it) noexcept {
    it_ = it;
    return *this;
  }
  vector_iterator_base& operator=(vector_iterator_base const& it) noexcept {
    it_ = it.it_;
    return *this;
  }
  vector_iterator_base& operator=(iterator_map&& it) noexcept {
    it_ = std::move(it);
    return *this;
  }
  vector_iterator_base& operator=(vector_iterator_base&& it) noexcept {
    it_ = std::move(it.it_);
    return *this;
  }
  ///@}

  /// \name Comparison operators (==, !=, <, >, <=, >=)
  ///@{
  friend inline bool operator==(const This& l, const This& r) noexcept {
    return boost::fusion::all(boost::fusion::zip(l.it_, r.it_), Eq());
  }
  friend inline bool operator<=(const This& l, const This& r) noexcept {
    return boost::fusion::all(boost::fusion::zip(l.it_, r.it_), LEq());
  }
  friend inline bool operator>=(const This& l, const This& r) noexcept {
    return boost::fusion::all(boost::fusion::zip(l.it_, r.it_), GEq());
  }

  friend inline bool operator!=(const This& l, const This& r) noexcept {
    return !(l == r);
  }
  friend inline bool operator<(const This& l, const This& r) noexcept {
    return !(l >= r);
  }
  friend inline bool operator>(const This& l, const This& r) noexcept {
    return !(l <= r);
  }
  ///@}

  /// \name Traversal operators (++, +=, +, --, -=, -)
  ///@{
  inline This& operator++() noexcept {
    it_ = boost::fusion::transform(it_, AdvFwd());
    return *this;
  }
  inline This operator++(int) noexcept {
    const auto it = *this;
    ++(*this);
    return it;
  }
  inline This& operator+=(const difference_type value) noexcept {
    it_ = boost::fusion::transform(it_, Increment(value));
    return *this;
  }
  inline This operator--(int) noexcept {
    const auto it = *this;
    --(*this);
    return it;
  }
  inline This& operator-=(const difference_type value) noexcept {
    it_ = boost::fusion::transform(it_, Decrement(value));
    return *this;
  }
  inline This& operator--() noexcept {
    it_ = boost::fusion::transform(it_, AdvBwd());
    return *this;
  }

  friend inline This operator-(This a, const difference_type value) noexcept {
    return a -= value;
  }
  friend inline This operator+(This a, const difference_type value) noexcept {
    return a += value;
  }

  friend inline difference_type operator-(const This l, const This r) noexcept {
    const auto result = boost::fusion::at_c<0>(l.it_).second
                        - boost::fusion::at_c<0>(r.it_).second;

    assert(boost::fusion::accumulate(boost::fusion::zip(l.it_, r.it_), result,
                                     check_distance()));
    return result;
  }
  ///@}

  /// \name Access operators
  ///@{
  inline reference operator*() noexcept { return ref_from_it(it_); }
  inline reference operator*() const noexcept { return ref_from_it(it_); }
  inline reference operator->() noexcept { return *(*this); }
  inline reference operator[](const difference_type v) noexcept {
    return *boost::fusion::transform(it_, Increment(v));
  }
  ///@}

 private:
  iterator_map it_;  ///< Tuple of iterators over column types

  /// \name Wrappers
  ///@{

  /// \brief Equality
  struct Eq {
    template <class T> inline bool operator()(T&& i) const noexcept {
      return boost::fusion::at_c<0>(i).second == boost::fusion::at_c
                                                 <1>(i).second;
    }
  };

  /// \brief Less Equal Than
  struct LEq {
    template <class T> inline bool operator()(T&& i) const noexcept {
      return boost::fusion::at_c<0>(i).second <= boost::fusion::at_c
                                                 <1>(i).second;
    }
  };

  /// \brief Greater Equal Than
  struct GEq {
    template <class T> inline bool operator()(T&& i) const noexcept {
      return boost::fusion::at_c<0>(i).second >= boost::fusion::at_c
                                                 <1>(i).second;
    }
  };

  /// \brief Advance forward
  struct AdvFwd {
    template <class T> inline auto operator()(T i) const noexcept {
      return i.second = ++(i.second);
    }
  };

  /// \brief Advance backward
  struct AdvBwd {
    template <class T> inline auto operator()(T i) const noexcept {
      return i.second = --(i.second);
    }
  };

  /// \brief Increment by value
  struct Increment {
    Increment(const difference_type value) noexcept : value_(value) {}
    template <class T>
    inline std::remove_reference_t<T> operator()(T&& i) const noexcept {
      return {i.second + value_};
    }
    const difference_type value_;
  };

  /// \brief Decrent by value
  struct Decrement {
    Decrement(const difference_type value) : value_(value) {}
    template <class T>
    inline std::remove_reference_t<T> operator()(T&& i) const noexcept {
      return {i.second - value_};
    }
    const difference_type value_;
  };

  struct check_distance {
    template <class T, class U>
    inline T operator()(const T acc, const U i) const noexcept {
      const auto tmp = boost::fusion::at_c<0>(i).second - boost::fusion::at_c
                                                          <1>(i).second;
      if (!(acc == tmp)) {
        std::cerr << "column types have different sizes!!\n";
        assert(acc == tmp);
      }
      return tmp;
    }
  };

  ///@}

  template <class U> struct val_to_ref {
    val_to_ref(U& v_) : v(v_) {}
    template <class T> inline auto operator()(const T&) const noexcept {
      using key = typename T::first_type;
      using value = typename T::second_type;
      return boost::fusion::make_pair<key, std::add_lvalue_reference_t<value>>(
          boost::fusion::at_key<key>(v));
    }
    U& v;
  };

  template <class U> struct it_to_ref {
    it_to_ref(U v_) : i(v_) {}
    template <class T> inline auto operator()(const T&) const noexcept {
      using key = typename T::first_type;
      using value = std::remove_reference_t
          <decltype(*typename T::second_type())>;
      return boost::fusion::make_pair<key, std::add_lvalue_reference_t<value>>(
          *boost::fusion::at_key<key>(i));
    }
    U i;
  };

 public:
  static inline reference_map ref_from_val(value_type& v) noexcept {
    return boost::fusion::transform(v, val_to_ref<value_type>{v});
  }
  static inline reference_map ref_from_it(iterator_map& v) noexcept {
    return boost::fusion::transform(v, it_to_ref<iterator_map>{v});
  }
  static inline reference_map ref_from_it(const iterator_map& v) noexcept {
    return boost::fusion::transform(v, it_to_ref<iterator_map>{v});
  }
};

}  // namespace detail

}  // namespace scattered
#endif  // SCATTERED_DETAIL_VECTOR_ITERATOR_BASE_HPP
