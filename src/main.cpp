#include <iostream>
#include <iomanip>

#include "./utils.h"
#include "./example/read_write_channel.h"

int main() {
    std::cout << std::boolalpha << std::setprecision(10);

    test_dispatcher();
    return 0;
}
