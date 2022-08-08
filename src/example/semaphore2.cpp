#include "./semaphore2.h"
#include "../utils.h"

Semaphore2::Semaphore2(int value) : value(value), lock(true) {}

void Semaphore2::P() {
  int  t      = 0;
  bool locked = false;

  while (true) {
    // lock
    while (lock.compare_exchange_weak(locked, true, std::memory_order_acquire)) {
      if (t++ > 500) sleep(1);  // spin -> sleep
    }

    if (value > 0) {
      value--;
      lock.compare_exchange_weak(locked, false, std::memory_order_release);
      break;
    }

    // quickly unlock
    lock.compare_exchange_weak(locked, false, std::memory_order_release);
  }
}

void Semaphore2::V() {
  int  t      = 0;
  bool locked = false;

  while (lock.compare_exchange_weak(locked, true, std::memory_order_acquire)) {
    if (t++ > 500) sleep(1);
  }

  value++;
  lock.compare_exchange_weak(locked, false, std::memory_order_release);
}

void test_semaphore2() {
  Semaphore2               s(2);
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
