#ifndef COMMON_H
#define COMMON_H

#include <ostream>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <type_traits>
#include <vector>
#include <forward_list>
#include <list>
#include <stack>
#include <queue>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <regex>
#include <chrono>
#include <iomanip>
#include <random>
#include <thread>
#include <future>
#include <functional>
#include <algorithm>
#include <exception>
#include <atomic>
#include <initializer_list>

#include <cstddef>
#include <cmath>
#include <climits>
#include <cstdio>

template <typename T>
using vec = std::vector<T>;

template <typename K, typename V>
using hashmap = std::unordered_map<K, V>;

#endif
