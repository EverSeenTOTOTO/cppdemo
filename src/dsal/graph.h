#ifndef GRAPH_H
#define GRAPH_H

#include "./direct_graph.h"
#include "./undirect_graph.h"
#include "../utils.h"

// 边带权图
template <typename V, typename E>
using wol_graph = ol_graph<V, weighted_data<E>>;

template <typename V, typename E>
using waml_graph = aml_graph<V, weighted_data<E>>;

// minimum cost spanning tree
template <typename V, typename E>
std::set<typename waml_graph<V, E>::edge*>& prim_mst(waml_graph<V, E> const& g) {
  using node     = typename waml_graph<V, E>::node;
  using edge     = typename waml_graph<V, E>::edge;
  using node_set = std::set<node*>;
  using edge_set = std::set<edge*>;

  auto verts   = g.get_verts();           // V
  auto visited = new node_set();          // U
  auto result  = new edge_set();          // save result edges
  auto between = new std::list<edge*>();  // save temporary edges between U and V-U

  // 新增vert后，在between中添加vert到v-u中结点的边
  auto insert_linked = [&](node* vert) {
    auto ptr = vert->linked;

    while (ptr != nullptr) {
      auto isHead = ptr->head == vert;
      auto v2     = isHead ? ptr->tail : ptr->head;

      if (visited->find(v2) == visited->end()) {  // v2属于v-u
        auto it = between->begin();

        // 有序插入，方便找到between中最小边
        for (; it != between->end(); ++it) {
          if ((*it)->data.weight > ptr->data.weight) {
            between->insert(it, ptr);
            break;
          }
        }

        if (it == between->end()) {
          between->insert(it, ptr);
        }
      } else {  // v2属于u，若ptr被选择将形成回路，需移除
        between->remove(ptr);
      }

      ptr = isHead ? ptr->head_link : ptr->tail_link;
    }
  };

  std::function<void(node* const)> prim_helper = [&](node* vert) {
    // 将边的另一端加入u中
    visited->insert(vert);

    if (visited->size() < g.vert_count()) {
      insert_linked(vert);

      // 找出最小边
      auto smallest = *(between->begin());

      result->insert(smallest);
      between->remove(smallest);

      // 将边的另一端加入u中
      prim_helper(vert == smallest->head ? smallest->tail : smallest->head);
    }
  };

  prim_helper(*(verts.begin()));

  delete visited;
  delete between;

  return *result;
}

namespace dsal_impl {

  template <typename V, typename E>
  vec<typename waml_graph<V, E>::edge*> const& get_all_edges(waml_graph<V, E> const& g) {
    using edge = typename waml_graph<V, E>::edge;

    std::set<edge*> edges;

    for (auto v : g.get_verts()) {
      auto ptr = v->linked;

      while (ptr != nullptr) {
        edges.insert(ptr);
        ptr = v == ptr->head ? ptr->head_link : ptr->tail_link;
      }
    }

    auto vp = new vec<edge*>;

    for (auto e : edges) {
      vp->push_back(e);
    }

    return *vp;
  }

}  // namespace dsal_impl

template <typename V, typename E>
std::set<typename waml_graph<V, E>::edge*> const& kruskal_mst(waml_graph<V, E> const& g) {
  using node     = typename waml_graph<V, E>::node;
  using edge     = typename waml_graph<V, E>::edge;
  using node_set = std::set<node*>;
  using edge_set = std::set<edge*>;

  auto verts  = g.get_verts();
  auto edges  = dsal_impl::get_all_edges(g);
  auto result = new edge_set();

  // 对所有边进行排序
  std::sort(edges.begin(), edges.end(), [](auto a, auto b) { return a->data.weight < b->data.weight; });

  std::map<node*, node_set*> components;

  // 初始时每个结点自成分量
  for (auto v : verts) {
    components.insert(std::make_pair(v, new node_set{v}));
  }

  for (auto e : edges) {
    if (result->size() == g.vert_count() - 1) break;  // 有n-1条边时完工

    auto headComponent = components.at(e->head);  // head所在连通分量
    auto tailComponent = components.at(e->tail);  // tail所在连通分量

    if (headComponent != tailComponent) {  // 不在同一个连通分量中
      result->insert(e);

      for (auto vert : *tailComponent) {  // 合并连通分量
        headComponent->insert(vert);
      }

      components.insert(std::make_pair(e->head, headComponent));

      for (auto& [key, value] : components) {
        if (value == tailComponent) {
          components.insert(std::make_pair(key, headComponent));
        }
      }
    }
  }

  for (auto& [_, value] : components) {
    delete value;
  }

  return *result;
};

// single source shortest path
void dijkstra_shortest_path();
void floyd_shortest_path();

// topology sort
void topology_sort();

// critical path
void critical_path();

void test_prim_mst();
void test_kruskal_mst();
void test_dijkstra_shortest_path();
void test_floyd_shortest_path();
void test_top_sort();
void test_critical_path();

#endif
