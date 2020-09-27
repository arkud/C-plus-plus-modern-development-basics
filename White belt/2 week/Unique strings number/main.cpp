#include <iostream>
#include <set>

int main() {
    std::set<std::string> s;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string str;
        std::cin >> str;

        s.insert(str);
    }

    std::cout << s.size();

    return 0;
}
