#include <algorithm>
#include "sum_reverse_sort.h"

int Sum(int x, int y) {
    return x + y;
}

std::string Reverse(std::string s) {
    std::reverse(s.begin(), s.end());
    return s;
}

void Sort(std::vector<int> &nums) {
    std::sort(nums.begin(), nums.end());
}


