#ifndef COUNTER_H
#define COUNTER_H
#include <string>
#include "../common.h"
#include "../utils.h"

class Counter {
 public:
  void inc() {
    std::lock_guard<std::mutex> g{mtx};
    count++;
  }

  void dec() {
    std::lock_guard<std::mutex> g{mtx};
    count--;
  }

  void reset() {
    std::lock_guard<std::mutex> g{mtx};
    count = 0;
  }

  void test(size_t thread_count) {
    reset();
    timer::reset();

    std::list<std::thread> ts;

    size_t each = 1e6;
    for (auto i = 0; i < thread_count; ++i) {
      ts.emplace_back(std::thread([&]() {
        for (size_t i = 0; i < each; ++i) {
          this->inc();
        }
      }));
    }

    for (auto& t : ts) {
      t.join();
    }

    expect_eq(count, each * thread_count, std::to_string(timer::count()));
  }

 private:
  size_t     count = 0;
  std::mutex mtx;
};

#endif
