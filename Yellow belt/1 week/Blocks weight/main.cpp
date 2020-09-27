#include <iostream>
#include <cstdint>

int main() {
    int n;
    std::cin >> n;

    int density;
    std::cin >> density;

    uint64_t total_weight = 0;

    for (int i = 0; i < n; i++) {
        uint64_t width, height, length;
        std::cin >> width >> height >> length;

        total_weight += width * height * length * density;
    }

    std::cout << total_weight;

    return 0;
}
