#include "./btree.h"

void test_btree() {
  auto a = new btree<int>(1);
  auto b = new btree<int>(2);
  auto c = new btree<int>(3);
  auto d = new btree<int>(4);

  a->lchild = b;
  a->rchild = c;
  b->lchild = d;

  expect_eq(a->depth(), 3, "btree.depth()");
  expect_eq(a->node_count(), 4, "breee.node_count()");

  vec<int> pre;
  vec<int> in;
  vec<int> post;

  a->pre_order([&pre](int x) { pre.push_back(x); });
  a->in_order([&in](int x) { in.push_back(x); });
  a->post_order([&post](int x) { post.push_back(x); });

  expect_eq(pre, vec<int>{1, 2, 4, 3}, "btree.pre_order()");
  expect_eq(in, vec<int>{4, 2, 1, 3}, "btree.in_order()");
  expect_eq(post, vec<int>{4, 2, 3, 1}, "btree.post_order()");

  delete a;
}

void test_rebuild_btree() {
  vec<char> in{'B', 'D', 'C', 'E', 'A', 'F', 'H', 'G'};
  vec<char> post{'D', 'E', 'C', 'B', 'H', 'G', 'F', 'A'};

  auto      bt = rebuild_btree(in, post);
  vec<char> pre;

  bt->pre_order([&pre](char c) { pre.push_back(c); });

  expect_eq(pre, vec<char>{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}, "rebuild_btree");

  delete bt;
}

void test_build_huffman_tree() {
  auto v = vec<huffman_tree<char>*>{
      new huffman_tree<char>('a', 7),
      new huffman_tree<char>('b', 5),
      new huffman_tree<char>('c', 2),
      new huffman_tree<char>('d', 4),
  };
  auto t = build_huffman_tree(v);

  expect_eq(t->data.weight, 18, "build_huffman_tree");
}
