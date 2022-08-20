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

  expect_eq(edges.size(), 5, "prim_mst size");
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

  expect_eq(edges.size(), 5, "prim_mst size");
}
