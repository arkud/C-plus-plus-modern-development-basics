#include <iostream>
#include <cmath>

int main() {

    double a, b, c;
    std::cin >> a >> b >> c;

    if (b == c && c == 0)
        std::cout << 0;
    else if (a == 0) {
        if (b != 0)
            std::cout << (double) -c / b;
    } else {
        int discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            std::cout << (-b + std::sqrt(discriminant)) / (2 * a) << " " << (-b - std::sqrt(discriminant)) / (2 * a);
        } else if (discriminant == 0)
            std::cout << (-b + std::sqrt(discriminant)) / (2 * a);
    }
    return 0;
}
