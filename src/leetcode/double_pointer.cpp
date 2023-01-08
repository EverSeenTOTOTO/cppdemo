#include "./double_pointer.h"
#include "../dsal/sort.h"

using slist = singly_list<int>;

const slist::list_node* detect_circle(slist const& list) {
  auto slow = list.first();
  auto fast = list.first();

  while (fast != nullptr) {
    slow = slow->next;
    fast = fast->next != nullptr ? fast->next->next : nullptr;

    if (slow == fast) break;
  }

  if (slow == nullptr) return nullptr;

  slow = list.first();
  while (slow != fast) {
    slow = slow->next;
    fast = fast->next;
  }

  return slow;
}

void test_detect_circle() {
  slist l{3, 2, 0, 4};
  auto  last = l.last();

  last->next = l.first()->next;

  expect_eq(detect_circle(l), l.first()->next, "test_detect_circle [3,2,0,4]");
  last->next = nullptr;

  slist l2{1, 2};
  last = l2.last();

  last->next = l2.first();

  expect_eq(detect_circle(l2), l2.first(), "test_detect_circle [1,2]");
  last->next = nullptr;

  slist l3{1};

  expect_eq(detect_circle(l3), nullptr, "test_detect_circle [1]");
}

void move_zeros(vec<int>& nums) {
  auto fast = 0;
  auto slow = 0;

  while (fast < nums.size()) {
    if (nums[fast] != 0) {
      nums[slow++] = nums[fast];
    }
    fast++;
  }

  for (auto i = slow; i < nums.size(); i++) {
    nums[i] = 0;
  }
}

void test_move_zeros() {
  vec<int> v{0, 1, 0, 3, 12};

  move_zeros(v);

  expect_eq(v, vec<int>{1, 3, 12, 0, 0}, "test move_zeros [0,1,0,3,12]");
}

std::pair<int, int> two_sum(vec<int>& nums, int target) {
  auto left   = 0;
  auto right  = 1;
  auto result = new std::pair<int, int>{-1, -1};

  while (left < right) {
    if (nums[left] + nums[right] == target) {
      result->first  = left;
      result->second = right;
      break;
    }

    // 增大窗口
    if (right < nums.size() - 1) {
      right++;
    } else {
      // 缩小窗口
      left++;
    }
  }

  return *result;
}

void test_two_sum() {
  auto nums   = vec<int>{2, 7, 11, 15};
  auto target = 9;

  expect_eq(two_sum(nums, target), std::pair<int, int>{0, 1}, "test two_sum [2,7,11,15]");

  nums   = vec<int>{2, 3, 4};
  target = 6;

  expect_eq(two_sum(nums, target), std::pair<int, int>{0, 2}, "test two_sum [2,3,4]");

  nums   = vec<int>{3, 3};
  target = 6;

  expect_eq(two_sum(nums, target), std::pair<int, int>{0, 1}, "test two_sum [3,3]");
}

std::pair<int, int> longest_substr(std::string const& s) {
  auto left   = 0;
  auto right  = 0;
  auto result = new std::pair<int, int>{left, 1};

  hashmap<char, size_t> count;

  while (right <= s.size()) {
    if (right - left > result->second) {
      result->first  = left;
      result->second = right - left;
    }

    // 增大窗口
    // if (s.substr(left, right - left).find(s[right]) == std::string::npos) {
    if (count[s[right]] < 1) {
      count[s[right]]++;
      right++;
    } else {
      count[s[left]]--;
      // 缩小窗口
      left++;
    }
  }

  return *result;
}

void test_longest_substr() {
  expect_eq(longest_substr("abc"), std::pair<int, int>{0, 3}, "test longest_substr abc");
  expect_eq(longest_substr("abcabcbb"), std::pair<int, int>{0, 3}, "test longest_substr abcabcbb");
  expect_eq(longest_substr("bbbb"), std::pair<int, int>{0, 1}, "test longest_substr bbbb");
  expect_eq(longest_substr("pwwkew"), std::pair<int, int>{2, 3}, "test longest_substr pwwkew");
}

std::pair<int, int> shortest_window(std::string const& s, std::string const& t) {
  auto left         = 0;
  auto right        = 0;
  auto result       = new std::pair<int, int>{-1, -1};
  auto substr_cover = [&](size_t l, size_t r) {
    if (r - l < t.size()) return false;

    auto substr = s.substr(left, right - left);

    for (auto ch : t) {
      if (substr.find(ch) == std::string::npos) return false;
    }

    return true;
  };

  while (right <= s.size()) {
    auto cover = substr_cover(left, right);

    if (!cover) {
      // 增大窗口
      right++;
    } else {
      if (result->second == -1 || right - left < result->second) {
        result->first  = left;
        result->second = right - left;
      }

      // 缩小窗口
      left++;
    }
  }

  return *result;
}

void test_shortest_window() {
  expect_eq(shortest_window("a", "b"), std::pair<int, int>{-1, -1}, "test shortest_window a b");
  expect_eq(shortest_window("a", "a"), std::pair<int, int>{0, 1}, "test shortest_window a a");
  expect_eq(shortest_window("ac", "ab"), std::pair<int, int>{-1, -1}, "test shortest_window ac ab");
  expect_eq(shortest_window("adobecodebanc", "abc"), std::pair<int, int>{9, 4},
            "test shortest_window adobecodebanc abc");
}
