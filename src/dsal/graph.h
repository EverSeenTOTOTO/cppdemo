#ifndef GRAPH_H
#define GRAPH_H

#include "./direct_graph.h"
#include "./undirect_graph.h"
#include "../utils.h"

// 边带权图
template <typename V, typename E, typename W = int>
using wol_graph = ol_graph<V, weighted_data<E, W>>;

template <typename V, typename E, typename W = int>
using waml_graph = aml_graph<V, weighted_data<E, W>>;

// minimum cost spanning tree
template <typename V, typename E>
std::set<typename waml_graph<V, E>::edge*>& prim_mst(waml_graph<V, E> const& g) {
  using node     = typename waml_graph<V, E>::node;
  using edge     = typename waml_graph<V, E>::edge;
  using node_set = std::set<node*>;
  using edge_set = std::set<edge*>;

  auto             verts = g.get_verts();    // V
  node_set         visited;                  // U
  auto             result = new edge_set();  // save result edges
  std::list<edge*> between;                  // save temporary edges between U and V-U

  // 新增vert后，在between中添加vert到v-u中结点的边
  auto insert_linked = [&](node* vert) {
    for (auto ptr : vert->get_linked_edges()) {
      auto v2 = ptr->head == vert ? ptr->tail : ptr->head;

      if (visited.find(v2) == visited.end()) {  // v2属于v-u
        auto it = between.begin();

        // 有序插入，方便找到between中最小边
        for (; it != between.end(); ++it) {
          if ((*it)->data.weight > ptr->data.weight) {
            between.insert(it, ptr);
            break;
          }
        }

        if (it == between.end()) {
          between.insert(it, ptr);
        }
      } else {  // v2属于u，若ptr被选择将形成回路，需移除
        between.remove(ptr);
      }
    }
  };

  std::function<void(node* const)> prim_helper = [&](node* vert) {
    // 将边的另一端加入u中
    visited.insert(vert);

    if (visited.size() < g.vert_count()) {
      insert_linked(vert);

      // 找出最小边
      auto smallest = *(between.begin());

      between.remove(smallest);
      result->insert(smallest);

      // 将边的另一端加入u中
      prim_helper(vert == smallest->head ? smallest->tail : smallest->head);
    }
  };

  prim_helper(*(verts.begin()));

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
  std::sort(edges.begin(), edges.end(),
            [](auto a, auto b) { return a->data.weight < b->data.weight; });

  hashmap<node*, node_set*> components;

  // 初始时每个结点自成分量
  for (auto v : verts) {
    components.insert_or_assign(v, new node_set{v});
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

      components.insert_or_assign(e->head, headComponent);

      for (auto& [key, value] : components) {
        if (value == tailComponent) {
          components.insert_or_assign(key, headComponent);
        }
      }
    }
  }

  return *result;
};

namespace dsal_impl {

  template <typename G>
  auto get_edge_weight(G const& g, typename G::node* from, typename G::node* to) ->
      typename G::edge::type::weight_type {
    using weight_type = typename G::edge::type::weight_type;

    if (to == from) {
      return static_cast<weight_type>(0);
    } else {
      auto e = g.find_edge(from->data, to->data);

      return e != nullptr ? e->data.weight : static_cast<weight_type>(INT_MAX);
    }
  }

  template <typename WT>
  WT add_weight(WT const& a, WT const& b) {
    auto c     = a + b;
    auto infty = static_cast<WT>(INT_MAX);

    if (a > 0 && b > 0 && c <= 0) return infty;  // overflow

    return c;
  }

}  // namespace dsal_impl

// single source shortest path
template <typename V, typename E>
auto dijkstra_shortest_path(wol_graph<V, E> const& g, V const& start)
    -> std::map<typename wol_graph<V, E>::node*,
                typename wol_graph<V, E>::edge::type::weight_type> const& {
  using node        = typename wol_graph<V, E>::node;
  using edge        = typename wol_graph<V, E>::edge;
  using weight_type = typename edge::type::weight_type;

  auto from = g.find_vert(start);

  if (from == nullptr) {
    throw std::runtime_error("not a valid node");
  }

  auto cost = new std::map<node*, weight_type>;  // total cost from start to node

  // init cost
  for (auto to : g.get_verts()) {
    cost->insert_or_assign(to, dsal_impl::get_edge_weight(g, from, to));
  }

  std::set<node*>  visited;  // u
  std::list<edge*> between;  // edges between u and v-u

  std::function<void(node*)> update_cost = [&](node* vert) {
    visited.insert(vert);

    for (auto e : vert->get_out_edges()) {
      if (visited.find(e->tail) != visited.end()) continue;

      auto directWeight   = cost->at(e->tail);
      auto indirectWeight = dsal_impl::add_weight(cost->at(vert), e->data.weight);

      if (indirectWeight < directWeight) {  // path is shorter through vert
        // TODO: save mediator
        cost->insert_or_assign(e->tail, indirectWeight);
      }
    }

    if (visited.size() < g.vert_count()) {
      // 找出v-u中cost最小的顶点。如果有负权边，这里找到的不一定是最小的
      auto it       = cost->begin();
      auto smallest = cost->end();

      for (; it != cost->end(); it++) {
        if (visited.find(it->first) == visited.end()) {
          if (smallest == cost->end() || smallest->second > it->second) {
            smallest = it;
          }
        }
      }

      if (smallest != cost->end()) {
        update_cost(smallest->first);
      }
    }
  };

  update_cost(from);

  return *cost;
}

