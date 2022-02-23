#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H
#include <vector>
#include <string>
#include <chrono>
#include <future>

#include "../utils.h"
#include "./threadPool.h"

using namespace std::chrono_literals;

class RenderObject {
public:
    RenderObject(std::string name) : name(name), unit(name) {}

    void render() {
        std::cout << "Render " << name << std::endl;
    }

    void update() {
      std::cout << "Update " << name << " to " << name + unit << std::endl;
      name = name + unit;
    }

private:
    std::string unit;
    std::string name;
};

class Scene {
public:
  Scene(size_t buffer_size) {
    this->current_buffer = new std::vector<RenderObject*>(buffer_size);
    this->next_buffer = new std::vector<RenderObject*>(buffer_size);

    for (size_t i = 0; i < buffer_size; i++) {
      current_buffer->at(i) = new RenderObject(std::to_string(i));
      next_buffer->at(i) = new RenderObject(std::to_string(i));
    }
  }

  ~Scene() {
    delete current_buffer;
    delete next_buffer;
  }

  size_t size() {
    return current_buffer->size();
  }

  void render(int delay) {
    for(auto object : *current_buffer) {
      object->render();
      sleep(delay);
    }
  }

  void update(int delay) {
    for(auto object : *next_buffer) {
      object->update();
      sleep(delay);
    }
    swap_buffers();
  }

protected:
  void swap_buffers() {
    std::vector<RenderObject*>* tmp = current_buffer;
    current_buffer = next_buffer;
    next_buffer = tmp;
  }

private:
  std::vector<RenderObject*> *current_buffer;
  std::vector<RenderObject*> *next_buffer;
};

// template<size_t FPS>
// void create_loop(std::string const& name, std::function<void()> const& action) {
//   using namespace std::chrono;
//   auto rate = duration<double, std::ratio<1, FPS>>(1s);
//   size_t frame = 1000 / rate.count();
// 
//   while(true) {
//     auto begin = high_resolution_clock::now();
// 
//     action();
// 
//     auto elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - begin).count();
// 
//     // 如果时间不足一帧，则等待。模拟设备的固定速率
//     if (elapsed < frame) {
//       std::this_thread::sleep_for(milliseconds(frame - elapsed));
//     }
//   }
// }
// 

template<size_t FPS>
auto compute_pixel_cost(size_t pixel_count) {
  auto rate = std::chrono::duration<size_t, std::ratio<1, FPS>>(1s);
  auto frame = 1000 / rate.count();

  return frame / pixel_count;
}

void test_double_buffer() {
    auto threadPool = new ThreadPool(2);

    auto scene = new Scene(3);
    auto p = std::promise<void>(); // never fulfilled

    auto render_speed = compute_pixel_cost<10>(scene->size());
    auto gpu_speed = compute_pixel_cost<10>(scene->size());

    // 模拟渲染设备
    threadPool->submit([&]() {
        while(true) {
            scene->render(render_speed);
        }
        p.set_value();
    });

    // 模拟GPU
    threadPool->submit([&]() {
        while(true) {
            scene->update(gpu_speed);
        }
    });

    p.get_future().wait();
}
#endif // DOUBLE_BUFFER_H
