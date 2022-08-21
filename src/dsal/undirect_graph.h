#ifndef UNDIRECT_GRAPH_H
#define UNDIRECT_GRAPH_H

#include "../common.h"

// 邻接多重表，adjacent_multilist
template <typename V, typename E>
class aml_edge;
template <typename V, typename E>
class aml_graph;

template <typename V, typename E>
class aml_node {
 public:
  using node = aml_node<V, E>;
  using edge = aml_edge<V, E>;
  using type = V;

  V     data;
  edge* linked = nullptr;

  aml_node() = delete;
  aml_node(V const& data) : data(data) {}

  ~aml_node() {
    for (auto e : get_linked_edges()) {
      e->remove_self();
      delete e;
    }

    linked = nullptr;
  }

  std::list<edge*> const& get_linked_edges() const {
    auto ptr   = linked;
    auto edges = new std::list<edge*>;

    while (ptr != nullptr) {
      edges->push_back(ptr);
      ptr = ptr->head == this ? ptr->head_link : ptr->tail_link;
    }

    return *edges;
  }

  friend class aml_edge<V, E>;
  friend class aml_graph<V, E>;

 protected:
  void remove_edge(edge* e) {  // 从linked链表中移除一条依附于此结点的边
    auto  ptr  = linked;
    edge* prev = nullptr;

    while (ptr != nullptr) {
      auto next = ptr->head == this ? ptr->head_link : ptr->tail_link;

      if (ptr == e) {
        if (prev == nullptr) {  // ptr == linked
          linked = next;
        } else {
          if (prev->head == this) {
            prev->head_link = next;
          } else {
            prev->tail_link = next;
          }
        }
        break;
      }

      prev = ptr;
      ptr  = next;
    }
  }
};

template <typename V, typename E>
class aml_edge {
 public:
  using node = aml_node<V, E>;
  using edge = aml_edge<V, E>;
  using type = E;

  E     data;
  node* head      = nullptr;
  node* tail      = nullptr;
  edge* head_link = nullptr;
  edge* tail_link = nullptr;

  aml_edge() = delete;
  aml_edge(E const& data) : data(data) {}

  friend class aml_node<V, E>;
  friend class aml_graph<V, E>;

 protected:
  void remove_self() {
    head->remove_edge(this);
    tail->remove_edge(this);
  }
};

template <typename V, typename E>
class aml_graph {
 public:
  using node  = aml_node<V, E>;
  using edge  = aml_edge<V, E>;
  using vtype = typename node::type;
  using etype = typename edge::type;

  inline size_t vert_count() const {
    return verts.size();
  }

  inline std::list<node*> const& get_verts() const {
    return verts;
  }

  size_t edge_count() const {
    size_t count = 0;

    for (auto v : verts) {
      count += v->get_linked_edges().size();
    }

    return count / 2;
  }

  node* add_vert(V const& node_value) {
    if (find_vert(node_value) != nullptr) {
      throw std::runtime_error("vert already exist");
    }

    auto v = new node(node_value);

    verts.push_back(v);

    return v;
  }

  node* find_vert(V const& node_value) const {
    for (auto v : verts) {
      if (v->data == node_value) {
        return v;
      }
    }

    return nullptr;
  }

  // add edge, from or to will be added into graph if don't existz
  edge* add_edge(E const& edge_value, V const& from, V const& to) {
    if (find_edge(from, to) != nullptr) {
      throw std::runtime_error("edge already exist");
    }

    auto head = find_vert(from);

    if (head == nullptr) {
      head = add_vert(from);
    }

    auto tail = find_vert(to);

    if (tail == nullptr) {
      tail = add_vert(to);
    }

    auto e = new edge(edge_value);

    e->head      = head;
    e->tail      = tail;
    e->head_link = head->linked;
    e->tail_link = tail->linked;
    head->linked = e;
    tail->linked = e;

    return e;
  }

  edge* find_edge(V const& from, V const& to) const {
    auto head = find_vert(from);
    auto tail = find_vert(to);

    if (head != nullptr && tail != nullptr) {
      for (auto e : head->get_linked_edges()) {
        if (e->head == head && e->tail == tail) {
          return e;
        }

        if (e->head == tail && e->tail == head) {
          return e;
        }
      }
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
    auto edge = find_edge(from, to);

    if (edge != nullptr) {
      edge->remove_self();

      delete edge;
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

        for (auto e : top->get_linked_edges()) {
          s.push(e->head == top ? e->tail : e->head);
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

        for (auto e : top->get_linked_edges()) {
          q.push(e->head == top ? e->tail : e->head);
        }
      }
    }
  }

 private:
  std::list<node*> verts;
};

void test_aml_graph();
void test_aml_graph_traverse();

#endif
