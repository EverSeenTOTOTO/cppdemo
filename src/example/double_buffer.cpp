#include "./double_buffer.h"

RenderObject::RenderObject(std::string name) : name(name), unit(name) {}

void RenderObject::render() {
  std::cout << "Render " << name << std::endl;
}

void RenderObject::update() {
  std::cout << "Update " << name << " to " << name + unit << std::endl;
  name = name + unit;
}

Scene::Scene(size_t buffer_size) {
  this->current_buffer = new std::vector<RenderObject *>(buffer_size);
  this->next_buffer    = new std::vector<RenderObject *>(buffer_size);

  for (size_t i = 0; i < buffer_size; i++) {
    current_buffer->at(i) = new RenderObject(std::to_string(i));
    next_buffer->at(i)    = new RenderObject(std::to_string(i));
  }
}

Scene::~Scene() {
  delete current_buffer;
  delete next_buffer;
}

size_t Scene::size() {
  return current_buffer->size();
}

void Scene::render(int delay) {
  for (auto object : *current_buffer) {
    object->render();
    sleep(delay);
  }
}

void Scene::update(int delay) {
  for (auto object : *next_buffer) {
    object->update();
    sleep(delay);
  }
  swap_buffers();
}

void Scene::swap_buffers() {
  std::vector<RenderObject *> *tmp = current_buffer;
  current_buffer                   = next_buffer;
  next_buffer                      = tmp;
}

template <size_t FPS>
auto compute_pixel_cost(size_t pixel_count) {
  auto rate  = std::chrono::duration<size_t, std::ratio<1, FPS>>(1s);
  auto frame = 1000 / rate.count();

  return frame / pixel_count;
}

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
