#include <random>

int randomBetween(int from, int to) {
    std::random_device r;
    std::default_random_engine eng(r());
    std::uniform_int_distribution<int> uniform_dist(from, to);
    return uniform_dist(eng);
}