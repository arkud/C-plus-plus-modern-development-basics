/*
#include <iostream>

int main() {
    int x;
    std::cin >> x;

    int n;
    std::cin >> n;

    if (n == 0) {
        std::cout << x;
        return 0;
    }

    std::string result(n, '(');
    result += std::to_string(x);
    std::cin.ignore();

    for (int i = 0; i < n; i++) {
        std::string to_add;
        std::getline(std::cin, to_add);
        result += ") " + to_add;
    }

    std::cout << result;

    return 0;
}

*/
