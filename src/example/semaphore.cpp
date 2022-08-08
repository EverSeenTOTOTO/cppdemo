#include "./semaphore.h"

Semaphore::Semaphore(int value) : value(value) {}

void Semaphore::P() {
  std::unique_lock<std::mutex> lock(mtx);
  // Note that lock must be acquired before entering this method, and it is reacquired after
  // wait(lock) exits, which means that lock can be used to guard access to stop_waiting(). see
  // https://en.cppreference.com/w/cpp/thread/condition_variable/wait
  cv.wait(lock, [this]() { return value > 0; });
  value--;
}

void Semaphore::V() {
  std::unique_lock<std::mutex> lock(mtx);
  value++;
  cv.notify_one();
}

void test_semaphore() {
  Semaphore                s(2);
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
