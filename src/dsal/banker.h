#ifndef BANKER_H

#include "../common.h"
#include "../utils.h"

// banker algorithm

using uvec = vec<size_t>;

namespace dsal_impl {

  template <typename T>
  std::map<T, uvec> const& compute_needed(std::map<T, std::pair<uvec, uvec>> const& table) {
    auto result = new std::map<T, uvec>;

    for (auto [p, info] : table) {
      auto allocated = info.first;
      auto required  = info.second;
      auto needed    = new uvec;

      for (auto i = 0; i < allocated.size(); ++i) {
        needed->push_back(required[i] - allocated[i]);
      }

      result->insert_or_assign(p, *needed);
    }

    return *result;
  }

  inline bool fit(uvec const& needed, uvec const& available) {
    for (auto i = 0; i < needed.size(); i++) {
      if (needed[i] > available[i]) {
        return false;
      }
    }
    return true;
  }

  inline void update_available(uvec& available, uvec const& allocated) {
    for (auto i = 0; i < available.size(); ++i) {
      available[i] += allocated[i];
    }
  }
}  // namespace dsal_impl

template <typename T>
vec<T> const& banker(std::map<T, std::pair<uvec, uvec>> const& table, uvec const& total) {
  auto result = new vec<T>;

  auto needed    = dsal_impl::compute_needed(table);
  auto available = slice(total, 0, total.size());

  while (true) {
    auto changed = false;

    for (auto [p, requirement] : needed) {
      if (dsal_impl::fit(requirement, available)) {
        result->push_back(p);
        changed = true;
        needed.erase(p);
        dsal_impl::update_available(available, table.at(p).first);
        break;
      }
    }

    if (!changed) break;
  }

  return *result;
}

void test_banker();

#endif  // !BANKER_H
