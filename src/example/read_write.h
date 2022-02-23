#ifndef READ_WRITE_H
#define READ_WRITE_H
#include <mutex>
#include <condition_variable>

#include "../utils.h"
#include "./semaphore.h"

struct PriorReaderState {
  PriorReaderState() : reader_count(0) {}
  
  int reader_count;

  Semaphore rs;
  Semaphore ws;
};

class Tester {
public:
  Tester(std::string const& name, int arrive_time, int action_cost) : name(name), arrive_time(arrive_time), action_cost(action_cost) {}

  std::string name;
  int arrive_time;
  int action_cost;
};

class PriorReader : public Tester {
public:
  PriorReader(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create reader " + name + ", read cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void read(PriorReaderState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.rs.P();
      state.reader_count++;
      timer::report(name + " entered" + ", reader count " + std::to_string(state.reader_count));

      if (state.reader_count == 1) {
          state.ws.P();
          timer::report(name + " is first reader, it locks for write so that later writers should wait until no reader is reading");
      }
      state.rs.V();

      timer::report(name + " reading...");
      sleep(action_cost);

      state.rs.P();
      state.reader_count--;
      state.rs.V();
      timer::report(name + " left");

      if (state.reader_count == 0) {
          state.ws.V();
          timer::report(name + " is last reader, unlock write");
      }
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
      state.ws.P();
      timer::report(name + " entered");

      timer::report(name + " writing...");
      sleep(action_cost);

      state.ws.V();
      timer::report(name + " left");
  }
};

void test_prior_reader() {
    timer::reset();

    auto threads = std::vector<std::thread>();

    PriorReaderState state;

    PriorReader r1("r1", 0, 1000);
    PriorReader r2("r2", 800, 1000);
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

#endif
