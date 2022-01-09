#include <cmath>

// 整数阶乘
template<size_t N>
struct fact {
  static constexpr size_t value = N * fact<N-1>::value;
};

template<>
struct fact<0> {
  static constexpr size_t value = 1.0;
};

// 利用麦克劳林级数计算自然对数e
template<size_t N>
struct eular {
  static constexpr double value = 1.0 / (double)(fact<N>::value) + eular<N - 1>::value;
};

template<>
struct eular<0> {
  static constexpr double value = 1.0;
};

static constexpr auto Eular = eular<10>::value;

// 判断是偶数
template<size_t N>
struct is_even {
  static constexpr bool value = N % 2 == 0;
};

// 整数幂
template<int val, size_t N>
struct power {
  static constexpr auto value = val * power<val, N - 1>::value;
};

template<int val>
struct power<val, 0> {
  static constexpr auto value = 1.0;
};

// Ramanujan 什么神仙公式，效率好高啊
template<size_t N>
struct pi_helper {
  static constexpr double value = fact<4 * N>::value / power<fact<N>::value, 4>::value
    * (26390.0 * N + 1103.0) / power<396, 4 * N>::value
    + pi_helper<N - 1>::value;
};

template<>
struct pi_helper<0> {
  static constexpr double value = 1103.0;
};

template<size_t N>
struct pi {
  static constexpr double value = 9801.0 / 2.0 / std::sqrt(2.0) / pi_helper<N>::value;
};

static constexpr auto PI = pi<2>::value;