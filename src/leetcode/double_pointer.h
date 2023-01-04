#ifndef DOUBLE_POINTER_H
#define DOUBLE_POINTER_H
#include "../common.h"
#include "../dsal/linked_list.h"

/* 双指针问题 */

// 判断链表存在环，快慢指针
const singly_list<int>::list_node* detect_circle(singly_list<int> const& list);
void                               test_detect_circle();

// 移动所有的零到数组末端，快慢指针
vec<int> move_zeros(vec<int>& nums);
void     test_move_zeros();

// 有序数组两数和，左右指针
vec<int> two_sum(vec<int>& nums, int target);
void     test_two_sum();

#endif
