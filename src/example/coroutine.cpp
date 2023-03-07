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

CoroutineManager::~CoroutineManager() {
  for (auto &co : coroutines) {
    delete co;
  }
}

CoroutineManager::Coroutine *CoroutineManager::create(CoroutineManager::callback &&fn) {
  Coroutine *co = new Coroutine(move(fn));

  coroutines.push_back(co);

  return co;
}

void CoroutineManager::yield() {
  if (setjmp(current->env) == 0) {  // run to next iteration -> 0; jump back here -> 1
    longjmp(main_env, 1);           // yield to main
  }
}

void CoroutineManager::resume(Coroutine *co) {
  if (co->state == CoroutineState::Finished) {
    return;
  }

  if (setjmp(main_env) == 0) {
    current = co;  // must occur before cb()

    if (co->state == CoroutineState::Initial) {
      co->state = CoroutineState::Pending;
      co->cb();
      return;
    }

    longjmp(co->env, 1);  // resume coroutine
  }
}

void test_coroutine() {
  CoroutineManager co;

  auto g = co.create([&co] {
    for (auto i = 0; i < 4; ++i) {
      print(i);
      co.yield();
    }
  });

  co.resume(g);
  co.resume(g);
  co.resume(g);
  co.resume(g);
}
