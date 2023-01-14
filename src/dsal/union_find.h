#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "../common.h"

class union_find {
 public:
  union_find(size_t n);

  void   union_both(int p, int q);
  bool   connected(int p, int q);
  int    find(int x);
  size_t get_count() const;

 private:
  size_t   count;
  vec<int> parent;
};

size_t nums_of_islands_uf(vec<vec<int>> const& grid);
void   test_nums_of_islands_uf();

size_t nums_of_islands_dfs(vec<vec<int>> const& grid);
void   test_nums_of_islands_dfs();

#endif
