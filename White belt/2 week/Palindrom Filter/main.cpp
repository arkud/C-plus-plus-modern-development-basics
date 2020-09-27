#include <string>
#include <vector>

bool IsPalindrom(std::string arg_str) {

    if (arg_str == "")
        return true;

    for (int i = 0, j = arg_str.length() - 1; i < j; i++, j--) {
        if (arg_str[i] != arg_str[j])
            return false;
    }

    return true;
}


std::vector<std::string> PalindromFilter(std::vector<std::string> words, int minLength) {

    std::vector<std::string> result;
    for (auto item:words) {
        if (item.length() >= minLength && IsPalindrom(item))
            result.push_back(item);
    }

    return result;
}
