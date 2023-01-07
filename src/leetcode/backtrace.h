#ifndef BACKTRACE_H
#define BACKTRACE_H
#include "../common.h"

/* 回溯 */

// 无重复全排列
vec<vec<int>> permute(vec<int> const& nums);
void          test_permute();

// 有重复求全排列
vec<vec<int>> permute_unique(vec<int> const& nums);
void          test_permute_unique();

// 无重复全部子集，假设nums已排序
vec<vec<int>> subsets(vec<int> const& nums);
void          test_subsets();

// 有重复全部子集，假设nums已排序
vec<vec<int>> subsets_with_dup(vec<int> const& nums);
void          test_subsets_with_dup();

// 组合总和，数字不可重复选，假设nums已排序
vec<vec<int>> combination_sum(vec<int> const& candidates, int target);
void          test_combination_sum();

// 组合总和，数字可重复选，假设nums已排序
vec<vec<int>> combination_sum_repeat(vec<int> const& candidates, int target);
void          test_combination_sum_repeat();

#endif
