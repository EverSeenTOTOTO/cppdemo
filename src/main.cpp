#include <iostream>
#include <iomanip>
#include <functional>

#include "./template/expr.h"

int main() {
    using namespace std;
    using namespace std::placeholders;

    cout << boolalpha << setprecision(10);

    auto foo = (4^(3*x+2)) + y*(3-x);
    auto bar = d(foo, x); // 对x求偏导
    auto baz = d(foo, y); // 对y求偏导

    cout << bar << endl;
    cout << baz << endl;

    cout << baz(y = 2) << endl;

    cout << bar(x = 2, y = 2) << endl; // 272554.5618
    cout << baz(x = 2, y = 2) << endl; // 1


    return 0;
}
