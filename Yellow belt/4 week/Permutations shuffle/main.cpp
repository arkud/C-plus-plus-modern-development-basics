#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> v(n);
    for (auto &item : v) {
        item = n--;
        std::cout << item << " ";
    }
    std::cout << "\n";

    while (std::prev_permutation(v.begin(), v.end())) {
        std::for_each(v.begin(), v.end(), [](const int &item) {
            std::cout << item << " ";
        });
        std::cout << "\n";
    }

    return 0;
}
