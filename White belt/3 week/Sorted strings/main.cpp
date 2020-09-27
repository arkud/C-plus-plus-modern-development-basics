#include <iostream>
#include <vector>
#include <algorithm>

class SortedStrings {
public:
    void AddString(const std::string &s) {
        pf_strings.push_back(s);
    }

    std::vector<std::string> GetSortedStrings() {
        std::vector<std::string> result = pf_strings;
        std::sort(result.begin(), result.end());

        return result;
    }

private:
    std::vector<std::string> pf_strings;
};
