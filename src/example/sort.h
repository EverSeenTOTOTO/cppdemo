#include "../common.h"
#include "../utils.h"

template <typename T>
vec<T>& slice(vec<T> const& vec, size_t begin, size_t length) {
  auto      bit = vec.begin();
  ::vec<T>* v   = new ::vec<T>;

  for (size_t i = begin; v->size() < length && bit + i < vec.end(); ++i) {
    v->push_back(*(bit + i));
  }

  return *v;
}
void test_slice();

void bubble_sort(vec<int>& s);
void test_bubble_sort();

void quick_sort(vec<int>& s);
void test_quick_sort();

void quick_sort_parallel(vec<int>& s);
void test_quick_sort_parallel();
