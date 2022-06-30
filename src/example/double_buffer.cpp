#include "./double_buffer.h"

void test_double_buffer() {
  auto threadPool = new ThreadPool(2);

  auto scene = new Scene(3);
  auto p     = std::promise<void>();  // never fulfilled

  auto render_speed = compute_pixel_cost<10>(scene->size());
  auto gpu_speed    = compute_pixel_cost<10>(scene->size());

  // 模拟渲染设备
  threadPool->submit([&]() {
    while (true) { scene->render(render_speed); }
    p.set_value();
  });

  // 模拟GPU
  threadPool->submit([&]() {
    while (true) { scene->update(gpu_speed); }
  });

  p.get_future().wait();
}
