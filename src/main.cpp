#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "./type_list.h"
#include "./constants.h"

int main() {
    std::cout << std::boolalpha << std::setprecision(20);

    std::cout << Eular << std::endl;
    std::cout << PI << std::endl;

    return 0;
}