#ifndef SEMAPHONE_H
#define SEMAPHONE_H
#include "../common.h"

class Semaphore2 {
 public:
  Semaphore2(int value = 1);

  void P();
  void V();

 private:
  std::atomic_bool lock;
  int              value;
};

void test_semaphore2();
#endif
