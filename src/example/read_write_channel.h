#ifndef READ_WRITE_CHANNEL_H
#define READ_WRITE_CHANNEL_H

#include <queue>

#include "./parallel_queue.h"
#include "../utils.h"

enum MessageType { Arrived, Left, AllowRead, AllowWrite };

template <typename T>
struct Message {
  MessageType type;
  T           data;

  Message(MessageType type, T data) : type(type), data(data) {}
};

template <typename T>
using Channel = ParallelQueue<Message<T*>>;

class Actor;

class Dispatcher {
  using ActorQueue = std::queue<Actor*>;

 public:
  Channel<Actor> channel;

  // 读优先调度
  void prior_reader();
  // 写优先调度
  void prior_writer();

 protected:
  void pr_arrive(Actor* actor);
  void pr_left(Actor* actor);
  void wr_arrive(Actor* actor);
  void wr_left(Actor* actor);
  void allow_read();
  void allow_write();

 private:
  ActorQueue pending_readers;
  ActorQueue pending_writers;
  bool       is_writing    = false;
  size_t     reading_count = 0;
};

class Actor {
 public:
  Actor(std::string const& name, int arrive_time, int action_cost, bool is_reader);

  void run(Dispatcher& dispacher);

  Channel<Actor>     channel;
  std::string const& name;
  int                arrive_time;
  int                action_cost;
  bool               is_reader;
};

void test_dispatcher();
#endif
