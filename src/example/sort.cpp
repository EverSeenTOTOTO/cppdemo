#include "./sort.h"

void test_slice() {
  using namespace std;

  auto v = vec<int>{1, 2, 3};

  expect_eq(slice(v, 0, 1), vec<int>{1}, "test slice 0 1");
  expect_eq(slice(v, 0, 9), vec<int>{1, 2, 3}, "test slice 0 9");
  expect_eq(slice(v, 2, 9), vec<int>{3}, "test slice 2 9");
  expect_eq(slice(v, 4, 0), vec<int>{}, "test slice 4 0");
}

void bubble_sort(vec<int>& s) {
  for (auto i = 0; i < s.size(); ++i) {
    for (auto j = i + 1; j < s.size(); ++j) {
      if (s[i] > s[j]) { swap(s, i, j); }
    }
  }
}

void test_bubble_sort() {
  using namespace std;

  auto s = range(5, 0);

  bubble_sort(s);
  display(s);

  expect_eq(s, range(1, 6), "test bubble_sort");
}

int partition(vec<int>& s, int begin, int end, int pivot_offset) {
  int r = end;

  swap(s, begin + pivot_offset, r);

  int i = begin;
  while (i < r) {
    if (s[i] > s[r]) {
      swap(s, r - 1, i);
      swap(s, r - 1, r);
      r = r - 1;
    } else {
      i = i + 1;
    }
  }

  return r;
}

void quick_sort_helper(vec<int>& s, int begin, int end) {
  if (end - begin >= 1) {
    int r = partition(s, begin, end, (end - begin) / 2);  // set the mid element as pivot

    quick_sort_helper(s, begin, r - 1);
    quick_sort_helper(s, r + 1, end);
  }
}

void quick_sort(vec<int>& s) {
  return quick_sort_helper(s, 0, s.size() - 1);
}

void test_quick_sort() {
  using namespace std;

  auto s = range(5, 0);

  quick_sort(s);
  display(s);

  expect_eq(s, range(1, 6), "test quick_sort");
}

void quick_sort_parallel_helper(vec<int>& s, int begin, int end) {
  if (end - begin >= 1) {
    int r = partition(s, begin, end, 0);  // set the first element as pivot

    auto p = std::async(std::launch::async, [&]() { quick_sort_helper(s, begin, r - 1); });

    quick_sort_helper(s, r + 1, end);
    p.wait();
  }
}

void quick_sort_parallel(vec<int>& v) {
  return quick_sort_parallel_helper(v, 0, v.size() - 1);
}

void test_quick_sort_parallel() {
  timer::reset();

  auto s = range(5e6, 0);

  timer::report("sync");
  quick_sort(s);
  expect_eq(s[0], 1, "test quick_sort sync");
  timer::report("sync done");
  timer::report("async");

  s = range(5e6, 0);

  quick_sort_parallel(s);
  expect_eq(s[0], 1, "test quick_sort async");
  timer::report("async done");
}
