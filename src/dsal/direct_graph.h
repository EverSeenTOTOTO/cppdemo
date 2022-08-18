#include "../common.h"

// 十字链表 orthogonal_list
template <typename V, typename E>
class ol_edge;

template <typename V, typename E>
class ol_node {
 public:
  using node = ol_node<V, E>;
  using edge = ol_edge<V, E>;

  V     data;
  edge* first_in  = nullptr;  // 以此结点为tail的一条弧
  edge* first_out = nullptr;  // 以此结点为head的一条弧

  ol_node() = delete;
  ol_node(V const& data) : data(data) {}

  ~ol_node() {
    auto ptr = first_in;

    while (ptr != nullptr) {
      auto temp = ptr->tail_link;
      ptr->head->remove_out_edge(ptr);  // 从另一端出边链表中移除ptr
      delete ptr;
      ptr = temp;
    }

    ptr = first_out;

    while (ptr != nullptr) {
      auto temp = ptr->head_link;
      ptr->tail->remove_in_edge(ptr);
      delete ptr;
      ptr = temp;
    }

    first_in = first_out = nullptr;
  }

  void remove_out_edge(edge* out) {
    auto  ptr  = first_out;
    edge* prev = nullptr;

    while (ptr != nullptr) {
      if (ptr == out) {
        if (prev == nullptr) {  // ptr == first_out
          first_out = ptr->head_link;
        } else {
          prev->head_link = ptr->head_link;
        }
        break;
      }
      prev = ptr;
      ptr  = ptr->head_link;
    }
  }

  void remove_in_edge(edge* in) {
    auto  ptr  = first_in;
    edge* prev = nullptr;

    while (ptr != nullptr) {
      if (ptr == in) {
        if (prev == nullptr) {  // ptr == first_in
          first_in = ptr->tail_link;
        } else {
          prev->tail_link = ptr->tail_link;
        }
        break;
      }
      prev = ptr;
      ptr  = ptr->tail_link;
    }
  }
};

template <typename V, typename E>
class ol_edge {
 public:
  using node = ol_node<V, E>;
  using edge = ol_edge<V, E>;

  E     data;
  node* head      = nullptr;  // 箭头起始
  node* tail      = nullptr;  // 箭头指向
  edge* head_link = nullptr;  // 头相同的另一条弧
  edge* tail_link = nullptr;  // 尾相同的另一条弧

  ol_edge() = delete;
  ol_edge(E const& data) : data(data) {}
};

template <typename V, typename E>
class ol_graph {
 public:
  using node = ol_node<V, E>;
  using edge = ol_edge<V, E>;

  ~ol_graph() {
    for (auto v : verts) {
      delete v;
    }
  }

  inline size_t vert_count() const {
    return verts.size();
  }

  size_t edge_count() const {
    if (vert_count() == 0) return 0;

    size_t count = 0;
    edge*  ptr   = nullptr;

    for (auto v : verts) {
      ptr = v->first_out;
      while (ptr != nullptr) {
        count++;
        ptr = ptr->head_link;
      }
    }

    return count;
  }

  node* add_vert(V const& node_value) {
    auto v = new node(node_value);

    verts.push_back(v);

    return v;
  }

  node* find_vert(V const& node_value) {
    for (auto v : verts) {
      if (v->data == node_value) {
        return v;
      }
    }

    return nullptr;
  }

  // add edge, from or to will be added into graph if don't existz
  edge* add_edge(E const& edge_value, V const& from, V const& to) {
    auto head = find_vert(from);

    if (head == nullptr) {
      head = add_vert(from);
    }

    auto tail = find_vert(to);

    if (tail == nullptr) {
      tail = add_vert(to);
    }

    auto e = new edge(edge_value);

    e->head         = head;
    e->tail         = tail;
    e->head_link    = head->first_out;
    e->tail_link    = tail->first_in;
    head->first_out = e;
    tail->first_in  = e;

    return e;
  }

  edge* find_edge(V const& from, V const& to) {
    auto head = find_vert(from);
    auto tail = find_vert(to);

    if (head != nullptr && tail != nullptr) {
      auto ptr = head->first_out;

      while (ptr != nullptr && ptr->tail != tail) {
        ptr = ptr->head_link;
      }

      return ptr;
    }

    return nullptr;
  }

  void remove_vert(V const& node_value) {
    auto vert = find_vert(node_value);

    if (vert != nullptr) {
      verts.remove(vert);

      delete vert;
    }
  }

  void remove_edge(V const& from, V const& to) {
    auto head = find_vert(from);
    auto tail = find_vert(to);

    if (head != nullptr && tail != nullptr) {
      edge* e   = nullptr;
      auto  ptr = head->first_out;

      while (ptr != nullptr) {
        if (ptr->tail == tail) {
          e = ptr;
          break;
        }
        ptr = ptr->head_link;
      }

      if (e != nullptr) {
        head->remove_out_edge(e);
        tail->remove_in_edge(e);

        delete e;
      }
    }
  }

  using callback = std::function<void(V)>;
  using tagset   = std::set<node*>;

  void dfs(callback const& cb, node* start = nullptr) {
    tagset tags;

    if (start != nullptr) {  // traverse one component
      dfs_traverse(cb, start, tags);
    } else {  // traverse all vertexs
      for (auto v : verts) {
        dfs_traverse(cb, v, tags);
      }
    }
  }

  void bfs(callback const& cb, node* start = nullptr) {
    tagset tags;

    if (start != nullptr) {  // traverse a component
      bfs_traverse(cb, start, tags);
    } else {  // traverse all vertexs
      for (auto v : verts) {
        bfs_traverse(cb, v, tags);
      }
    }
  }

 protected:
  void dfs_traverse(callback const& cb, node* start, tagset& tags) {
    std::stack<node*> s;

    s.push(start);

    while (!s.empty()) {
      auto top = s.top();

      s.pop();

      if (tags.find(top) == tags.end()) {
        tags.insert(top);
        cb(top->data);

        auto out = top->first_out;

        while (out != nullptr) {
          s.push(out->tail);
          out = out->head_link;
        }
      }
    }
  }

  void bfs_traverse(callback const& cb, node* start, tagset& tags) {
    std::queue<node*> q;

    q.push(start);

    while (!q.empty()) {
      auto top = q.front();

      q.pop();

      if (tags.find(top) == tags.end()) {
        tags.insert(top);
        cb(top->data);

        auto out = top->first_out;

        while (out != nullptr) {
          q.push(out->tail);
          out = out->head_link;
        }
      }
    }
  }

 private:
  std::list<node*> verts;
};

void test_ol_graph();
void test_ol_graph_traverse();
