#ifndef DOUBLE_POINTER_H
#define DOUBLE_POINTER_H
#include "../common.h"
#include "../dsal/linked_list.h"

/* 双指针问题 */

// 判断链表存在环，快慢指针
const singly_list<int>::list_node* detect_circle(singly_list<int> const& list);
void                               test_detect_circle();

// 移动所有的零到数组末端，快慢指针
void move_zeros(vec<int>& nums);
void test_move_zeros();

// 有序数组两数和，滑动窗口
std::pair<int, int> two_sum(vec<int>& nums, int target);
void                test_two_sum();

// 求串s不含重复字符的最长子串，返回子串长度和首个子串的位置，滑动窗口
std::pair<int, int> longest_substr(std::string const& s);
void                test_longest_substr();

// 求串s中涵盖串t所有字符的最小子串，返回子串长度和首个子串的位置，滑动窗口
std::pair<int, int> shortest_window(std::string const& s, std::string const& t);
void                test_shortest_window();

#endif
