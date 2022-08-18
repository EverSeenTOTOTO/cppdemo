#include "./direct_graph.h"
#include "../utils.h"

void test_ol_graph() {
  ol_graph<std::string, size_t> g;

  g.add_edge(1, "v1", "v2");
  g.add_edge(2, "v1", "v3");
  g.add_edge(3, "v3", "v4");
  g.add_edge(4, "v4", "v1");
  g.add_edge(5, "v3", "v1");
  g.add_edge(6, "v4", "v3");
  g.add_edge(7, "v4", "v2");

  expect_eq(g.vert_count(), 4, "ol_graph.vert_count()");
  expect_eq(g.edge_count(), 7, "ol_graph.edge_count()");

  g.remove_vert("v1");

  expect_eq(g.vert_count(), 3, "delete v1 ol_graph.vert_count()");
  expect_eq(g.edge_count(), 3, "delete v1 ol_graph.edge_count()");

  g.add_edge(1, "v1", "v2");
  g.add_edge(2, "v1", "v3");
  g.add_edge(4, "v4", "v1");
  g.add_edge(5, "v3", "v1");

  expect_eq(g.vert_count(), 4, "rebuild ol_graph.vert_count()");
  expect_eq(g.edge_count(), 7, "rebuild ol_graph.edge_count()");

  g.remove_edge("v1", "v3");

  expect_eq(g.edge_count(), 6, "delete v1->v3 ol_graph.edge_count()");
}

void test_ol_graph_traverse() {
  ol_graph<std::string, size_t> g;

  g.add_edge(1, "v1", "v2");
  g.add_edge(2, "v1", "v3");
  g.add_edge(3, "v3", "v4");
  g.add_edge(4, "v4", "v1");
  g.add_edge(5, "v3", "v1");
  g.add_edge(6, "v4", "v3");
  g.add_edge(7, "v4", "v2");

  vec<std::string> dfs;
  vec<std::string> bfs;

  g.dfs([&dfs](auto x) { dfs.push_back(x); });
  g.bfs([&bfs](auto x) { bfs.push_back(x); });

  expect_eq(dfs, vec<std::string>{"v1", "v2", "v3", "v4"}, "ol_graph.dfs()");
  expect_eq(bfs, vec<std::string>{"v1", "v3", "v2", "v4"}, "ol_graph.bfs()");

  dfs.clear();
  bfs.clear();

  g.dfs([&dfs](auto x) { dfs.push_back(x); }, g.find_vert("v3"));
  g.bfs([&bfs](auto x) { bfs.push_back(x); }, g.find_vert("v3"));

  expect_eq(dfs, vec<std::string>{"v3", "v4", "v1", "v2"}, "ol_graph.dfs(start)");
  expect_eq(bfs, vec<std::string>{"v3", "v1", "v4", "v2"}, "ol_graph.bfs(start)");
}
