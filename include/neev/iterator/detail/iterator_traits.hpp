// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE.txt
// 
// (C) Copyright 2014 Pierre Talbot <ptalbot@hyc.io>

#include <boost/type_traits.hpp>
#include <boost/iterator/detail/minimum_category.hpp>

namespace neev{
namespace detail{

#define GENERATE_HAS_MEMBER(M)                                          \
template<class T>                                                       \
class has_member_##M                                                    \
{                                                                       \
  struct fallback { int X; };                                           \
  struct derived : T, fallback { };                                     \
                                                                        \
  typedef char yes[1];                                                  \
  typedef char no[2];                                                   \
  template <typename U, U> struct check;                                \
  template<typename U> static no& has_member(check<int fallback::*, &U::M>*);   \
  template<typename> static yes& has_member(...);                       \
public:                                                                 \
  typedef has_member_##M type;                                          \
  enum { value = sizeof(has_member<derived>(0)) == sizeof(yes) };       \
};

#define GENERATE_HAS_TYPE(MT)                                           \
template<class T>                                                       \
class has_type_##MT                                                     \
{                                                                       \
  typedef char yes[1];                                                  \
  typedef char no[2];                                                   \
  template<typename U> static yes& has_type(typename U::MT*);           \
  template<typename> static no& has_type(...);                          \
public:                                                                 \
  typedef has_type_##MT type;                                           \
  enum { value = sizeof(has_type<T>(0)) == sizeof(yes) };               \
};


GENERATE_HAS_MEMBER(begin)
GENERATE_HAS_MEMBER(end)
GENERATE_HAS_TYPE(iterator)
GENERATE_HAS_TYPE(const_iterator)

template <class T, bool fundamental = boost::is_fundamental<T>::value>
struct is_iterable
: boost::integral_constant<bool,
    has_member_begin<T>::value &&
    has_member_end<T>::value &&
    boost::conditional<
      boost::is_const<T>::value,
      has_type_const_iterator<T>,
      has_type_iterator<T>
    >::type::value
>
{};

template <class T>
struct is_iterable<T, true>
: boost::false_type
{};

template <class T, bool iterable = is_iterable<T>::value>
struct iterable_nested_depth
: boost::integral_constant<std::size_t,
    iterable_nested_depth<
      typename boost::conditional<
        boost::is_const<T>::value,
        typename T::const_iterator::value_type,
        typename T::iterator::value_type
      >::type
    >::value + 1
>
{};

template <class T>
struct iterable_nested_depth<T, false>
: boost::integral_constant<std::size_t, 0>
{};

template <class Iterator>
struct iterator_nested_depth
: iterable_nested_depth<typename Iterator::value_type>
{};

template <class Iterator>
class inner_iterator
{
  typedef typename Iterator::value_type value_type;
public:
  typedef typename boost::conditional<
    boost::is_const<value_type>::value,
    typename value_type::const_iterator,
    typename value_type::iterator
  >::type type;
};

template <
  class Iterator, 
  std::size_t depth=iterator_nested_depth<Iterator>::value
>
struct innermost_iterator
: innermost_iterator<typename inner_iterator<Iterator>::type, depth-1>
{};

template <class Iterator>
struct innermost_iterator<Iterator, 0>
{
  typedef Iterator type;
};

template <
  class Iterator,
  std::size_t depth=iterator_nested_depth<Iterator>::value
>
struct innermost_iterator_value_type
{
  typedef typename 
    innermost_iterator<Iterator, depth>::type::value_type type;
};

template <
  class Iterator,
  std::size_t depth,
  class MinimumTraversalTag
>
struct minimum_nested_iterator_category_impl
{
private:
  typedef typename boost::iterator_traversal<Iterator>::type traversal_tag;
public:
  typedef typename minimum_nested_iterator_category_impl<
    typename inner_iterator<Iterator>::type,
    depth - 1,
    typename boost::detail::minimum_category<traversal_tag, MinimumTraversalTag>::type
  >::type type;
};

template <
  class Iterator,
  class MinimumTraversalTag
>
struct minimum_nested_iterator_category_impl<Iterator, 0, MinimumTraversalTag>
{
  typedef MinimumTraversalTag type;
};

template <
  class Iterator,
  std::size_t depth=iterator_nested_depth<Iterator>::value,
  class MinimumTraversalTag = typename boost::iterator_traversal<Iterator>::type
>
struct minimum_nested_iterator_category
: minimum_nested_iterator_category_impl<
    Iterator,
    depth,
    MinimumTraversalTag
>
{};

}} // namespace neev::detail