#ifndef UTILS_H
#define UTILS_H
#include "./common.h"

template <typename T, typename W = int>
class weighted_data {
 public:
  using data_type   = T;
  using weight_type = W;

  T data;
  W weight;

  weighted_data() = delete;
  weighted_data(W const& w) : weight(w) {}
  weighted_data(T const& t, W const& w) : data(t), weight(w) {}
};

template <typename Vec>
inline void swap(Vec& v, size_t a, size_t b) {
  auto temp = v[a];
  v[a]      = v[b];
  v[b]      = temp;
}

template <typename T>
int find_index(vec<T> const& vec, T const& t) {
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i] == t) return i;
  }

  return -1;
}

template <typename T>
vec<T>& slice(vec<T> const& vec, size_t begin, size_t length) {
  auto      bit = vec.begin();
  ::vec<T>* v   = new ::vec<T>;

  for (size_t i = begin; v->size() < length && bit + i < vec.end(); ++i) {
    v->push_back(*(bit + i));
  }

  return *v;
}

namespace dsal_impl {
  // functions cannot be partial specialization
  template <typename T, typename = void>
  struct print_helper {
    void print(T const& any) {
      std::cout << any << "\t";
    }
  };

  template <typename T>
  struct print_helper<
      T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> {
    void print(T const& v) {
      print_helper<typename T::value_type> d;

      for_each(v.begin(), v.end(), [&](auto x) { d.print(x); });
      std::cout << std::endl;
    }
  };

  template <typename T1, typename T2>
  struct print_helper<std::pair<T1, T2>> {
    void print(std::pair<T1, T2> const& v) {
      print_helper<T1> d1;
      print_helper<T2> d2;

      d1.print(v.first);
      d2.print(v.second);
      std::cout << std::endl;
    }
  };
}  // namespace dsal_impl

template <typename T>
inline void print(T const& t) {
  dsal_impl::print_helper<T> d;
  d.print(t);
}

inline vec<int>& range(int from, int to) {
  vec<int>* v = new vec<int>;

  for (auto i = from; to >= from ? i < to : i > to; to >= from ? ++i : --i) {
    v->push_back(i);
  }

  return *v;
}

template <typename T>
bool equals_to(vec<T> const& a, vec<T> const& b) {
  if (a.size() != b.size()) return false;

  auto p =
      std::mismatch(a.begin(), a.end(), b.begin(), [](auto x, auto y) { return equals_to(x, y); });

  return p.first == a.end() && p.second == b.end();
}

template <typename T>
bool equals_to(std::set<T> const& a, std::set<T> const& b) {
  if (a.size() != b.size()) return false;

  for (auto value : a) {
    if (b.find(value) == b.end()) {
      return false;
    }
  }

  return true;
}

template <typename K, typename V>
bool equals_to(std::map<K, V> const& a, std::map<K, V> const& b) {
  if (a.size() != b.size()) return false;

  for (auto const& [key, value] : a) {
    if (!equals_to(b.at(key), value)) {
      return false;
    }
  }

  return true;
}

template <typename T, typename U>
bool equals_to(T const& a, T const& b) {
  return a == b;
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

inline void expect(bool expr, std::string const& message) {
  if (!expr) {
    std::cout << "FAIL: " << message << std::endl;
    throw std::runtime_error(message);
  } else {
    std::cout << "PASS: " << message << std::endl;
  }
}

inline void expect_not(bool expr, std::string const& message) {
  expect(!expr, message);
}

template <typename T, typename U>
void expect_eq(T a, U b, std::string const& message) {
  expect(equals_to<T, U>(a, b), message);
}

template <typename S>
void expect_match(S const& str, std::string const& re, std::string const& message) {
  expect(std::regex_match(str, std::regex(re)), message);
}

#endif
