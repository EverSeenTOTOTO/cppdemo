#ifndef SEMAPHONE_H
#define SEMAPHONE_H
#include <condition_variable>
#include <iostream>
#include <mutex>

class Semaphore {
 public:
  Semaphore(int value = 1) : value(value) {}

  void P() {
    std::unique_lock<std::mutex> lock(mtx);
    // Note that lock must be acquired before entering this method, and it is reacquired after wait(lock) exits, which means that lock can be used to guard access to stop_waiting().
    // see https://en.cppreference.com/w/cpp/thread/condition_variable/wait
    cv.wait(lock, [this]() { return value > 0; });
    value--;
  }

  void V() {
    std::unique_lock<std::mutex> lock(mtx);
    value++;
    cv.notify_one();
  }

 private:
  std::mutex mtx;
  std::condition_variable cv;
  int value;
};
#endif
