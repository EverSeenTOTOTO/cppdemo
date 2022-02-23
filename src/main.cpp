#include <iostream>
#include <iomanip>

#include "./example/read_write.h"

int main() {
    std::cout << std::boolalpha << std::setprecision(10);

    test_prior_reader();

    return 0;
}
