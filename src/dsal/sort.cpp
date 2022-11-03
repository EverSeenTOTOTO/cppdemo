#include "./sort.h"

void test_slice() {
  using namespace std;

  auto v = vec<int>{1, 2, 3};

  expect_eq(slice(v, 0, 1), vec<int>{1}, "test slice 0 1");
  expect_eq(slice(v, 0, 9), vec<int>{1, 2, 3}, "test slice 0 9");
  expect_eq(slice(v, 2, 9), vec<int>{3}, "test slice 2 9");
  expect_eq(slice(v, 4, 0), vec<int>{}, "test slice 4 0");
}

static vec<int> demo{97, 78, 65, 49, 49, 13, 27, 38};
static vec<int> result{13, 27, 38, 49, 49, 65, 78, 97};

void insert_sort(vec<int>& s) {
  for (auto i = 1; i < s.size(); ++i) {
    if (s[i] < s[i - 1]) {
      int temp = s[i];
      s[i]     = s[i - 1];  // right shift s[i-1]

      // find insert place
      auto j = i - 2;
      for (; j >= 0 && s[j] > temp; --j) {
        s[j + 1] = s[j];  // right shift s[j]
      }
      // insert
      s[j + 1] = temp;
    }
  }
}

void test_insert_sort() {
  using namespace std;

  auto s = range(5, 0);

  insert_sort(s);
  expect_eq(s, range(1, 6), "test insert_sort");

  auto clone = slice(demo, 0, demo.size());

  insert_sort(clone);
  expect_eq(clone, result, "test insert_sort demo");
}

void bubble_sort(vec<int>& s) {
  for (auto i = 0; i < s.size(); ++i) {
    for (auto j = i + 1; j < s.size(); ++j) {
      if (s[i] > s[j]) {
        swap(s, i, j);
      }
    }
  }
}

void test_bubble_sort() {
  using namespace std;

  auto s = range(5, 0);

  bubble_sort(s);
  expect_eq(s, range(1, 6), "test bubble_sort");

  auto clone = slice(demo, 0, demo.size());

  bubble_sort(clone);
  expect_eq(clone, result, "test bubble_sort demo");
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
  expect_eq(s, range(1, 6), "test quick_sort");

  auto clone = slice(demo, 0, demo.size());

  quick_sort(clone);
  expect_eq(clone, result, "test quick_sort demo");
}

// 大根堆
void adjust_heap(vec<int>& h, int root, int len) {
  if (root >= len) return;

  auto lidx   = 2 * root;
  auto ridx   = lidx + 1;
  auto lchild = lidx >= len ? -1 : h[lidx];
  auto rchild = ridx >= len ? -1 : h[ridx];

  if (h[root] < std::max(lchild, rchild)) {
    if (lchild > rchild) {
      swap(h, root, lidx);
      adjust_heap(h, lidx, len);
    } else {
      swap(h, root, ridx);
      adjust_heap(h, ridx, len);
    }
  }
}

void heap_sort(vec<int>& s) {
  for (auto i = std::floor(s.size() / 2) - 1; i >= 0; --i) {
    adjust_heap(s, i, s.size() - 1);
  }

  for (auto i = s.size() - 1; i > 0; --i) {
    swap(s, 0, i);
    adjust_heap(s, 0, i);
  }
}

void test_heap_sort() {
  using namespace std;

  auto s = range(5, 0);

  heap_sort(s);
  expect_eq(s, range(1, 6), "test heap_sort");

  auto clone = slice(demo, 0, demo.size());

  heap_sort(clone);
  expect_eq(clone, result, "test heap_sort demo");
}

template <typename T>
using listvec = vec<std::list<T>>;

void distribute(vec<int>& s, listvec<int>& v, int r) {
  auto radix = std::pow(10, r - 1);

  for (auto x : s) {
    auto num = (int) std::floor((x / radix)) % 10;

    // x < radix can be ignored, except 0 when radix = 1
    if (num == 0 && x != 0) continue;

    v.at(num).push_back(x);
  }
}

// combine individual lists into vec
bool collect(vec<int>& s, listvec<int>& v) {
  auto i       = 0;
  auto changed = false;

  for (auto& l : v) {
    if (!l.empty()) {
      changed = true;

      for (auto const& x : l) {
        s[i++] = x;
      }

      l.clear();
    }
  }

  return changed;
}

void radix_sort(vec<int>& s) {
  listvec<int> v(10);  // 0~9

  int radix = 1;
  while (true) {
    distribute(s, v, radix++);
    if (!collect(s, v)) break;
  }
}

void test_radix_sort() {
  using namespace std;

  auto s = range(5, 0);

  radix_sort(s);
  expect_eq(s, range(1, 6), "test radix_sort");

  auto clone = slice(demo, 0, demo.size());

  radix_sort(clone);
  expect_eq(clone, result, "test radix_sort demo");
}
