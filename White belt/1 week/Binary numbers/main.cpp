#include <iostream>
#include <vector>

int main() {
    int a;
    std::cin >> a;

    std::vector<bool> binary_a;

    while (a != 1 && a != 0) {
        binary_a.push_back(a % 2);
        a = a / 2;
    }
    binary_a.push_back(a);

    bool trigger = true;
    for (int i = binary_a.size(); i >= 0; --i) {
        if (trigger) {
            if (binary_a[i] == 1) {
                trigger = false;
                std::cout << binary_a[i];
            }
        } else
            std::cout << binary_a[i];
    }

    return 0;
}
