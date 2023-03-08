#include "./coroutine.h"
#include <csetjmp>
#include "../utils.h"

void test_setjmp() {
  jmp_buf main_env;
  jmp_buf co_env;

  if (setjmp(main_env) == 0) {
    for (auto i = 0; i < 10; ++i) {
      print(i);

      if (setjmp(co_env) == 0) {
        longjmp(main_env, 1);  // yield
      }
    }
  } else {
    longjmp(co_env, 1);  // resume
  }
}

std::ostream &operator<<(std::ostream &os, CoroutineState const &state) {
  switch (state) {
    case CoroutineState::Initial:
      os << "Initial";
      break;
    case CoroutineState::Pending:
      os << "Pending";
      break;
    case CoroutineState::Fulfilled:
      os << "Fulfilled";
      break;
    case CoroutineState::Rejected:
      os << "Rejected";
      break;
    default:
      break;
  }

  return os;
}

template <typename T>
T *CoroutineManager::Coroutine::get_result() const {
  return state == CoroutineState::Fulfilled ? static_cast<T *>(result.first) : nullptr;
}

std::exception *CoroutineManager::Coroutine::get_exception() const {
  return state == CoroutineState::Rejected ? static_cast<std::exception *>(result.second) : nullptr;
};

CoroutineManager::~CoroutineManager() {
  for (auto &co : coroutines) {
    delete co;
  }
}

CoroutineManager::Coroutine *CoroutineManager::create(CoroutineManager::callback &&fn) {
  Coroutine *co = new Coroutine(std::move([this, cb = std::move(fn)] {
    try {
      cb();
      done(nullptr);
    } catch (std::exception &e) {
      panic(&e);
    }
  }));

  coroutines.push_back(co);

  return co;
}

void CoroutineManager::yield() {
  check_is_in_coroutine("yield()");

  if (setjmp(current->env) == 0) {  // run to next iteration -> 0; jump back here -> 1
    current = current->parent;
    longjmp(current != nullptr ? current->env : main_env, 1);  // yield to upper coroutine
  }
}

template <typename T>
void CoroutineManager::done(T &&any) {
  check_is_in_coroutine("done()");

  if (current->state == CoroutineState::Initial || current->state == CoroutineState::Pending) {
    current->state        = CoroutineState::Fulfilled;
    current->result.first = &any;
    yield();  // will never return back
  }
}

void CoroutineManager::panic(std::exception *e) {
  check_is_in_coroutine("panic()");

  if (current->state == CoroutineState::Initial || current->state == CoroutineState::Pending) {
    current->state         = CoroutineState::Rejected;
    current->result.second = e;
    yield();  // will never return back
  }
}

void CoroutineManager::resume(Coroutine *co) {
  co->parent = current;
  current    = co;

  if (co->state == CoroutineState::Fulfilled || co->state == CoroutineState::Rejected) {
    return;
  }

  if (setjmp(co->parent != nullptr ? co->parent->env : main_env) == 0) {
    if (co->state == CoroutineState::Initial) {
      co->state = CoroutineState::Pending;
      co->cb();
      return;
    }

    longjmp(co->env, 1);  // resume coroutine
  }
}

void CoroutineManager::check_is_in_coroutine(std::string const &method) const {
  if (current == nullptr) {
    throw std::runtime_error(method + " can only be called in coroutine functions");
  }
}

void test_coroutine() {
  CoroutineManager co;

  auto g = co.create([&co] {
    for (auto i = 0; i < 4; ++i) {
      print(i);
      co.yield();
    }
    co.done(42);
    co.done(24);
  });

  co.resume(g);
  co.resume(g);
  co.resume(g);
  co.resume(g);
  co.resume(g);
  co.resume(g);

  print(*(g->get_result<int>()));
}

void test_coroutine_nested() {
  CoroutineManager co;

  vec<int> v;

  auto h = co.create([&] {
    v.push_back(42);
    co.yield();
    v.push_back(24);
    co.yield();
  });
  auto g = co.create([&] {
    for (auto i = 0; i < 4; ++i) {
      v.push_back(i);
      co.yield();
    }

    co.resume(h);
    co.resume(h);
    co.resume(h);
    // co.resume(h); // FIXME
  });
  auto f = co.create([&] {
    co.resume(g);
    co.resume(g);
    co.resume(g);
    co.resume(g);
    co.resume(g);
    co.resume(g);
    co.resume(g);
    co.resume(g);
  });

  co.resume(f);

  print(v);
}
