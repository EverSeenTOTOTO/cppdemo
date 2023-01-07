#include "./linked_list.h"
#include "../utils.h"

void test_singly_list() {
  singly_list<int> list;

  expect_eq(list.first(), nullptr, "empty singly_list.first()");
  expect_eq(list.last(), list.first(), "empty singly_list.last()");
  expect_eq(list.len(), 0, "empty singly_list.len()");
  expect_eq(list.empty(), true, "empty singly_list.empty()");

  singly_list<int> l2{1, 2, 3};
  expect_eq(l2.first()->data, 1, "singly_list.first()");
  expect_eq(l2.last()->data, 3, "singly_list.last()");
  expect_eq(l2.len(), 3, "singly_list.len()");
  expect_eq(l2.empty(), false, "singly_list.empty()");

  l2.append(5);
  l2.append(4);
  expect_eq(l2.last()->data, 4, "append singly_list.last()");
  expect_eq(l2.len(), 5, "append singly_list.len()");

  l2.prepend(0);
  l2.prepend(0);
  expect_eq(l2.first()->data, 0, "prepend singly_list.first()");
  expect_eq(l2.len(), 7, "prepend singly_list.len()");

  l2.remove_all(0);
  expect_eq(l2.first()->data, 1, "remove_all 0 singly_list.first()");
  expect_eq(l2.len(), 5, "remove_all 0 singly_list.len()");

  singly_list<int> l3{1, 2, 3, 4};

  l3.reverse();
  expect_eq(l3.first()->data, 4, "reverse singly_list.first()");
  expect_eq(l3.last()->data, 1, "reverse singly_list.last()");
}

singly_list<int>::list_node* find_max(singly_list<int> const& l) {
  auto max = l.first();
  auto ptr = max;

  while (ptr) {
    if (ptr->data > max->data) {
      max = ptr;
    }
    ptr = ptr->next;
  }

  return max;
}

void test_find_max() {
  singly_list<int> l{1, 2, 3, 3, 2, 1};

  expect_eq(find_max(l)->data, 3, "find_max");

  l.remove_all(3);
  l.prepend(4);
  l.append(5);

  expect_eq(find_max(l)->data, 5, "find_max changed");
}
