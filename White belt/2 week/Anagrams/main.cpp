#include <iostream>
#include <map>

bool is_anagram(const std::string& arg_first, const std::string& arg_second) {
    if (arg_first.length() != arg_second.length())
        return false;

    std::map<char, int> first_word;
    std::map<char, int> second_word;

    for (const auto &letter : arg_first) {
        first_word[letter]++;
    }
    for (const auto &letter : arg_second) {
        second_word[letter]++;
    }

    return first_word == second_word;
}

int main() {
    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string first, second;
        std::cin >> first >> second;

        if (is_anagram(first, second))
            std::cout << 1 << std::endl;
        else
            std::cout << 0 << std::endl;
    }

    return 0;
}
