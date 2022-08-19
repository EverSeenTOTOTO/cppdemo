#include "./undirect_graph.h"
#include "../utils.h"

void test_aml_graph() {
  aml_graph<std::string, size_t> g;

  g.add_edge(1, "v1", "v2");
  g.add_edge(2, "v2", "v3");
  g.add_edge(3, "v3", "v4");
  g.add_edge(4, "v1", "v4");
  g.add_edge(5, "v2", "v5");
  g.add_edge(6, "v3", "v5");

  expect_eq(g.vert_count(), 5, "aml_graph.vert_count()");
  expect_eq(g.edge_count(), 6, "aml_graph.edge_count()");

  g.remove_vert("v1");

  expect_eq(g.vert_count(), 4, "delete v1 aml_graph.vert_count()");
  expect_eq(g.edge_count(), 4, "delete v1 aml_graph.edge_count()");

  g.add_edge(1, "v1", "v2");
  g.add_edge(4, "v1", "v4");

  expect_eq(g.vert_count(), 5, "rebuild aml_graph.vert_count()");
  expect_eq(g.edge_count(), 6, "rebuild aml_graph.edge_count()");

  g.remove_edge("v1", "v4");

  expect_eq(g.edge_count(), 5, "delete v1->v4 aml_graph.edge_count()");
}

void test_aml_graph_traverse() {
  aml_graph<std::string, size_t> g;

  g.add_edge(1, "v1", "v2");
  g.add_edge(2, "v2", "v3");
  g.add_edge(3, "v3", "v4");
  g.add_edge(4, "v1", "v4");
  g.add_edge(5, "v2", "v5");
  g.add_edge(6, "v3", "v5");

  vec<std::string> dfs;
  vec<std::string> bfs;

  g.dfs([&dfs](auto x) { dfs.push_back(x); });
  g.bfs([&bfs](auto x) { bfs.push_back(x); });

  expect_eq(dfs, vec<std::string>{"v1", "v2", "v3", "v4", "v5"}, "aml_graph.dfs()");
  expect_eq(bfs, vec<std::string>{"v1", "v4", "v2", "v3", "v5"}, "aml_graph.bfs()");

  dfs.clear();
  bfs.clear();

  g.dfs([&dfs](auto x) { dfs.push_back(x); }, g.find_vert("v3"));
  g.bfs([&bfs](auto x) { bfs.push_back(x); }, g.find_vert("v3"));

  expect_eq(dfs, vec<std::string>{"v3", "v2", "v1", "v4", "v5"}, "aml_graph.dfs(start)");
  expect_eq(bfs, vec<std::string>{"v3", "v5", "v4", "v2", "v1"}, "aml_graph.bfs(start)");
}
