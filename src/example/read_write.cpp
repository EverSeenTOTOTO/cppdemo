#include "./read_write.h"

void test_prior_reader() {
  timer::reset();

  auto threads = std::vector<std::thread>();

  PriorReaderState state;

  PriorReader r1("r1", 0, 1000);
  PriorReader r2("r2", 800, 1000);
  LowerWriter w1("w1", 500, 1000);

  threads.emplace_back(std::thread([&]() { r1.read(state); }));
  threads.emplace_back(std::thread([&]() { r2.read(state); }));
  threads.emplace_back(std::thread([&]() { w1.write(state); }));

  for (auto& t : threads) { t.join(); }
}

void test_prior_writer() {
  timer::reset();

  auto threads = std::vector<std::thread>();

  PriorWriterState state;

  PriorWriter w1("w1", 800, 1000);
  LowerReader r1("r1", 0, 1000);
  LowerReader r2("r2", 800, 1000);

  threads.emplace_back(std::thread([&]() { r1.read(state); }));
  threads.emplace_back(std::thread([&]() { r2.read(state); }));
  threads.emplace_back(std::thread([&]() { w1.write(state); }));

  for (auto& t : threads) { t.join(); }
}

void test_fair() {
  timer::reset();

  auto threads = std::vector<std::thread>();

  FairState state;

  Reader r1("r1", 0, 1000);
  Reader r2("r2", 500, 1000);
  Writer w1("w1", 800, 1000);

  threads.emplace_back(std::thread([&]() { r1.read(state); }));
  threads.emplace_back(std::thread([&]() { r2.read(state); }));
  threads.emplace_back(std::thread([&]() { w1.write(state); }));

  for (auto& t : threads) { t.join(); }
}
