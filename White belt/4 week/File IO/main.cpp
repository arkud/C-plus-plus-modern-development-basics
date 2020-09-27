#include <iostream>
#include <fstream>

int main() {
    std::ifstream fin("input.txt");

    if (fin.is_open()) {
        std::string line;
        while (getline(fin, line)) {
            std::cout << line << std::endl;
        }
    }

    return 0;
}
