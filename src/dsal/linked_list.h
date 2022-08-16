#include "../common.h"

template <typename T>
class SinglyList {
 public:
  struct Node {
    T     data;
    Node* next = nullptr;

    Node() = delete;
    Node(T const& t) : data(t){};
  };

  SinglyList() = default;
  SinglyList(std::initializer_list<T> list) {
    for (auto item : list) {
      append(item);
    }
  }

  ~SinglyList() {
    auto ptr = head;

    while (ptr != nullptr) {
      head = ptr->next;
      delete ptr;
      ptr = head;
    }

    head = nullptr;
  }

  auto first() const {
    return head;
  }

  auto last() const {
    if (empty()) return head;

    auto ptr = head;
    while (ptr->next != nullptr) {
      ptr = ptr->next;
    }

    return ptr;
  }

  void append(T const& value) {
    auto tail = last();

    if (tail) {
      tail->next = new Node(value);
    } else {
      head = new Node(value);
    }
  }

  void prepend(T const& value) {
    auto node = new Node(value);

    node->next = head;
    head       = node;
  }

  size_t len() const {
    auto ptr   = head;
    auto count = 0;

    while (ptr) {
      count++;
      ptr = ptr->next;
    }

    return count;
  }

  void remove_all(T const& value) {
    Node* prev = nullptr;
    auto  ptr  = head;

    while (ptr != nullptr) {
      if (ptr->data == value) {
        auto next = ptr->next;

        if (prev == nullptr) {  // ptr == head
          head = next;
        } else {
          prev->next = next;
        }

        delete ptr;
        ptr = next;
      } else {
        prev = ptr;
        ptr  = ptr->next;
      }
    }
  }

  bool empty() const {
    return head == nullptr;
  }

  void reverse() {
    Node* tail = nullptr;

    std::function<void(Node*)> reverse_helper = [&tail, &reverse_helper](Node* node) {
      if (node && node->next) {
        reverse_helper(node->next);
        node->next->next = node;
        node->next       = nullptr;
      } else {
        tail = node;
      }
    };

    reverse_helper(head);
    head = tail;
  }

 private:
  Node* head = nullptr;
};

SinglyList<int>::Node* find_max(SinglyList<int> const& l);

void test_singly_list();
void test_find_max();
