#include "../common.h"

vec<int>& base_convertion(int number, size_t base);

void test_base_conversion();

template <typename T>
class circular_deque {
 public:
  circular_deque() = default;
  circular_deque(size_t capacity) : capacity(capacity + 1) {
    base = new T[capacity];  // one extra space to distinguish empty or full
  }

  ~circular_deque() {
    if (base != nullptr) {
      delete[] base;
      base = nullptr;
    }
  }

  void push_back(T const& value) {
    check_full();

    base[tail] = value;
    tail       = (tail + 1) % capacity;
  }

  auto pop_front() {
    check_empty();

    T value = base[head];
    head       = (head + 1) % capacity;

    return value;
  }

  void push_front(T const& value) {
    check_full();

    head       = (head + capacity - 1) % capacity;
    base[head] = value;
  }

  auto pop_back() {
    check_empty();

    tail = (tail + capacity - 1) % capacity;

    return base[tail];
  }

  inline size_t len() const {
    return (tail - head + capacity) % capacity;
  }

  inline bool empty() const {
    return head == tail;
  }

  inline bool full() const {
    return capacity <= 1 || (tail + 1) % capacity == head;
  }

  inline size_t max_len() const {
    return capacity - 1;
  }

 protected:
  void check_full() {
    if (full()) {
      throw std::runtime_error("queue is full");
    }
  }

  void check_empty() {
    if (empty()) {
      throw std::runtime_error("queue is empty");
    }
  }

 private:
  T*     base     = nullptr;
  size_t capacity = 1;
  size_t head     = 0;
  size_t tail     = 0;
};

void test_circular_deque();
