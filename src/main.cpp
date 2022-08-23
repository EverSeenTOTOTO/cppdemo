// #include "example/semaphore2.h"
// #include "example/read_write_channel.h"
// #include "example/unsigned.h"
// #include "dsal/sort.h"
// #include "dsal/linked_list.h"
// #include "dsal/stack_queue.h"
// #include "dsal/btree.h"
#include "dsal/direct_graph.h"
#include "dsal/undirect_graph.h"
#include "dsal/graph.h"

int main() {
  using namespace std;

  // test_semaphore2();
  // test_unsigned();
  // test_dispatcher();
  // test_slice();
  // test_bubble_sort();
  // test_quick_sort();
  // test_quick_sort_parallel();
  // test_singly_list();
  // test_find_max();
  // test_base_conversion();
  // test_circular_deque();
  // test_btree();
  // test_rebuild_btree();
  // test_build_huffman_tree();
  test_ol_graph();
  test_ol_graph_traverse();
  test_aml_graph();
  test_aml_graph_traverse();
  test_prim_mst();
  test_kruskal_mst();
  test_dijkstra_shortest_path();
  test_floyd_shortest_path();
  // test_topo_sort();
  test_critical_path();

  return 0;
}
