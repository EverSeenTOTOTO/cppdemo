#ifndef READ_WRITE_H
#define READ_WRITE_H

#include "../utils.h"
#include "./semaphore.h"

class Tester {
public:
  Tester(std::string const& name, int arrive_time, int action_cost) : name(name), arrive_time(arrive_time), action_cost(action_cost) {}

  std::string name;
  int arrive_time;
  int action_cost;
};

// 读者优先
struct PriorReaderState {
  PriorReaderState() : reader_count(0) {}
  
  int reader_count;

  Semaphore sem_rc;
  Semaphore sem_data;
};

class PriorReader : public Tester {
public:
  PriorReader(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create reader " + name + ", read cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void read(PriorReaderState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.sem_rc.P();
      state.reader_count++;
      timer::report(name + " changed reader count to: " + std::to_string(state.reader_count));

      if (state.reader_count == 1) {
          state.sem_data.P();
          timer::report(name + " is first reader, it locks data");
      }
      state.sem_rc.V();

      timer::report(name + " reading...");
      sleep(action_cost);

      state.sem_rc.P();
      state.reader_count--;

      if (state.reader_count == 0) {
          state.sem_data.V();
          timer::report(name + " is last reader, unlock data");
      }
      state.sem_rc.V();
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
      state.sem_data.P();

      timer::report(name + " writing...");
      sleep(action_cost);

      state.sem_data.V();
      timer::report(name + " left");
  }
};

void test_prior_reader() {
    timer::reset();

    auto threads = std::vector<std::thread>();

    PriorReaderState state;

    PriorReader r1("r1", 0, 1000);
    PriorReader r2("r2", 800, 1000);
    LowerWriter w1("w1", 500, 1000);

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

// 写者优先
struct PriorWriterState {
  PriorWriterState() : reader_count(0), writer_count(0) {}

  int reader_count;
  int writer_count;

  Semaphore sem_rc;
  Semaphore sem_wc;
  Semaphore sem_data;
  Semaphore sem_token;
};

class PriorWriter : public Tester {
public:
  PriorWriter(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create writer " + name + ", write cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void write(PriorWriterState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.sem_wc.P();
      state.writer_count++;
      timer::report(name + " changed writer count to: " + std::to_string(state.writer_count));
      if (state.writer_count == 1) {
          state.sem_token.P();
          timer::report(name + " is first writer, it locks token");
      }
      state.sem_wc.V();

      state.sem_data.P();
      timer::report(name + " writing...");
      sleep(action_cost);
      state.sem_data.V();

      state.sem_wc.P();
      state.writer_count--;
      timer::report(name + " changed writer count to: " + std::to_string(state.writer_count));
      if (state.writer_count == 0) {
          state.sem_token.V();
          timer::report(name + " is last writer, unlock token");
      }
      state.sem_wc.V();
      timer::report(name + " left");
  }
};

class LowerReader : public Tester {
public:
  LowerReader(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create reader " + name + ", read cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void read(PriorWriterState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.sem_token.P();
      timer::report(name + " got token");
      state.sem_rc.P();
      state.reader_count++;
      timer::report(name + " changed reader count to: " + std::to_string(state.reader_count));

      if (state.reader_count == 1) {
          state.sem_data.P();
          timer::report(name + " is first reader, it locks data");
      }
      state.sem_rc.V();
      state.sem_token.V();

      timer::report(name + " unlocked token, now reading...");
      sleep(action_cost);

      state.sem_rc.P();
      state.reader_count--;

      if (state.reader_count == 0) {
          state.sem_data.V();
          timer::report(name + " is last reader, unlock data");
      }
      state.sem_rc.V();
      timer::report(name + " left");

  }
};

void test_prior_writer() {
    timer::reset();

    auto threads = std::vector<std::thread>();

    PriorWriterState state;

    PriorWriter w1("w1", 800, 1000);
    LowerReader r1("r1", 0, 1000);
    LowerReader r2("r2", 800, 1000);

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


// 公平竞争
struct FairState {
  FairState() : reader_count(0) {}

  int reader_count;

  Semaphore sem_rc;
  Semaphore sem_data;
  Semaphore sem_token;
};

class Reader: public Tester {
public:
  Reader(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create reader " + name + ", read cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void read(FairState& state) {
      sleep(arrive_time);

      timer::report(name + " arrived");
      state.sem_token.P();
      timer::report(name + " got token");
      state.sem_rc.P();
      state.reader_count++;
      timer::report(name + " changed reader count to: " + std::to_string(state.reader_count));

      if (state.reader_count == 1) {
          state.sem_data.P();
          timer::report(name + " is first reader, it locks data");
      }
      state.sem_rc.V();
      state.sem_token.V();

      timer::report(name + " unlocked token, now reading...");
      sleep(action_cost);

      state.sem_rc.P();
      state.reader_count--;

      if (state.reader_count == 0) {
          state.sem_data.V();
          timer::report(name + " is last reader, unlock data");
      }
      state.sem_rc.V();
      timer::report(name + " left");
  }
};

class Writer: public Tester {
public:
  Writer(std::string const& name, int arrive_time, int action_cost) : Tester(name, arrive_time, action_cost) {
    timer::report("create writer " + name + ", write cost " + std::to_string(action_cost) + "ms" + ", will arrive at " + std::to_string(arrive_time) + "ms");
  }

  void write(FairState& state) {
    sleep(arrive_time);

    timer::report(name + " arrived");
    state.sem_token.P();
    timer::report(name + " got token");
    state.sem_data.P();
    timer::report(name + " writing...");
    sleep(action_cost);
    state.sem_data.V();
    state.sem_token.V();
    timer::report(name + " left");
  }
};

void test_fair() {
    timer::reset();

    auto threads = std::vector<std::thread>();

    FairState state;

    Reader r1("r1", 0, 1000);
    Reader r2("r2", 500, 1000);
    Writer w1("w1", 800, 1000);

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
