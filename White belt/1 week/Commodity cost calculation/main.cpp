#include <iostream>

int main() {
    double n;
    double a, b;
    double x, y;

    std::cin >> n >> a >> b >> x >> y;

    if (n > b)
        std::cout << n * (1 - y / 100);
    else if (n > a)
        std::cout << n * (1 - x / 100);
    else
        std::cout << n;

    return 0;
}
