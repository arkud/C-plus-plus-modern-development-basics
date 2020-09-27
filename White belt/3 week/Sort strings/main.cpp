#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<std::string> v(n);
    for (auto &i : v) {
        std::cin >> i;
    }

    std::sort(v.begin(), v.end(), [](const std::string &a, const std::string &b) {
        std::string A, B;
        for (const auto &i : a) {
            A += tolower(i);
        }
        for (const auto &i : b) {
            B += tolower(i);
        }

        return A < B;
    });

    for (const auto &i : v) {
        std::cout << i << " ";
    }

    return 0;
}
