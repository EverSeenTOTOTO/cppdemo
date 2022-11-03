#include "./semaphore.h"

Semaphore::Semaphore(int value) : value(value) {}

void Semaphore::P() {
  std::unique_lock<std::mutex> lock(mtx);
  // 相当于：
  // 1. while (value > 0) {
  // 2.   unlock();
  // 3.   lock();
  // 4. }
  // 并且要求当进程执行到1和2之间时不会被调度走，要调度也只能在2和3之间
  cv.wait(lock, [this]() { return value > 0; });
  value--;
}

void Semaphore::V() {
  std::unique_lock<std::mutex> lock(mtx);
  value++;
  cv.notify_one();
}

void test_semaphore() {
  Semaphore                s(1);
  std::vector<std::thread> v;

  for (auto i = 0; i < 4; ++i) {
    v.push_back(std::thread([&s]() {
      s.P();

      auto id = std::this_thread::get_id();

      std::cout << id << " entered." << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      std::cout << id << " exit." << std::endl;

      s.V();
    }));
  }

  for (auto& t : v) {
    t.join();
  }
}
