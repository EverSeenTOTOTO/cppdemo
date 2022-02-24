#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <queue>
#include <functional>
#include <thread>
#include <condition_variable>

#include "./semaphore.h"

class ThreadPool {
  public:
    typedef std::function<void(void)> Task;
    ThreadPool(size_t length) {
      this->mutex = new Semaphore(1);
      this->taskCount = new Semaphore(0);
      for(size_t i=0; i<length; ++i) {
        std::thread worker([this]() {
          while(1) {
            taskCount->P();
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
      delete taskCount;
    }

    void submit(Task const& task) {
      mutex->P();
      tasks.push(task);
      mutex->V();
      taskCount->V();
    }

  private:
    std::queue<Task> tasks;
    Semaphore *mutex;
    Semaphore *taskCount;
};
#endif
