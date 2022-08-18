#include "../common.h"
#include "../utils.h"

template <typename T>
class btree {
 public:
  btree() = delete;
  btree(T const& t) : data(t) {}
  ~btree() {
    if (lchild != nullptr) {
      lchild->~btree();
    }

    if (rchild != nullptr) {
      rchild->~btree();
    }

    lchild = rchild = nullptr;
  }

  T      data;
  btree* lchild = nullptr;
  btree* rchild = nullptr;

  size_t depth() const {
    auto ldepth = lchild != nullptr ? lchild->depth() : 0;
    auto rdepth = rchild != nullptr ? rchild->depth() : 0;

    return std::max(ldepth, rdepth) + 1;
  }

  size_t node_count() const {
    auto lcount = lchild != nullptr ? lchild->node_count() : 0;
    auto rcount = rchild != nullptr ? rchild->node_count() : 0;

    return lcount + rcount + 1;
  }

  inline bool is_leaf() const {
    return lchild == nullptr && rchild == nullptr;
  }

  void pre_order(std::function<void(T)> const& callback) {
    callback(data);

    if (lchild != nullptr) {
      lchild->pre_order(callback);
    }

    if (rchild != nullptr) {
      rchild->pre_order(callback);
    }
  }

  void in_order(std::function<void(T)> const& callback) {
    if (lchild != nullptr) {
      lchild->in_order(callback);
    }

    callback(data);

    if (rchild != nullptr) {
      rchild->in_order(callback);
    }
  }

  void post_order(std::function<void(T)> const& callback) {
    if (lchild != nullptr) {
      lchild->post_order(callback);
    }

    if (rchild != nullptr) {
      rchild->post_order(callback);
    }

    callback(data);
  }
};

// 根据中序序列和后序序列重建二叉树
template <typename T>
btree<T>* rebuild_btree(vec<T> const& in, vec<T> const& post) {
  if (post.size() != in.size()) {
    throw std::runtime_error("invalid sequence");
  }

  if (post.size() == 0) {
    return nullptr;
  } else {
    auto last     = post.size() - 1;
    auto rootData = post[last];
    auto root     = new btree<T>(rootData);
    auto index    = find_index(in, rootData);
    auto rcount   = last - index;

    if (index == -1) {
      throw std::runtime_error("invalid sequence");
    }

    root->lchild = rebuild_btree(slice(in, 0, index), slice(post, 0, index));
    root->rchild = rebuild_btree(slice(in, index + 1, rcount), slice(post, index, rcount));

    return root;
  }
};

template <typename T>
class weighted_data {
 public:
  T      data;
  size_t weight;

  weighted_data() = delete;
  weighted_data(size_t w) : weight(w) {}
  weighted_data(T const& t, size_t w) : data(t), weight(w) {}
};

template <typename T>
class huffman_tree : public btree<weighted_data<T>> {
 public:
  huffman_tree(size_t w) : btree<weighted_data<T>>(weighted_data<T>(w)) {}
  huffman_tree(T const& t, size_t w) : btree<weighted_data<T>>(weighted_data<T>(t, w)) {}
};

template <typename T>
huffman_tree<T>* build_huffman_tree(vec<huffman_tree<T>*> const& forest) {
  if (forest.size() == 1) {
    return forest[0];
  }

  auto clone = slice(forest, 0, forest.size());

  std::sort(clone.begin(), clone.end(), [](auto a, auto b) { return a->data.weight < b->data.weight; });

  auto lchild = clone[0];
  auto rchild = clone[1];
  auto root   = new huffman_tree<T>(lchild->data.weight + rchild->data.weight);

  root->lchild = lchild;
  root->rchild = rchild;

  auto rest = slice(clone, 2, forest.size() - 2);

  rest.push_back(root);

  return build_huffman_tree(rest);
}

void test_btree();
void test_rebuild_btree();
void test_build_huffman_tree();
