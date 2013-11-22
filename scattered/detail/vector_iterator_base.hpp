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
#include "assert.hpp"
#include "returns.hpp"
#include "map_mutation.hpp"
#include "container_traits.hpp"
#include "unqualified.hpp"
#include "get.hpp"

namespace scattered {

namespace detail {

/// \brief RandomAccessIterator for scattered::vector container
/// If const_tag is const -> const_iterator, otherwise -> iterator
template <bool is_const_, class column_types, class column_tags,
          template <class> class container_type, class original_type>
class vector_iterator_base {
 public:
  /// \name Utility aliases
  ///@{
  static const constexpr bool is_const = is_const_;
  using types = column_types;
  using tags = column_tags;
  using container = container_traits<types, tags, container_type>;
  using This = vector_iterator_base
      <is_const, types, tags, container_type, original_type>;
  using const_This = vector_iterator_base
      <true, types, tags, container_type, original_type>;
  friend const_This;
  using non_const_This = vector_iterator_base
      <false, types, tags, container_type, original_type>;
  friend non_const_This;
  using original_value_type = original_type;
  ///@}

  /// \name Map types
  ///@{
  using iterator_map = std::conditional_t
      <!is_const, typename container::iterator_type,
       typename container::const_iterator_type>;
  using const_iterator_map = typename const_This::iterator_map;
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


  /// \name REFACTOR THIS \todo refactor this
  ///@{
  using MemberMap = typename detail::as_fusion_map<original_value_type>::type;

  template <class P> struct key_of {
    using type = typename detail::unqualified_t<P>::first_type;
  };
  template <class P> using key_of_t = typename key_of<P>::type;


  static value_type from_type(original_value_type& value) {
    value_type tmp;
    boost::fusion::for_each(boost::fusion::zip(MemberMap{}, value),
                            [&](auto&& i) {
      using key = key_of_t<decltype(boost::fusion::at_c<0>(i))>;
      get<key>(tmp) = boost::fusion::at_c<1>(i);
    });
    return tmp;
  }
  ///@}

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

    /// \todo this should take a reference
    auto operator=(original_value_type other) RETURNS(assign_(from_type(other), *this));

    /// \todo refactor this
    static original_value_type to_type(reference ref) {
      original_value_type tmp;
      boost::fusion::for_each(ref, [&](auto&& i) {
          using key = key_of_t<decltype(i)>;
          get<key>(tmp) = i.second;
        });
      return tmp;
    }

    operator original_value_type() {
      return to_type(*this);
    }

    template <class L, class R>
    friend inline bool operator==(L l, R r) noexcept {
      return boost::fusion::all(boost::fusion::zip(l, r), Eq());
    }
    template <class L, class R>
    friend inline bool operator<=(L l, R r) noexcept {
      return boost::fusion::all(boost::fusion::zip(l, r), LEq());
    }
    template <class L, class R>
    friend inline bool operator>=(L l, R r) noexcept {
      return boost::fusion::all(boost::fusion::zip(l, r), GEq());
    }
    template <class L, class R>
    friend inline bool operator!=(L l, R r) noexcept {
      return !(l == r);
    }
    template <class L, class R>
    friend inline bool operator<(L l, R r) noexcept {
      return !(l >= r);
    }
    template <class L, class R>
    friend inline bool operator>(L l, R r) noexcept {
      return !(l <= r);
    }
  };
  ///@}

  /// DefaultConstructible/CopyConstructible/MoveConstructible
  /// @{
 private:
  template <class T> static constexpr bool is_This_is_const_and_T_not() {
    return std::is_same<detail::unqualified_t<T>, non_const_This>::value
           && std::is_same<This, const_This>::value;
  }

 public:
  vector_iterator_base() = default;
  vector_iterator_base(This const& it) noexcept : it_(it.it_) {}
  template <class T, std::enable_if_t<is_This_is_const_and_T_not<T>(), int> = 0>
  vector_iterator_base(T const& it) noexcept : it_(it.it_) {}
  This& operator=(This const& it) noexcept {
    it_ = it.it_;
    return *this;
  }
  template <class T, std::enable_if_t<is_This_is_const_and_T_not<T>(), int> = 0>
  This& operator=(T const& it) noexcept {
    it_ = it.it_;
    return *this;
  }

