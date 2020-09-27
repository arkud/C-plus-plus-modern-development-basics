#include <string>

bool IsPalindrom(std::string arg_str) {

    if(arg_str == "")
        return true;

    for (int i = 0, j = arg_str.length() - 1; i < j; i++, j--) {
        if(arg_str[i] != arg_str[j])
            return false;
    }

    return true;
}
