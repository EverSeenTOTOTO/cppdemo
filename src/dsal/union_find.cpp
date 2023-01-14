#include "./union_find.h"
#include "../utils.h"

union_find::union_find(size_t n) {
  count  = n;
  parent = move(vec<int>(n));
  for (auto i = 0; i < n; ++i) {
    parent[i] = i;
  }
}

void union_find::union_both(int p, int q) {
  int rp = find(p);
  int rq = find(q);

  if (rp != rq) {
    parent[rp] = rq;
    count--;
  }
}

bool union_find::connected(int p, int q) {
  return find(p) == find(q);
}

int union_find::find(int x) {
  if (parent[x] != x) {           // root.parent == root
    parent[x] = find(parent[x]);  // x.parent = find(x.parent)
  }

  return parent[x];
}

size_t union_find::get_count() const {
  return count;
}

size_t nums_of_islands_uf(vec<vec<int>> const& grid) {
  size_t flood_count = 0;
  size_t row         = grid.size();
  size_t col         = grid[0].size();

  union_find uf(row * col);

  for (auto i = 0; i < row; ++i) {
    for (auto j = 0; j < col; ++j) {
      if (grid[i][j] == 1) {
        if (j + 1 < col && grid[i][j + 1] == 1) {
          uf.union_both(i * row + j, i * row + j + 1);
        }
        if (i + 1 < row && grid[i + 1][j] == 1) {
          uf.union_both(i * row + j, (i + 1) * row + j);
        }
      } else {
        flood_count++;
      }
    }
  }

  return uf.get_count() - flood_count;
}

void test_nums_of_islands_uf() {
  auto grid = move(vec<vec<int>>{
      vec<int>{1, 1, 0, 0, 0},
      vec<int>{1, 1, 0, 0, 0},
      vec<int>{0, 0, 1, 0, 0},
      vec<int>{0, 0, 0, 1, 1}
  });

  expect_eq(nums_of_islands_uf(grid), 3, "test nums_of_islands_dfs 3");
}

size_t nums_of_islands_dfs(vec<vec<int>> const& grid) {
  size_t   count = 0;
  size_t   row   = grid.size();
  size_t   col   = grid[0].size();
  vec<int> visited(col * grid.size());

  std::function<bool(int, int)> is_island = [&](int i, int j) {
    return grid[i][j] == 1 && visited[i * col + j] != 1;
  };

  std::function<void(int, int)> dfs_flood_fill = [&](int i, int j) {
    if (i < 0 || j < 0 || i == row || j == col) return;  // border
    if (!is_island(i, j)) return;                        // already flood filled

    visited[i * col + j] = 1;  // flooding

    // dfs_flood_fill(i - 1, j);
    // dfs_flood_fill(i, j - 1);
    dfs_flood_fill(i + 1, j);
    dfs_flood_fill(i, j + 1);
  };

  for (auto i = 0; i < row; ++i) {
    for (auto j = 0; j < col; ++j) {
      if (is_island(i, j)) {
        count++;
        dfs_flood_fill(i, j);  // flood fill this island
      }
    }
  }

  return count;
}

void test_nums_of_islands_dfs() {
  auto grid = move(vec<vec<int>>{
      vec<int>{1, 1, 0, 0, 0},
      vec<int>{1, 1, 0, 0, 0},
      vec<int>{0, 0, 1, 0, 0},
      vec<int>{0, 0, 0, 1, 1}
  });

  expect_eq(nums_of_islands_dfs(grid), 3, "test nums_of_islands_dfs 3");
}
