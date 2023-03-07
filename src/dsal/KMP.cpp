#include "./KMP.h"
#include <algorithm>
#include "../utils.h"

namespace dsal_impl {
  int next_naive_impl(std::string const& pattern, size_t index) {
    for (int i = index; i > 0; --i) {
      auto prefix = pattern.substr(0, i);
      auto suffix = pattern.substr(index - i + 1, i);

      if (prefix == suffix) {
        return i;
      }
    }

    return 0;
  }
}  // namespace dsal_impl

vec<int> const& next_naive(std::string const& pattern) {
  auto next = new vec<int>;

  for (auto i = 0; i < pattern.size(); ++i) {
    next->push_back(dsal_impl::next_naive_impl(pattern, i));
  }

  return *next;
}

void test_next_naive() {
  auto p    = "abc";
  auto next = next_naive(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 0}, "test_next_naive abc");

  p    = "abcdabc";
  next = next_naive(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 0, 0, 1, 2, 3}, "test_next_naive abcdabc");

  p    = "abcabdddabcabc";
  next = next_opt(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 0, 1, 2, 0, 0, 0, 1, 2, 3, 4, 5, 3},
            "test_next_naive abcabdddabcabc");

  // 2019 408 09
  p    = "abaabc";
  next = next_naive(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 1, 1, 2, 0}, "test_next_naive abaabc");
}

vec<int> const& next_opt(std::string const& pattern) {
  auto next = new vec<int>{0};

  auto i     = 0;  // next[index - 1]
  auto index = 1;

  while (index < pattern.size()) {
    if (pattern[index] == pattern[i]) {  // 前（后）缀长度L增加1
      index += 1;
      i += 1;
      next->push_back(i);
    } else if (i > 0) {  // 缩小前缀和后缀的长度
      i = next->at(i - 1);
    } else {  // 从0~index-1，没有最长的前缀P[0, L]恰好与后缀P[index-L+1, index-1]相同
      next->push_back(0);
      index++;
    }
  }

  return *next;
}

void test_next_opt() {
  auto p    = "abc";
  auto next = next_opt(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 0}, "test_next_opt abc");

  p    = "abcdabc";
  next = next_opt(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 0, 0, 1, 2, 3}, "test_next_opt abcdabc");

  p    = "abcabdddabcabc";
  next = next_opt(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 0, 1, 2, 0, 0, 0, 1, 2, 3, 4, 5, 3},
            "test_next_opt abcabdddabcabc");

  // 2019 408 09
  p    = "abaabc";
  next = next_opt(p);

  print(next);
  expect_eq(next, vec<int>{0, 0, 1, 1, 2, 0}, "test_next_opt abaabc");
}
