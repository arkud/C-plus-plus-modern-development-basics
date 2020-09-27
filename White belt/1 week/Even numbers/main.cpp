#include <iostream>

int main() {
    int a, b;
    std::cin >> a >> b;

    if (a % 2 != 0)
        ++a;
    for (int i = a; i <= b; i += 2) {
        std::cout << i << " ";
    }

    return 0;
}
