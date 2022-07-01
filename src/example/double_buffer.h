#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H
#include <chrono>
#include <future>
#include <string>
#include <vector>

#include "../utils.h"
#include "./thread_pool.h"

using namespace std::chrono_literals;

class RenderObject {
 public:
  RenderObject(std::string name);

  void render();
  void update();

 private:
  std::string unit;
  std::string name;
};

class Scene {
 public:
  Scene(size_t buffer_size);

  ~Scene();

  size_t size();
  void   render(int delay);
  void   update(int delay);

 protected:
  void swap_buffers();

 private:
  std::vector<RenderObject *> *current_buffer;
  std::vector<RenderObject *> *next_buffer;
};

template <size_t FPS>
auto compute_pixel_cost(size_t pixel_count);

void test_double_buffer();
#endif  // DOUBLE_BUFFER_H
