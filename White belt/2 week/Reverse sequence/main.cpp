#include <vector>

void Reverse(std::vector<int> &v) {
    for (int i = 0, j = v.size() - 1; i < j; i++, j--) {
        std::swap(v[i], v[j]);
    }
}

std::vector<int> Reversed(const std::vector<int>& v){
    std::vector<int> result = v;

    Reverse(result);

    return result;
}
