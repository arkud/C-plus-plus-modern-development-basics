#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    size_t n;
    std::cin >> n;

    std::vector<int> degrees(n);

    for (auto &item:degrees) {
        std::cin >> item;
    }

    int64_t avg_degree = 0;
    for (const auto &item:degrees) {
        avg_degree += item;
    }
    avg_degree /= static_cast<int>(n);

    std::vector<int> result;
    for (size_t i = 0; i < degrees.size(); i++) {
        if (degrees[i] > avg_degree) {
            result.push_back(i);
        }
    }

    std::cout << result.size() << std::endl;
    for (const auto &item:result) {
        std::cout << item << " ";
    }

    return 0;
}
