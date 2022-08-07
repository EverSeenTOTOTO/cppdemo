#ifndef UTILS_H
#define UTILS_H
#include "./common.h"

inline void expect(bool expr, std::string const& message) {
  if (!expr) {
    std::cout << "FAIL: " << message << std::endl;
    throw new std::runtime_error(message);
  } else {
    std::cout << "PASS: " << message << std::endl;
  }
}

inline void expect_not(bool expr, std::string const& message) {
  expect(!expr, message);
}

template <typename T>
void expect_eq(vec<T> const& a, vec<T> const& b, std::string const& message) {
  auto p = std::mismatch(a.begin(), a.end(), b.begin());

  expect(a.size() == b.size() && p.first == a.end() && p.second == b.end(), message);
}

template <typename T>
void expect_eq(T const& a, T const& b, std::string const& message) {
  expect(a == b, message);
}

template <typename Vec>
inline void swap(Vec& v, size_t a, size_t b) {
  auto temp = v[a];
  v[a]      = v[b];
  v[b]      = temp;
}

template<typename T>
inline void display(vec<T> const& v) {
  using namespace std;

  for_each(v.begin(), v.end(), [](auto x) { cout << x << "\t"; });
  cout << endl;
}

inline vec<int>& range(int from, int to) {
  vec<int>* v = new vec<int>;

  for (auto i = from; to >= from ? i < to : i > to; to >= from ? ++i : --i) {
    v->push_back(i);
  }

  return *v;
}

inline int random_integer(int from, int to) {
  std::random_device                 r;
  std::mt19937                       eng(r());
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
