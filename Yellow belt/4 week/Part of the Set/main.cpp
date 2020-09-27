#include <set>
#include <vector>

template<typename T>
std::vector<T> FindGreaterElements(const std::set<T> &elements, const T &border) {

    for (auto it = elements.begin(); it != elements.end(); it++) {
        if (*it > border) {
            return std::vector<T>(it, elements.end());
        }
    }
    return std::vector<T>();
}

