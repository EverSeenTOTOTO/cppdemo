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

vec<int> move_zeros(vec<int>& nums) {
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

  return nums;
}

void test_move_zeros() {
  vec<int> v{0, 1, 0, 3, 12};

  expect_eq(move_zeros(v), vec<int>{1, 3, 12, 0, 0}, "test move_zeros [0,1,0,3,12]");
}

vec<int> two_sum(vec<int>& nums, int target) {
  int left  = 0;
  int right = nums.size() - 1;

  while (left < right) {
    auto sum = nums[left] + nums[right];
    if (sum == target) {
      return *(new vec<int>{left, right});
    } else if (sum < target) {
      left++;
    } else {
      right--;
    }
  }

  return *(new vec<int>{});
}

void test_two_sum() {
  auto nums   = vec<int>{2, 7, 11, 15};
  auto target = 9;

  expect_eq(two_sum(nums, target), vec<int>{0, 1}, "test twoSum [2,7,11,15]");

  nums   = vec<int>{2, 3, 4};
  target = 6;

  expect_eq(two_sum(nums, target), vec<int>{0, 2}, "test twoSum [2,3,4]");

  nums   = vec<int>{3, 3};
  target = 6;

  expect_eq(two_sum(nums, target), vec<int>{0, 1}, "test twoSum [3,3]");
}
