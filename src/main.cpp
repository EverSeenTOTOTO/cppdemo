#include <iostream>
#include <iomanip>

#include "./utils.h"
#include "./example/read_write.h"

int main() {
    std::cout << std::boolalpha << std::setprecision(10);

    test_prior_writer();
    
    return 0;
}
