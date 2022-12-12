#include "./banker.h"

void test_banker() {
  std::map<std::string, std::pair<uvec, uvec>> table{
      {"1", {{0, 0, 1, 4}, {0, 6, 5, 6}}},
      {"2", {{1, 4, 3, 2}, {1, 9, 4, 2}}},
      {"3", {{1, 3, 5, 4}, {1, 3, 5, 6}}},
      {"4", {{1, 0, 0, 0}, {1, 7, 5, 0}}},
  };
  uvec total{1, 5, 2, 0};

  auto result = banker(table, total);

  expect_eq(result, vec<std::string>{"2", "1", "3", "4"}, "test banker");

  uvec total2{1, 1, 2};

  std::map<std::string, std::pair<uvec, uvec>> table2{{"p1", {{1, 0, 0}, {3, 2, 2}}},
                                                      {"p2", {{5, 1, 1}, {6, 1, 3}}},
                                                      {"p3", {{2, 1, 1}, {3, 1, 4}}},
                                                      {"p4", {{0, 0, 2}, {4, 2, 2}}}};

  auto result2 = banker(table2, total2);

  expect_eq(result2, vec<std::string>{"p2", "p1", "p3", "p4"}, "test banker2");
}
