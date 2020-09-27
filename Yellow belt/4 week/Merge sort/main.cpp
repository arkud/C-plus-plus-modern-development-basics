#include <iostream>
#include "3parts.cpp"

int main() {
    std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 2};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    return 0;
}