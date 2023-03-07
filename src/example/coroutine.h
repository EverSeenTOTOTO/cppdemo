#include <csetjmp>
#include "../common.h"

void test_setjmp();

enum CoroutineState { Initial = 0, Pending, Finished };

class CoroutineManager {
 public:
  using callback = std::function<void()>;

  struct Coroutine {
    jmp_buf         env;
    CoroutineState  state;
    callback const& cb;

    Coroutine(callback&& fn) : state(CoroutineState::Initial), cb(move(fn)) {}
  };

  ~CoroutineManager();

  Coroutine* create(callback&& fn);
  void       yield();
  void       resume(Coroutine* co);

 private:
  vec<Coroutine*> coroutines;
  Coroutine*      current = nullptr;
  jmp_buf         main_env;
};

void test_coroutine();
