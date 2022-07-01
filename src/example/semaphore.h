#ifndef SEMAPHONE_H
#define SEMAPHONE_H
#include <condition_variable>
#include <iostream>
#include <mutex>

class Semaphore {
 public:
  Semaphore(int value = 1);

  void P();
  void V();

 private:
  std::mutex              mtx;
  std::condition_variable cv;
  int                     value;
};

void test_semaphore();
#endif
