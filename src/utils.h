#ifndef UTILS_H
#define UTILS_H
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

inline int random_integer(int from, int to) {
  std::random_device                 r;
  std::default_random_engine         eng(r());
  std::uniform_int_distribution<int> uniform_dist(from, to);
  return uniform_dist(eng);
}

template <typename Time = std::chrono::milliseconds>
void sleep(int ms) {
  std::this_thread::sleep_for(Time(ms));
}

class timer {
 public:
  static void reset() {
    now = std::chrono::high_resolution_clock::now();
  }

  template <typename Time = std::chrono::milliseconds>
  static double count() {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<Time>(end - now).count();
  }

  template <typename Time = std::chrono::milliseconds>
  static void report(std::string const& message) {
    using namespace std;
    auto time_point = count<Time>();

    auto output = [&time_point, &message](string const& unit) {
      cout << left << setw(4) << "[" << time_point << unit << "]: " << message << endl;
    };

    if constexpr (is_same<Time, chrono::seconds>::value) {
      output("s");
    } else if constexpr (is_same<Time, chrono::milliseconds>::value) {
      output("ms");
    } else if constexpr (is_same<Time, chrono::microseconds>::value) {
      output("us");
    } else if constexpr (is_same<Time, chrono::nanoseconds>::value) {
      output("ns");
    } else {
      output("");
    }
  }

 private:
  static inline std::chrono::high_resolution_clock::time_point now =
      std::chrono::high_resolution_clock::now();
};

#endif
