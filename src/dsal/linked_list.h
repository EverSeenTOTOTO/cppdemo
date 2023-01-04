#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../common.h"

template <typename T>
class singly_list {
 public:
  struct list_node {
    T          data;
    list_node* next = nullptr;

    list_node() = delete;
    list_node(T const& t) : data(t){};
  };

  singly_list() = default;
  singly_list(std::initializer_list<T> list) {
    for (auto item : list) {
      append(item);
    }
  }

  ~singly_list() {
    auto ptr = head;

    while (ptr != nullptr) {
      head = ptr->next;
      delete ptr;
      ptr = head;
    }

    head = nullptr;
  }

  inline auto first() const {
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
      tail->next = new list_node(value);
    } else {
      head = new list_node(value);
    }
  }

  void prepend(T const& value) {
    auto node = new list_node(value);

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
    list_node* prev = nullptr;
    auto       ptr  = head;

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

  inline bool empty() const {
    return head == nullptr;
  }

  void reverse() {
    list_node* tail = nullptr;

    std::function<void(list_node*)> reverse_helper = [&tail, &reverse_helper](list_node* node) {
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
  list_node* head = nullptr;
};

singly_list<int>::list_node* find_max(singly_list<int> const& l);

void test_singly_list();
void test_find_max();

#endif
