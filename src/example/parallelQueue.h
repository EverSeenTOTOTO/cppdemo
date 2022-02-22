#ifndef PARALLEL_QUEUE_H
#define PARALLEL_QUEUE_H
#include <deque>
#include <optional>

#include "./semaphore.h"

class SimpleParallelQueue {
    public:
        SimpleParallelQueue(size_t length) {
            this->mutex = new Semaphore(1);
            this->slots = new Semaphore(length); // !! size_t -> int
            this->items = new Semaphore(0);
        }

        ~SimpleParallelQueue() {
            delete mutex;
            delete slots;
            delete items;
        }

        void write(int item) {
            slots->P();
            mutex->P();
            this->deque.push_back(item);
            mutex->V();
            items->V();
        }

        std::optional<int> read() { // 不需要optional
            items->P();
            mutex->P();
            std::optional<int> data;
            if(this->deque.size() > 0) {
              data.emplace(this->deque.front());
              this->deque.erase(this->deque.begin());
            }
            mutex->V();
            slots->V();
            return data;
        }

    private:
        std::deque<int> deque;
        Semaphore *mutex;
        Semaphore *slots;
        Semaphore *items;
};
#endif