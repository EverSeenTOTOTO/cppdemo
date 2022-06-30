#include "./read_write_channel.h"

void test_dispatcher() {
  Actor                    r1("r1", 0, 1000, true);
  Actor                    r2("r2", 800, 1000, true);
  Actor                    w1("w1", 500, 1000, false);
  Actor                    w2("w2", 900, 1000, false);
  Dispatcher<Actor>        dispatcher;
  std::vector<std::thread> threads;

  threads.emplace_back(std::thread([&]() { dispatcher.prior_writer(); }));
  threads.emplace_back(std::thread([&]() { r1.run(dispatcher); }));
  threads.emplace_back(std::thread([&]() { r2.run(dispatcher); }));
  threads.emplace_back(std::thread([&]() { w1.run(dispatcher); }));
  threads.emplace_back(std::thread([&]() { w2.run(dispatcher); }));

  for (auto& t : threads) { t.join(); }
}