  vector_iterator_base(This&& it) noexcept : it_(std::move(it.it_)) {}
  template <class T, std::enable_if_t<is_This_is_const_and_T_not<T>(), int> = 0>
  vector_iterator_base(T&& it) noexcept : it_(std::move(it.it_)) {}
  template <class T, std::enable_if_t<is_This_is_const_and_T_not<T>(), int> = 0>
  This& operator=(T&& it) noexcept {
    it_ = std::move(it.it_);
    return *this;
  }
  This& operator=(This&& it) noexcept {
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

    ASSERT(boost::fusion::accumulate(boost::fusion::zip(l.it_, r.it_), result,
                                     check_distance()) == result,
           "column types have different sizes");
    return result;
  }
  ///@}

  /// \name Access operators
  ///@{
  inline reference operator*() noexcept { return ref_from_it(it_); }
  inline reference operator*() const noexcept { return ref_from_it(it_); }
  inline reference operator->() noexcept { return *(*this); }
  inline reference operator[](const difference_type v) noexcept {
    return ref_from_it(boost::fusion::transform(it_, Increment(v)));
  }
  ///@}

 private:
  iterator_map it_;  ///< Tuple of iterators over column types

  /// \name Wrappers
  ///@{

  /// \brief Equality
  struct Eq {
    template <class T>
    inline auto operator()(T&& i) const
        noexcept -> decltype(
                       std::enable_if_t
                       <!std::is_floating_point
                        <detail::unqualified_t
                         <decltype(boost::fusion::at_c<0>(i).second)>>::value,
                        bool>()) {
      using type0 = detail::unqualified_t
          <decltype(boost::fusion::at_c<0>(i).second)>;
      using type1 = detail::unqualified_t
          <decltype(boost::fusion::at_c<1>(i).second)>;
      static_assert(std::is_same<type0, type1>::value,
                    "Expecting the same type - non_fp_overload");
      return boost::fusion::at_c<0>(i).second == boost::fusion::at_c
                                                 <1>(i).second;
    }
    template <class T>
    inline auto operator()(T&& i) const
        noexcept -> decltype(
                       std::enable_if_t
                       <std::is_floating_point
                        <detail::unqualified_t
                         <decltype(boost::fusion::at_c<0>(i).second)>>::value,
                        bool>()) {
      using type0 = detail::unqualified_t
          <decltype(boost::fusion::at_c<0>(i).second)>;
      using type1 = detail::unqualified_t
          <decltype(boost::fusion::at_c<1>(i).second)>;
      static_assert(std::is_same<type0, type1>::value,
                    "Expecting the same type - fp_overload");
      return std::abs(boost::fusion::at_c<0>(i).second
                      - boost::fusion::at_c<1>(i).second) < std::numeric_limits
             <type0>::epsilon();
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
    inline detail::unqualified_t<T> operator()(T&& i) const noexcept {
      return {i.second + value_};
    }
    const difference_type value_;
  };

  /// \brief Decrent by value
  struct Decrement {
    Decrement(const difference_type value) : value_(value) {}
    template <class T>
    inline detail::unqualified_t<T> operator()(T&& i) const noexcept {
      return {i.second - value_};
    }
    const difference_type value_;
  };

  struct check_distance {
    template <class T, class U>
    inline T operator()(const T acc, const U i) const noexcept {
      const auto tmp = boost::fusion::at_c<0>(i).second - boost::fusion::at_c
                                                          <1>(i).second;
      ASSERT(acc == tmp, "column types have different sizes!");
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
      using value = detail::unqualified_t<decltype(*typename T::second_type())>;
      return boost::fusion::make_pair<key, std::add_lvalue_reference_t<value>>(
          const_cast<value&>(*boost::fusion::at_key<key>(i)));
    }
    U i;
  };

  template <class U> struct it_to_cit {
    it_to_cit(U v_) : i(v_) {}
    template <class T> inline auto operator()(const T&) const noexcept {
      using key = typename T::first_type;
      using value = detail::unqualified_t<decltype(typename T::second_type())>;
      return boost::fusion::make_pair
          <key, typename container::template const_iterator
           <typename value::value_type>::type>(boost::fusion::at_key<key>(i));
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

  static inline const_This cit_from_it(const This& v) noexcept {
    return const_This::from_map(
        boost::fusion::transform(v.it_, it_to_cit<iterator_map>{v.it_}));
  }

  static vector_iterator_base from_map(iterator_map it) {
    This tmp;
    tmp.it_ = it;
    return tmp;
  }

  auto it() RETURNS(it_);
  auto it() const RETURNS(it_);
};

}  // namespace detail

}  // namespace scattered

#endif  // SCATTERED_DETAIL_VECTOR_ITERATOR_BASE_HPP
