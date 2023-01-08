#include "./graph.h"

void test_prim_mst() {
  using etype = typename waml_graph<std::string, int>::etype;

  waml_graph<std::string, int> g;

  g.add_edge(etype(6), "v1", "v2");
  g.add_edge(etype(1), "v1", "v3");
  g.add_edge(etype(5), "v1", "v4");
  g.add_edge(etype(5), "v2", "v3");
  g.add_edge(etype(3), "v2", "v5");
  g.add_edge(etype(5), "v3", "v4");
  g.add_edge(etype(6), "v3", "v5");
  g.add_edge(etype(4), "v3", "v6");
  g.add_edge(etype(2), "v4", "v6");
  g.add_edge(etype(6), "v5", "v6");

  auto edges = prim_mst(g);

  expect_eq(edges.size(), 5, "test prim_mst()");
}

void test_kruskal_mst() {
  using etype = typename waml_graph<std::string, int>::etype;

  waml_graph<std::string, int> g;

  g.add_edge(etype(6), "v1", "v2");
  g.add_edge(etype(1), "v1", "v3");
  g.add_edge(etype(5), "v1", "v4");
  g.add_edge(etype(5), "v2", "v3");
  g.add_edge(etype(3), "v2", "v5");
  g.add_edge(etype(5), "v3", "v4");
  g.add_edge(etype(6), "v3", "v5");
  g.add_edge(etype(4), "v3", "v6");
  g.add_edge(etype(2), "v4", "v6");
  g.add_edge(etype(6), "v5", "v6");

  auto edges = kruskal_mst(g);

  expect_eq(edges.size(), 5, "test kruskal_mst()");
}

void test_dijkstra_shortest_path() {
  using etype = typename wol_graph<std::string, int>::etype;

  wol_graph<std::string, int> g;

  g.add_edge(etype(10), "v0", "v2");
  g.add_edge(etype(30), "v0", "v4");
  g.add_edge(etype(100), "v0", "v5");
  g.add_edge(etype(5), "v1", "v2");
  g.add_edge(etype(50), "v2", "v3");
  g.add_edge(etype(10), "v3", "v5");
  g.add_edge(etype(20), "v4", "v3");
  g.add_edge(etype(60), "v4", "v5");

  auto cost = dijkstra_shortest_path(g, std::string("v0"));

  expect_eq(cost.at(g.find_vert("v0")), 0, "dijkstra v0");
  expect_eq(cost.at(g.find_vert("v1")), INT_MAX, "dijkstra v1");
  expect_eq(cost.at(g.find_vert("v2")), 10, "dijkstra v2");
  expect_eq(cost.at(g.find_vert("v3")), 50, "dijkstra v3");
  expect_eq(cost.at(g.find_vert("v4")), 30, "dijkstra v4");
  expect_eq(cost.at(g.find_vert("v5")), 60, "dijkstra v5");
}

void test_floyd_shortest_path() {
  using etype = typename wol_graph<std::string, int>::etype;

  wol_graph<std::string, int> g;

  g.add_edge(etype(10), "v0", "v2");
  g.add_edge(etype(30), "v0", "v4");
  g.add_edge(etype(100), "v0", "v5");
  g.add_edge(etype(5), "v1", "v2");
  g.add_edge(etype(50), "v2", "v3");
  g.add_edge(etype(10), "v3", "v5");
  g.add_edge(etype(20), "v4", "v3");
  g.add_edge(etype(60), "v4", "v5");

  auto cost = floyd_shortest_path(g, std::string("v0"));

  expect_eq(cost.at(g.find_vert("v0")), 0, "floyd v0");
  expect_eq(cost.at(g.find_vert("v1")), INT_MAX, "floyd v1");
  expect_eq(cost.at(g.find_vert("v2")), 10, "floyd v2");
  expect_eq(cost.at(g.find_vert("v3")), 50, "floyd v3");
  expect_eq(cost.at(g.find_vert("v4")), 30, "floyd v4");
  expect_eq(cost.at(g.find_vert("v5")), 60, "floyd v5");
}

void test_topo_sort() {
  ol_graph<std::string, int> g;

  g.add_edge(0, "v1", "v2");
  g.add_edge(1, "v1", "v3");
  g.add_edge(2, "v2", "v3");

  auto verts = topo_sort(g);

  vec<std::string> vs;

  std::for_each(verts.begin(), verts.end(), [&vs](auto x) { vs.push_back(x->data); });

  expect_eq(vs, vec<std::string>{"v1", "v2", "v3"}, "test topo_sort()");
}

void test_critical_path() {
  using etype = typename wol_graph<std::string, std::string, size_t>::etype;

  wol_graph<std::string, std::string, size_t> g;

  g.add_edge(etype("a1", 6), "v0", "v1");
  g.add_edge(etype("a2", 4), "v0", "v2");
  g.add_edge(etype("a3", 5), "v0", "v3");
  g.add_edge(etype("a4", 1), "v1", "v4");
  g.add_edge(etype("a5", 1), "v2", "v4");
  g.add_edge(etype("a6", 2), "v3", "v5");
  g.add_edge(etype("a7", 9), "v4", "v6");
  g.add_edge(etype("a8", 7), "v4", "v7");
  g.add_edge(etype("a9", 4), "v5", "v7");
  g.add_edge(etype("a10", 2), "v6", "v8");
  g.add_edge(etype("a11", 4), "v7", "v8");

  auto paths = critical_path(g);

  std::set<std::string> edges;

  std::for_each(paths.begin(), paths.end(), [&](auto e) { edges.insert(e->data.data); });

  expect_eq(edges, std::set<std::string>{"a1", "a4", "a7", "a8", "a10", "a11"},
            "test critical_path()");
}
