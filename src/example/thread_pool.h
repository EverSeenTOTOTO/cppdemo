#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

#include "./semaphore.h"

class ThreadPool {
 public:
  typedef std::function<void(void)> Task;
  ThreadPool(size_t length) {
    this->mutex = new Semaphore(1);
    this->task_count = new Semaphore(0);
    for (size_t i = 0; i < length; ++i) {
      std::thread worker([this]() {
        while (1) {
          task_count->P();
          mutex->P();
          auto task = tasks.front();
          tasks.pop();
          mutex->V();
          task();
        }
      });
      worker.detach();
    }
  }

  ~ThreadPool() {
    delete mutex;
    delete task_count;
  }

  void submit(Task const &task) {
    mutex->P();
    tasks.push(task);
    mutex->V();
    task_count->V();
  }

 private:
  std::queue<Task> tasks;
  Semaphore *mutex;
  Semaphore *task_count;
};
#endif
