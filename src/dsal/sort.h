#ifndef SORT_H
#define SORT_H

#include "../common.h"
#include "../utils.h"

void test_slice();

void insert_sort(vec<int>& s);
void test_insert_sort();

void bubble_sort(vec<int>& s);
void test_bubble_sort();

void quick_sort(vec<int>& s);
void test_quick_sort();

void heap_sort(vec<int>& s);
void test_heap_sort();

#endif
