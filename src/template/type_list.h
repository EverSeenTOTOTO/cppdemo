#ifndef TYPE_LIST_H
#define TYPE_LIST_H
#include <type_traits>

struct null_type {};

// 连接两个列表
template<typename, typename>
struct Concat;

template<template<typename...> typename L, typename... Args1, typename... Args2>
struct Concat<L<Args1...>, L<Args2...>> {
  using type = L<Args1..., Args2...>;
};

// 切片，从位置0开始取length个元素
template<typename L, size_t length>
struct SliceHelper;

template<template<typename...> typename L, size_t length, typename... Args>
struct SliceHelper<L<Args...>, length>  {
  typedef L<Args...> List;
  using type = Concat<
    typename std::conditional<
      std::is_same<typename List::first, null_type>::value, // TypeList<>
      L<>,
      L<typename List::first>
    >::type,
    typename SliceHelper<typename List::rest, length - 1>::type
  >::type;
};

template<template<typename...> typename L, typename... Args>
struct SliceHelper<L<Args...>, 0> {
  using type = L<>;
};

// 切片，从index开始，取length个，方法是先取[start, ...（到list尾部）]再利用SliceHelper取[start, ...（length个）]
template<typename L, size_t start, size_t length>
struct Slice {
  using type = Slice<typename L::rest, start - 1, length>::type;
};

template<typename L, size_t length>
struct Slice<L, 0, length> {
  using type = SliceHelper<L, length>::type;
};

// 根据索引取列表元素
template<typename L, size_t index>
struct At {
  using type = Slice<L, index, 1>::type::first;
};

// 根据元素查索引，不存在返回-1
template<typename, typename>
struct IndexOf;

template<template<typename...> class L, typename T, typename... Args>
struct IndexOf<L<Args...>, T> {
  typedef L<Args...> List;
  static constexpr int value = std::is_same<T, typename List::first>::value
    ? 0
    : IndexOf<typename List::rest, T>::value == -1
      ? -1
      : 1 + IndexOf<typename List::rest, T>::value;
};

template<template<typename...> class L, typename T>
struct IndexOf<L<>, T> {
  static const int value = -1;
};

// 在位置index，插入若干个元素Ts...
template<typename L, size_t index, typename... Ts>
struct InsertAt;

template<template<typename...> class L, size_t index, typename ...Args, typename... Ts>
struct InsertAt<L<Args...>, index, Ts...> {
  typedef L<Args...> List;
  using type = Concat<
    typename Concat<
      typename Slice<List, 0, index>::type,
      L<Ts...>
    >::type,
    typename Slice<List, index, List::length - index>::type
  >::type;
};

// 在位置index，删除length个元素
template<typename L, size_t index, size_t length>
struct RemoveAt;

template<template<typename...> class L, size_t index, size_t length, typename ...Args>
struct RemoveAt<L<Args...>, index, length> {
  typedef L<Args...> List;
  using type = Concat<
    typename Slice<List, 0, index>::type,
    typename Slice<List, index + length, List::length - index - length>::type // 感觉怪怪的
  >::type;
};

template<template<typename...> class L, size_t index, typename ...Args>
struct RemoveAt<L<Args...>, index, 0> {
  using type = L<Args...>;
};

// 去除指定类型，无论它出现多少次
template<typename, typename>
struct RemoveType;

template<template<typename...> class L, typename T, typename... Args>
struct RemoveType<L<Args...>, T> {
  typedef L<Args...> List;
  using type = std::conditional<
    std::is_same<T, typename List::first>::value,
    typename RemoveType<typename List::rest, T>::type,
    typename InsertAt<
      typename RemoveType<typename List::rest, T>::type,
      0,
      typename List::first
    >::type
  >::type;
};

template<template<typename...> class L, typename T>
struct RemoveType<L<>, T>{
  using type = L<>;
};

// 去除重复类型
template<typename>
struct RemoveDuplicate;

template<template<typename...> class L, typename... Args>
struct RemoveDuplicate<L<Args...>> {
  typedef L<Args...> List;
  typedef typename List::first First;
  using type = InsertAt<
    typename RemoveDuplicate<typename RemoveType<typename List::rest, First>::type>::type,
    0,
    First
  >::type;
};

template<template<typename...> class L>
struct RemoveDuplicate<L<>> {
  using type = L<>;
};

// 类型列表
template<typename ...T>
struct TypeList;

template<typename First, typename ...T>
struct TypeList<First, T...> {

  static constexpr size_t length = 1 + TypeList<T...>::length;

  using type = TypeList<First, T...>;
  using first = First;
  using rest = TypeList<T...>;
};

template<>
struct TypeList<> {
  static const size_t length = 0;

  using type = TypeList<>;
  using first = null_type;
  using rest = TypeList<>;
};

#endif