// all source shortest path
template <typename V, typename E>
auto floyd_shortest_path(wol_graph<V, E> const& g, V const& start)
    -> std::map<typename wol_graph<V, E>::node*,
                typename wol_graph<V, E>::edge::type::weight_type> const& {
  using node        = typename wol_graph<V, E>::node;
  using edge        = typename wol_graph<V, E>::edge;
  using weight_type = typename edge::type::weight_type;

  auto verts = g.get_verts();
  auto from  = g.find_vert(start);

  if (from == nullptr) {
    throw std::runtime_error("not a valid node");
  }

  std::map<node*, std::map<node*, weight_type>*> cost;

  // init cost
  for (auto vi : verts) {
    auto row = new std::map<node*, weight_type>;

    for (auto vj : verts) {
      auto weight = dsal_impl::get_edge_weight(g, vi, vj);

      row->insert_or_assign(vj, weight);
    }

    cost.insert_or_assign(vi, row);
  }

  for (auto vk : verts) {
    for (auto vi : verts) {
      for (auto vj : verts) {
        if (vi == vj || vi == vk || vj == vk) continue;

        auto directWeight   = cost.at(vi)->at(vj);
        auto indirectWeight = dsal_impl::add_weight(cost.at(vi)->at(vk), cost.at(vk)->at(vj));

        if (indirectWeight < directWeight) {
          cost.at(vi)->insert_or_assign(vj, indirectWeight);
        }
      }
    }
  }

  for (auto& [k, v] : cost) {
    if (k != from) {  // TODO: redundant
      delete v;
    }
  }

  return *cost.at(from);
}

// topology sort
template <typename V, typename E>
auto topo_sort(ol_graph<V, E> const& g) -> std::list<typename ol_graph<V, E>::node*> const& {
  using node = typename ol_graph<V, E>::node;
  using edge = typename ol_graph<V, E>::edge;

  std::stack<node*>       zeroInNodes;
  std::map<node*, size_t> inDegrees;

  auto verts  = g.get_verts();
  auto result = new std::list<node*>;

  for (auto v : g.get_verts()) {
    auto inDegree = v->get_in_edges().size();

    inDegrees.insert_or_assign(v, inDegree);

    if (inDegree == 0) {
      zeroInNodes.push(v);
    }
  }

  while (!zeroInNodes.empty()) {
    auto top = zeroInNodes.top();

    zeroInNodes.pop();
    result->push_back(top);

    auto out = top->get_out_nodes();

    for (auto tail : out) {
      auto newDegree = inDegrees.at(tail) - 1;

      inDegrees.insert_or_assign(tail, newDegree);

      if (newDegree == 0) {
        zeroInNodes.push(tail);
      }
    }
  }

  if (result->size() < g.vert_count()) {
    throw std::runtime_error("cirular nodes deteted");
  }

  return *result;
};

// critical path
template <typename V, typename E>
auto critical_path(wol_graph<V, E, size_t> const& g)
    -> std::list<typename wol_graph<V, E, size_t>::edge*> const& {
  using event  = typename wol_graph<V, E, size_t>::node;
  using action = typename wol_graph<V, E, size_t>::edge;

  hashmap<event*, size_t> earliestStartTimeOfEvent;  // 事件最早开始时间
  hashmap<event*, size_t> latestStartTimeOfEvent;    // 事件最迟开始时间

  auto topoSeq   = topo_sort(g);    // 拓扑序列
  auto lastEvent = topoSeq.back();  // 汇点

  // 初始化每个事件的最早开始时间为0
  for (auto evt : topoSeq) {
    earliestStartTimeOfEvent.insert_or_assign(evt, 0);
  }

  for (auto evt : topoSeq) {
    auto requiredTime = earliestStartTimeOfEvent.at(evt);

    //更新依赖evt的事件的最早开始时间
    for (auto e : evt->get_out_edges()) {
      auto maxStartTime = earliestStartTimeOfEvent.at(e->tail);  // 已有的最早开始时间
      auto startTime = requiredTime + e->data.weight;  // 此依赖路径导致的最早开始时间

      //后续事件须等待此事件完成
      earliestStartTimeOfEvent.insert_or_assign(e->tail, std::max(maxStartTime, startTime));
    }
  }

  // 初始化每个事件的最迟开始时间为汇点的最早发生时间
  for (auto evt : topoSeq) {
    latestStartTimeOfEvent.insert_or_assign(evt, earliestStartTimeOfEvent.at(lastEvent));
  }

  decltype(topoSeq) reversed;

  for (auto evt : topoSeq) {
    reversed.push_front(evt);
  }

  for (auto evt : reversed) {
    vec<size_t> startTimes;  // 记录所有依赖此事件的事件的最晚开始时间减去活动耗时的值

    for (auto e : evt->get_out_edges()) {
      startTimes.push_back(latestStartTimeOfEvent.at(e->tail) - e->data.weight);
    }

    if (startTimes.size() > 0) {
      std::sort(startTimes.begin(), startTimes.end());

      // 此事件的最晚开始时间为其中最早的一个
      latestStartTimeOfEvent.insert_or_assign(evt, startTimes[0]);
    }
  }

  auto result = new std::list<action*>;

  for (auto evt : topoSeq) {
    for (auto e : evt->get_out_edges()) {
      auto earliestStartTime =
          earliestStartTimeOfEvent.at(evt);  //活动的最早开始时间等于其前置事件的最早开始时间
      auto latestStartTime =
          latestStartTimeOfEvent.at(e->tail)
          - e->data.weight;  //活动的最晚开始时间为其后置事件的最晚开始时间减去活动耗时

      if (earliestStartTime == latestStartTime) {  //关键活动
        result->push_back(e);
      }
    }
  }

  return *result;
};

void test_prim_mst();
void test_kruskal_mst();
void test_dijkstra_shortest_path();
void test_floyd_shortest_path();
void test_topo_sort();
void test_critical_path();

#endif
