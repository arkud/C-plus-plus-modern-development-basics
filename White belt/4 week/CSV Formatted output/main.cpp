#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    std::ifstream fin("input.txt");

    if (!fin.is_open())
        return 0;

    int n, m;
    fin >> n >> m;

    for (int i = 0; i < n; i++) {
        int value;
        for (int j = 0; j < m; j++) {
            fin >> value;
            fin.ignore(1);

            std::cout << std::setw(10) << value;
            if (j != m - 1)
                std::cout << " ";
        }
        if (i != n - 1)
            std::cout << std::endl;
    }

}
