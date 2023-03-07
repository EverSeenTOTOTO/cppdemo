#include "./backtrace.h"
#include <functional>
#include "../utils.h"

vec<vec<int>> permute(vec<int> const& numbers) {
  using namespace std;

  auto result = new vec<vec<int>>;
  auto store  = new vec<int>;
  auto used   = move(vec<int>(numbers.size()));

  function<void(vec<int> const&)> backtrace = [&](vec<int> const& nums) {
    if (store->size() == nums.size()) {
      result->push_back(*store);
      return;
    }

    for (auto i = 0; i < nums.size(); ++i) {
      if (used[i]) continue;

      // choose
      used[i] = true;
      store->push_back(nums[i]);
      backtrace(nums);
      // unchoose
      used[i] = false;
      store->pop_back();
    }
  };

  backtrace(numbers);

  return *result;
}

void test_permute() {
  vec<int> nums{1, 2, 3};

  expect_eq(permute(nums),
            vec<vec<int>>{
                vec<int>{1, 2, 3},
                vec<int>{1, 3, 2},
                vec<int>{2, 1, 3},
                vec<int>{2, 3, 1},
                vec<int>{3, 1, 2},
                vec<int>{3, 2, 1}
  },
            "test permute [1,2,3]");
}

vec<vec<int>> permute_unique(vec<int> const& numbers) {
  using namespace std;

  auto result = new vec<vec<int>>;
  auto store  = new vec<int>;
  auto used   = move(vec<int>(numbers.size()));

  function<void(vec<int> const&)> backtrace = [&](vec<int> const& nums) {
    if (store->size() == nums.size()) {
      result->push_back(*store);
      return;
    }

    for (auto i = 0; i < nums.size();) {
      if (used[i]) {
        i++;
        continue;
      };

      // choose
      used[i] = true;
      store->push_back(nums[i]);
      backtrace(nums);
      // unchoose
      used[i] = false;
      store->pop_back();

      auto index = i + 1;
      while (nums[index] == nums[i]) {
        index++;
      }
      i = index;
    }
  };

  backtrace(numbers);

  return *result;
}

void test_permute_unique() {
  vec<int> nums{1, 1, 2};

  expect_eq(permute_unique(nums),
            vec<vec<int>>{
                vec<int>{1, 1, 2},
                vec<int>{1, 2, 1},
                vec<int>{2, 1, 1}
  },
            "test permute_unique [1,2,3]");
}

vec<vec<int>> subsets(vec<int> const& numbers) {
  using namespace std;

  auto store  = new vec<int>;
  auto result = new vec<vec<int>>;

  std::function<void(vec<int> const&)> backtrace = [&](vec<int> const& nums) {
    result->push_back(*store);

    for (auto i = 0; i < nums.size(); ++i) {
      // choose
      store->push_back(nums[i]);
      backtrace(slice(nums, i + 1, nums.size()));
      // unchoose
      store->pop_back();
    }
  };

  backtrace(numbers);

  return *result;
}

void test_subsets() {
  vec<int> nums{1, 2, 3};

  expect_eq(subsets(nums),
            vec<vec<int>>{
                vec<int>{ },
                vec<int>{1 },
                vec<int>{1,  2},
                vec<int>{1,  2, 3},
                vec<int>{1,3},
                vec<int>{2 },
                vec<int>{2,  3},
                vec<int>{3 }
  },
            "test subsets [1,2,3]");
}

vec<vec<int>> subsets_with_dup(vec<int> const& numbers) {
  using namespace std;

  auto store  = new vec<int>;
  auto result = new vec<vec<int>>;

  // std::function<void(vec<int> const&)> backtrace = [&](vec<int> const& nums) {
  //   result->push_back(*store);

  //   for (auto i = 0; i < nums.size(); ++i) {
  //     // already used
  //     if (store->size() > 0 && store->back() >= nums[i]) continue;
  //     // same choice
  //     if (i > 0 && nums[i] == nums[i - 1]) continue;

  //     // choose
  //     store->push_back(nums[i]);
  //     backtrace(nums);
  //     // unchoose
  //     store->pop_back();
  //   }
  // };

  std::function<void(vec<int> const&)> backtrace = [&](vec<int> const& nums) {
    result->push_back(*store);

    for (auto i = 0; i < nums.size();) {
      // choose
      store->push_back(nums[i]);

      // skip duplicate
      auto index = i + 1;
      while (nums[index] == nums[i]) {
        index++;
      }

      backtrace(slice(nums, index, nums.size()));
      // unchoose
      store->pop_back();

      i = index;
    }
  };

  backtrace(numbers);

  return *result;
}

void test_subsets_with_dup() {
  vec<int> nums{1, 2, 2, 3};

  expect_eq(subsets_with_dup(nums),
            vec<vec<int>>{
                vec<int>{ },
                vec<int>{1 },
                vec<int>{1,  2},
                vec<int>{1,  2, 3},
                vec<int>{1,3},
                vec<int>{2 },
                vec<int>{2,  3},
                vec<int>{3 }
  },
            "test subsets_with_dup [1,2,2,3]");
}

vec<vec<int>> combination_sum(vec<int> const& candidates, int target) {
  using namespace std;

  auto result = new vec<vec<int>>;
  auto store  = new vec<int>;
  auto sum    = 0;

  function<void(vec<int> const&)> backtrace = [&](vec<int> const& nums) {
    for (auto i = 0; i < nums.size();) {
      // choose
      sum += nums[i];
      store->push_back(nums[i]);

      auto index = i + 1;

      if (sum < target) {
        backtrace(slice(nums, index, nums.size()));
      } else if (sum == target) {
        result->push_back(*store);
      };

      // unchoose
      sum -= nums[i];
      store->pop_back();

      // skip duplicate
      while (nums[index] == nums[i]) {
        index++;
      }

      i = index;
    }
  };

  backtrace(candidates);

  return *result;
}

void test_combination_sum() {
  vec<int> candidates{1, 1, 2, 5, 6, 7, 10};
  int      target = 8;

  expect_eq(combination_sum(candidates, target),
            vec<vec<int>>{
                vec<int>{1,  1, 6},
                vec<int>{1,  2,   5},
                vec<int>{1, 7},
                vec<int>{2, 6  }
  },
            "test combination_sum [1, 1, 2, 5, 6, 7, 10]");
}

vec<vec<int>> combination_sum_repeat(vec<int> const& candidates, int target) {
  using namespace std;

  auto result = new vec<vec<int>>;
  auto store  = new vec<int>;
  auto sum    = 0;

  function<void(vec<int> const&, int)> backtrace = [&](vec<int> const& nums, int index) {
    for (auto i = 0; i < nums.size(); ++i) {
      if (i < index) continue;
      // choose
      sum += nums[i];
      store->push_back(nums[i]);

      if (sum < target) {
        backtrace(nums, i);
      } else if (sum == target) {
        result->push_back(*store);
      };

      // unchoose
      sum -= nums[i];
      store->pop_back();
    }
  };

  backtrace(candidates, -1);

  return *result;
}

void test_combination_sum_repeat() {
  vec<int> candidates{2, 3, 5};
  int      target = 8;

  expect_eq(combination_sum_repeat(candidates, target),
            vec<vec<int>>{
                vec<int>{2,  2, 2, 2},
                vec<int>{2, 3,  3},
                vec<int>{3, 5}
  },
            "test combination_sum_repeat [2,3,5]");
}
