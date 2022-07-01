#include "./read_write_channel.h"

// 读优先调度
void Dispatcher::prior_reader() {
  while (true) {
    auto msg = channel.read();
    switch (msg.type) {
      case MessageType::Arrived:
        timer::report(msg.data->name + " arrived");
        pr_arrive(msg.data);
        break;
      case MessageType::Left:
        timer::report(msg.data->name + " left");
        pr_left(msg.data);
        break;
      default:
        throw std::runtime_error("Unknown message type: " + std::to_string(msg.type));
    }
  }
}

// 写优先调度
void Dispatcher::prior_writer() {
  while (true) {
    auto msg = channel.read();
    switch (msg.type) {
      case MessageType::Arrived:
        timer::report(msg.data->name + " arrived");
        wr_arrive(msg.data);
        break;
      case MessageType::Left:
        timer::report(msg.data->name + " left");
        wr_left(msg.data);
        break;
      default:
        throw std::runtime_error("Unknown message type: " + std::to_string(msg.type));
    }
  }
}

void Dispatcher::pr_arrive(Actor* actor) {
  if (actor->is_reader) {
    pending_readers.push(actor);
    if (!is_writing) { allow_read(); }
  } else {
    pending_writers.push(actor);
    if (!is_writing && reading_count == 0) { allow_write(); }
  }
}

void Dispatcher::pr_left(Actor* actor) {
  if (actor->is_reader) {
    reading_count--;
  } else {
    is_writing = false;

    // reader prior
    while (!pending_readers.empty()) { allow_read(); }
  }

  if (reading_count == 0) { allow_write(); }
}

void Dispatcher::wr_arrive(Actor* actor) {
  if (actor->is_reader) {
    pending_readers.push(actor);
    if (!is_writing && pending_writers.empty()) { allow_read(); }
  } else {
    pending_writers.push(actor);
    if (!is_writing && reading_count == 0) { allow_write(); }
  }
}

void Dispatcher::wr_left(Actor* actor) {
  if (actor->is_reader) {
    reading_count--;
  } else {
    is_writing = false;
  }

  // 唤醒写者时不是while !empty，写者一次只能唤醒一个
  if (!pending_writers.empty()) { allow_write(); }

  if (!is_writing) {
    while (!pending_readers.empty()) { allow_read(); }
  }
}

void Dispatcher::allow_read() {
  if (!pending_readers.empty()) {
    auto reader = pending_readers.front();
    pending_readers.pop();
    Message<Actor*> msg(MessageType::AllowRead, reader);
    reader->channel.write(msg);
    reading_count++;
  }
}

void Dispatcher::allow_write() {
  if (!pending_writers.empty()) {
    auto writer = pending_writers.front();
    pending_writers.pop();
    Message<Actor*> msg(MessageType::AllowWrite, writer);
    writer->channel.write(msg);
    is_writing = true;
  }
}

Actor::Actor(std::string const& name, int arrive_time, int action_cost, bool is_reader)
    : name(name), arrive_time(arrive_time), action_cost(action_cost), is_reader(is_reader) {}

void Actor::run(Dispatcher& dispacher) {
  sleep(arrive_time);

  Message<Actor*> msg = Message<Actor*>(MessageType::Arrived, this);
  dispacher.channel.write(msg);

  channel.read();  // must be AllowRead/AllowWrite because dispacher only send once for each
                   // actor channel
  timer::report(name + " doing action");
  sleep(action_cost);

  msg = Message<Actor*>(MessageType::Left, this);
  dispacher.channel.write(msg);
}

void test_dispatcher() {
  Actor                    r1("r1", 0, 1000, true);
  Actor                    r2("r2", 800, 1000, true);
  Actor                    w1("w1", 500, 1000, false);
  Actor                    w2("w2", 900, 1000, false);
  Dispatcher               dispatcher;
  std::vector<std::thread> threads;

  threads.emplace_back(std::thread([&]() { dispatcher.prior_writer(); }));
  threads.emplace_back(std::thread([&]() { r1.run(dispatcher); }));
  threads.emplace_back(std::thread([&]() { r2.run(dispatcher); }));
  threads.emplace_back(std::thread([&]() { w1.run(dispatcher); }));
  threads.emplace_back(std::thread([&]() { w2.run(dispatcher); }));

  for (auto& t : threads) { t.join(); }
}
