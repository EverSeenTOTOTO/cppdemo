#ifndef SEMAPHONE_H
#define SEMAPHONE_H
#include <iostream>
#include <mutex>
#include <condition_variable>

class Semaphore {
    public:
        Semaphore(int value = 1): value(value) {}

        void P() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return value > 0;
            });
            value --;
        }

        void V() {
            std::unique_lock<std::mutex> lock(mtx);
            value ++;
            cv.notify_one();
        }

    private:
        std::mutex mtx;
        std::condition_variable cv;
        int value;
};
#endif
