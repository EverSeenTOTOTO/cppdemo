#ifndef TYPE_LIST_H
#define TYPE_LIST_H
#include <cstddef>
#include <type_traits>

struct never {};

// 连接两个列表
template <typename, typename>
struct concat;

template <template <typename...> typename L, typename... Args1, typename... Args2>
struct concat<L<Args1...>, L<Args2...>> {
  using type = L<Args1..., Args2...>;
};

// 切片，从位置0开始取length个元素
template <typename L, size_t length>
struct slice_helper;

template <template <typename...> typename L, size_t length, typename... Args>
struct slice_helper<L<Args...>, length> {
  typedef L<Args...> List;
  using type = typename concat<typename std::conditional<std::is_same<typename List::first, never>::value,  // type_list<>
                                                         L<>, L<typename List::first>>::type,
                               typename slice_helper<typename List::rest, length - 1>::type>::type;
};

template <template <typename...> typename L, typename... Args>
struct slice_helper<L<Args...>, 0> {
  using type = L<>;
};

// 切片，从start开始，取length个，方法是先取[start, ...（到list尾部）]再利用slice_helper取[start, ...（length个）]
template <typename L, size_t start, size_t length>
struct slice {
  using type = typename slice<typename L::rest, start - 1, length>::type;
};

template <typename L, size_t length>
struct slice<L, 0, length> {
  using type = typename slice_helper<L, length>::type;
};

// 根据索引取列表元素
template <typename L, size_t index>
struct at {
  using type = typename slice<L, index, 1>::type::first;
};

// 根据元素查索引，不存在返回-1
template <typename, typename>
struct index_of;

template <template <typename...> class L, typename T, typename... Args>
struct index_of<L<Args...>, T> {
  typedef L<Args...> List;
  static constexpr int value = std::is_same<T, typename List::first>::value ? 0 : index_of<typename List::rest, T>::value == -1 ? -1 : 1 + index_of<typename List::rest, T>::value;
};

template <template <typename...> class L, typename T>
struct index_of<L<>, T> {
  static const int value = -1;
};

// 在位置index，插入若干个元素Ts...
template <typename L, size_t index, typename... Ts>
struct insert_at;

template <template <typename...> class L, size_t index, typename... Args, typename... Ts>
struct insert_at<L<Args...>, index, Ts...> {
  typedef L<Args...> List;
  using type = typename concat<typename concat<typename slice<List, 0, index>::type, L<Ts...>>::type, typename slice<List, index, List::length - index>::type>::type;
};

// 在位置index，删除length个元素
template <typename L, size_t index, size_t length>
struct remove_at;

template <template <typename...> class L, size_t index, size_t length, typename... Args>
struct remove_at<L<Args...>, index, length> {
  typedef L<Args...> List;
  using type = typename concat<typename slice<List, 0, index>::type, typename slice<List, index + length, List::length - index - length>::type  // 感觉怪怪的
                               >::type;
};

template <template <typename...> class L, size_t index, typename... Args>
struct remove_at<L<Args...>, index, 0> {
  using type = L<Args...>;
};

// 去除指定类型，无论它出现多少次
template <typename, typename>
struct remove_type;

template <template <typename...> class L, typename T, typename... Args>
struct remove_type<L<Args...>, T> {
  typedef L<Args...> List;
  using type = typename std::conditional<std::is_same<T, typename List::first>::value, typename remove_type<typename List::rest, T>::type, typename insert_at<typename remove_type<typename List::rest, T>::type, 0, typename List::first>::type>::type;
};

template <template <typename...> class L, typename T>
struct remove_type<L<>, T> {
  using type = L<>;
};

// 去除重复类型
template <typename>
struct remove_duplicate;

template <template <typename...> class L, typename... Args>
struct remove_duplicate<L<Args...>> {
  typedef L<Args...> List;
  typedef typename List::first First;
  using type = typename insert_at<typename remove_duplicate<typename remove_type<typename List::rest, First>::type>::type, 0, First>::type;
};

template <template <typename...> class L>
struct remove_duplicate<L<>> {
  using type = L<>;
};

// 类型列表
template <typename... T>
struct type_list;

template <typename First, typename... T>
struct type_list<First, T...> {
  static constexpr size_t length = 1 + type_list<T...>::length;

  using type = type_list<First, T...>;
  using first = First;
  using rest = type_list<T...>;
};

template <>
struct type_list<> {
  static const size_t length = 0;

  using type = type_list<>;
  using first = never;
  using rest = type_list<>;
};

template <typename, typename>
struct map;

template <template <typename...> class L, template <typename> typename F, typename... Args, typename _>
struct map<L<Args...>, F<_>> {
  using List = L<Args...>;
  using type = std::conditional<List::length == 0, List, concat<L<typename F<typename List::first>::type>, typename map<typename List::rest, F<_>>::type>>;
};

#endif
