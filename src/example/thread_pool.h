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

  ThreadPool(size_t length);

  ~ThreadPool();

  void submit(Task const &task);

 private:
  std::queue<Task> pending;
  Semaphore       *mutex;
  Semaphore       *task_count;
};

void test_thread_pool();
#endif
