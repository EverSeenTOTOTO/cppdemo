#include <limits.h>
#include <stdio.h>

void test_unsigned() {
  printf("%d\n", 0 == 0U);
  printf("%d\n", -1 < 0);
  printf("%d\n", -1 < 0U); // false

  printf("INT_MAX is %d\n", INT_MAX);
  printf("INT_MIN is %d\n", INT_MIN);

  printf("%d\n", INT_MAX > INT_MIN);
  printf("%d\n", (unsigned int) INT_MAX > INT_MIN); // false
  printf("%d\n", INT_MAX > (int) INT_MIN); // true

  printf("%d\n", -1 > -2);
  printf("%d\n", -1U > -2); 

  printf("(unsigned int) -1: %u\n", (unsigned int) -1);
  printf("(unsigned int) (INT_MIN - 1): %u\n", (unsigned int) INT_MIN - 1);
}
