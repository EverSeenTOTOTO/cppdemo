#include "./stack_queue.h"
#include "../utils.h"

vec<int>& base_convertion(int number, size_t base) {
  std::stack<int> s;
  int             n = number;

  while (n != 0) {
    s.push(n % base);
    n = n / base;
  }

  auto v = new vec<int>;

  while (!s.empty()) {
    v->push_back(s.top());
    s.pop();
  }

  return *v;
}

void test_base_conversion() {
  // 1348D = 2504O
  expect_eq(base_convertion(1348, 8), vec<int>{2, 5, 0, 4}, "base_convertion 1348, 8");
  // 17D = 11H
  expect_eq(base_convertion(17, 16), vec<int>{1, 1}, "base_convertion 17, 16");
  // 17D = 10001B
  expect_eq(base_convertion(17, 2), vec<int>{1, 0, 0, 0, 1}, "base_convertion 17, 2");
}

void test_circular_deque() {
  circular_deque<int> q;

  expect_eq(q.empty(), true, "empty circular_deque.empty()");
  expect_eq(q.full(), true, "empty circular_deque.full()");
  expect_eq(q.len(), 0, "empty circular_deque.len()");
  expect_eq(q.max_len(), 0, "empty circular_deque.max_len()");

  circular_deque<int> q2{4};

  expect_eq(q2.max_len(), 4, "circular_deque.max_len()");

  q2.push_back(1);
  q2.push_back(2);

  expect_eq(q2.empty(), false, "circular_deque.empty()");
  expect_eq(q2.full(), false, "circular_deque.full()");
  expect_eq(q2.len(), 2, "circular_deque.len()");
  expect_eq(q2.pop_front(), 1, "circular_deque.pop_front()");

  q2.push_back(2);
  q2.push_back(3);
  q2.push_back(4);
  expect_eq(q2.len(), 4, "circular_deque.len()");
  expect_eq(q2.full(), true, "circular_deque.full()");

  try {
    q2.push_back(5);
  } catch (std::runtime_error e) {
    expect_match(e.what(), ".*full.*", "full circular_deque.push_back()");
  }

  for (auto i : range(0, q2.len())) {
    q2.pop_front();
  }

  try {
    q2.pop_front();
  } catch (std::runtime_error e) {
    expect_match(e.what(), ".*empty.*", "empty circular_deque.pop_front()");
  }

  q2.push_front(1);
  q2.push_front(2);

  expect_eq(q2.empty(), false, "circular_deque.empty()");
  expect_eq(q2.full(), false, "circular_deque.full()");
  expect_eq(q2.len(), 2, "circular_deque.len()");
  expect_eq(q2.pop_back(), 1, "circular_deque.pop_back()");

  q2.push_front(2);
  q2.push_front(3);
  q2.push_front(4);
  expect_eq(q2.len(), 4, "circular_deque.len()");
  expect_eq(q2.full(), true, "circular_deque.full()");

  try {
    q2.push_front(5);
  } catch (std::runtime_error e) {
    expect_match(e.what(), ".*full.*", "full circular_deque.push_front()");
  }

  for (auto i : range(0, q2.len())) {
    q2.pop_back();
  }

  try {
    q2.pop_back();
  } catch (std::runtime_error e) {
    expect_match(e.what(), ".*empty.*", "empty circular_deque.pop_back()");
  }

  for (auto i : range(0, q2.max_len())) {
    if (i % 2 == 0) {
      q2.push_back(i);
    } else {
      q2.push_front(i);
    }
  }

  expect_eq(q2.full(), true, "circular_deque.full()");
}
