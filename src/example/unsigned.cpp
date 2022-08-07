#include "./unsigned.h"
#include "../utils.h"

void test_unsigned() {
  expect(0 == 0U, "0 == 0U");
  expect(-1 < 0, "-1 < 0");
  expect_not(-1 < 0U, "-1 > 0U");

  expect(INT_MAX > INT_MIN, "INT_MAX > INT_MIN");
  expect_not((unsigned int) INT_MAX > INT_MIN, "(unsigned int) INT_MAX < INT_MIN");

  expect(-1 > -2, "-1 > -2");
  expect(-1U > -2, "-1U > -2");
}
