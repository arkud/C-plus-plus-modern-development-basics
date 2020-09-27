#include <iostream>
#include <set>
#include <vector>
#include <map>

void add(std::map<std::string, std::set<std::string>> &arg_dict, const std::string &arg_first,
         const std::string &arg_second) {

    arg_dict[arg_first].insert(arg_second);
    arg_dict[arg_second].insert(arg_first);
}

int count(std::map<std::string, std::set<std::string>> &arg_dict, const std::string &word) {
    return arg_dict[word].size();
}

bool
check(std::map<std::string, std::set<std::string>> &arg_dict, const std::string &first, const std::string &second) {
    return arg_dict[first].count(second) != 0;
}

int main() {
    std::map<std::string, std::set<std::string>> dict;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string action;
        std::cin >> action;

        if (action == "ADD") {
            std::string first, second;
            std::cin >> first >> second;

            if (second > first)
                std::swap(first, second);

            add(dict, first, second);
        } else if (action == "COUNT") {
            std::string first;
            std::cin >> first;

            std::cout << count(dict, first) << std::endl;
        } else {
            std::string first, second;
            std::cin >> first >> second;

            if (second > first)
                std::swap(first, second);

            if (check(dict, first, second))
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
    }
    return 0;
}
