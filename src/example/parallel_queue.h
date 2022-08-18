#ifndef PARALLEL_QUEUE_H
#define PARALLEL_QUEUE_H
#include "../common.h"
#include "./semaphore.h"

template <typename T>
class ParallelQueue {
 public:
  ParallelQueue(size_t length = 1) {
    this->mutex = new Semaphore(1);
    this->slots = new Semaphore(length);
    this->items = new Semaphore(0);
  }

  ~ParallelQueue() {
    delete mutex;
    delete slots;
    delete items;
  }

  void write(T const& item) {
    slots->P();
    mutex->P();
    queue.push(item);
    mutex->V();
    items->V();
  }

  auto read() {
    items->P();
    mutex->P();
    auto data = queue.front();
    queue.pop();
    mutex->V();
    slots->V();
    return data;
  }

  size_t size() {
    mutex->P();
    size_t size = queue.size();
    mutex->V();
    return size;
  }

 private:
  std::queue<T> queue;
  Semaphore*    mutex;
  Semaphore*    items;
  Semaphore*    slots;
};
#endif
