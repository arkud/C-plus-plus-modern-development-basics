#include <iostream>
#include <fstream>
#include <vector>

struct Student {
    std::string first_name;
    std::string last_name;
    int day;
    int month;
    int year;
};

int main() {

    int n;
    std::cin >> n;

    std::vector<Student> v(n);

    for (auto &student: v) {
        std::cin >> student.first_name >> student.last_name >> student.day >> student.month >> student.year;
    }

    int m;
    std::cin >> m;

    for (int j = 0; j < m; j++) {
        std::string request;
        int index;
        std::cin >> request >> index;

        if (index <= 0 || index > n)
            std::cout << "bad request";
        else if (request == "name") {
            std::cout << v[index - 1].first_name << " " << v[index - 1].last_name;
        } else if (request == "date") {
            std::cout << v[index - 1].day << "." << v[index - 1].month << "." << v[index - 1].year;
        } else {
            std::cout << "bad request";
        }

        if (j != m - 1)
            std::cout << std::endl;
    }

    return 0;
}
