#include <iostream>
#include <vector>
#include <string>

#include "./type_list.h"

int main() {
    std::cout << std::boolalpha;

    using Empty = TypeList<>;
    using L1 = TypeList<int, char>;
    using L2 = TypeList<char, long>;

    using L = Concat<L1, L2>::type;

    using Temp = RemoveDuplicate<L>::type;

    return 0;
}