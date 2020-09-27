#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> SplitIntoWords(const std::string &s) {
    std::vector<std::string> result;

    auto it = s.begin();
    auto prev_it = it;
    while (it != s.end()) {
        it = std::find(it + 1, s.end(), ' ');
        result.emplace_back(prev_it, it);
        prev_it = it;
        prev_it++;
    }

    return result;
}


