#include <csetjmp>
#include "../common.h"

void test_setjmp();

enum CoroutineState { Initial = 0, Pending, Fulfilled, Rejected };

class CoroutineManager {
 public:
  using callback = std::function<void()>;

  struct Coroutine {
    jmp_buf                           env;
    CoroutineState                    state;
    callback                          cb;
    std::pair<void*, std::exception*> result;
    Coroutine*                        parent;

    Coroutine(callback&& fn)
        : state(CoroutineState::Initial), cb(move(fn)), result(nullptr, nullptr) {}

    template <typename T>
    T*              get_result() const;
    std::exception* get_exception() const;
  };

  ~CoroutineManager();

  Coroutine* create(callback&& fn);

  // TODO: pass argument
  void yield();
  void resume(Coroutine* co);

  template <typename T>
  void done(T&& any);
  void panic(std::exception* e);

 protected:
  void check_is_in_coroutine(std::string const& method) const;

 private:
  vec<Coroutine*> coroutines;
  Coroutine*      current = nullptr;
  jmp_buf         main_env;
};

void test_coroutine();
void test_coroutine_nested();
