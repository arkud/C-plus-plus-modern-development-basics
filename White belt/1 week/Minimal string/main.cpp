#include <iostream>

int main() {
    std::string min;

    std::cin>>min;
    for (int i = 0; i < 2; i++) {
        std::string str;
        std::cin >> str;
        if (str < min)
            min = str;
    }
    std::cout << min;
    return 0;
}
