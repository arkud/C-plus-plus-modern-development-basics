#include <iostream>
#include <sstream>

void EnsureEqual(const std::string &left, const std::string &right) {
    std::stringstream stream;

    if (left != right) {
        stream << left << " != " << right;
        throw std::runtime_error(stream.str());
    }
}
/*

int main() {
    try {
        EnsureEqual("C++ White", "C++ White");
        EnsureEqual("C++ White", "C++ Yellow");
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}*/
