#include <iostream>
#include <vector>
#include <algorithm>


int main() {
    int n;
    std::cin >> n;

    std::vector<int> v(n);

    for (auto &i : v) {
        std::cin >> i;
    }

    std::sort(v.begin(), v.end(), [](const int &a, const int &b) {
        return std::abs(a) < std::abs(b);
    });

    for (const auto &i : v) {
        std::cout << i << " ";
    }

    return 0;
}
