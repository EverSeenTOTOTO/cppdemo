#include "./thread_pool.h"
#include <future>
#include <thread>

ThreadPool::ThreadPool(size_t length) {
  this->mutex      = new Semaphore(1);
  this->task_count = new Semaphore(0);

  for (size_t i = 0; i < length; ++i) {
    std::thread worker([this]() {
      while (1) {
        task_count->P();
        mutex->P();
        auto task = pending.front();
        pending.pop();
        mutex->V();
        task();
      }
    });

    // 因为直接detach了，想要等待worker执行完的话可以借助promise
    worker.detach();
  }
}

ThreadPool::~ThreadPool() {
  delete mutex;
  delete task_count;
}

void ThreadPool::submit(Task const &task) {
  mutex->P();
  pending.push(task);
  mutex->V();
  task_count->V();
}

void test_thread_pool() {
  ThreadPool         pool(4);
  std::promise<void> p;

  for (auto i = 0; i < 8; ++i) {
    pool.submit([i, &p]() {
      auto id = std::this_thread::get_id();

      std::cout << i << " entered." << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      std::cout << i << " exit." << std::endl;

      if (i == 7) { p.set_value(); }
    });
  }

  p.get_future().wait();
}
