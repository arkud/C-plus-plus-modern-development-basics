#include <iostream>
#include <string>

int main() {
    std::string word;

    std::cin >> word;

    int index = -2;
    for (int i = 0; i < word.size(); i++) {
        if (word[i] == 'f') {
            if (index == -2)
                index = -1;
            else {
                index = i;
                break;
            }
        }
    }

    std::cout << index;

    return 0;
}
