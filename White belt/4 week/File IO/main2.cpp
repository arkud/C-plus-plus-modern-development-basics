#include <iostream>
#include <fstream>


int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    if (fin.is_open()) {
        std::string line;
        while (getline(fin, line)) {
            fout << line << std::endl;
        }
    }

    return 0;
}

