// #include "example/semaphore.h"
// #include "example/read_write_channel.h"
// #include "example/unsigned.h"
#include "./example/sort.h"

int main() {
  using namespace std;

  // test_semaphore();
  // test_unsigned();
  // test_dispatcher();
  // test_slice();
  // test_bubble_sort();
  // test_quick_sort();
  test_quick_sort_parallel();

  return 0;
}
