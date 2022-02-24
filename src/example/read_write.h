#ifndef READ_WRITE_H
#define READ_WRITE_H
#include <mutex>
#include <condition_variable>

#include "../utils.h"
#include "./semaphore.h"

struct PriorReaderState {
  PriorReaderState() : reader_count(0) {}
  
  int reader_count;

  std::mutex rcm;
  std::mutex wm;
};

class Tester {
public:
  Tester(std::string const& name, int arrive_time, int action_cost) : name(name), arrive_time(arrive_time), action_cost(action_cost) {}

  std::string name;
  int arrive_time;
  int action_cost;
};

// 读者优先
class PriorReader : public Tester {
public:
  PriorReader(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create reader " + name + ", read cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void read(PriorReaderState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.rcm.lock();
      state.reader_count++;
      timer::report(name + " changed reader count to: " + std::to_string(state.reader_count));

      if (state.reader_count == 1) {
          state.wm.lock();
          timer::report(name + " is first reader, it locks for write so that later writers should wait until no reader is reading and pending");
      }
      state.rcm.unlock();

      timer::report(name + " reading...");
      sleep(action_cost);

      state.rcm.lock();
      state.reader_count--;

      if (state.reader_count == 0) {
          state.wm.unlock();
          timer::report(name + " is last reader, unlock write");
      }
      state.rcm.unlock();
      timer::report(name + " left");
  }
};

class LowerWriter : public Tester {
public:
  LowerWriter(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create writer " + name + ", write cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void write(PriorReaderState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.wm.lock();

      timer::report(name + " writing...");
      sleep(action_cost);

      state.wm.unlock();
      timer::report(name + " left");
  }
};

void test_prior_reader() {
    timer::reset();

    auto threads = std::vector<std::thread>();

    PriorReaderState state;

    PriorReader r1("r1", 0, 1000);
    PriorReader r2("r2", 1200, 1000);
    LowerWriter w1("w1", 500, 500);

    threads.emplace_back(std::thread([&]() {
          r1.read(state);
    }));
    threads.emplace_back(std::thread([&]() {
          r2.read(state);
    }));
    threads.emplace_back(std::thread([&]() {
          w1.write(state);
    }));

    for(auto& t : threads) {
        t.join();
    }
}

struct PriorWriterState {
  PriorWriterState() : {}

  int writer_count;

  std::mutex rm;
  std::mutex wcm;
};

class PriorWriter : public Tester {
public:
  PriorWriter(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create writer " + name + ", write cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void write(PriorWriterState& state) {
  }
};

class LowerReader : public Tester {
public:
  LowerReader(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create reader " + name + ", read cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void read(PriorWriterState& state) {
  }
};

void test_prior_writer() {
    timer::reset();

    auto threads = std::vector<std::thread>();

    PriorWriterState state;

    LowerReader r1("r1", 0, 1000);
    LowerReader r2("r2", 500, 1000);
    PriorWriter w1("w1", 800, 1000);

    threads.emplace_back(std::thread([&]() {
          r1.read(state);
    }));
    threads.emplace_back(std::thread([&]() {
          r2.read(state);
    }));
    threads.emplace_back(std::thread([&]() {
          w1.write(state);
    }));

    for(auto& t : threads) {
        t.join();
    }
}
#endif
