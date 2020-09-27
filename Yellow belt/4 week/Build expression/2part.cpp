#include <iostream>
#include <deque>

int main() {
    int x;
    std::cin >> x;

    int n;
    std::cin >> n;

    if (n == 0) {
        std::cout << x;
        return 0;
    }

    std::deque<std::string> result;
    result.push_back(std::to_string(x));
    std::cin.ignore();

    for (int i = 0; i < n; i++) {
        std::string to_insert;
        std::getline(std::cin, to_insert);

        if ((result.back()[0] == '+' || result.back()[0] == '-') &&
            (to_insert[0] == '*' || to_insert[0] == '/')) {
            result.push_front("(");
            result.emplace_back(")");
        }
        result.emplace_back(" ");
        result.push_back(to_insert);

    }


    for (const auto &item:result) {
        std::cout << item;
    }

    return 0;
}

