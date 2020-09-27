#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    std::ifstream fin("input.txt");

    if (fin.is_open()) {
        std::cout << std::fixed << std::setprecision(3);
        while (!fin.eof()) {
            double input;
            fin >> input;
            std::cout << input << std::endl;
        }
    }

    return 0;
}
