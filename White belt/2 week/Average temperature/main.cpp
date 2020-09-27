#include <iostream>
#include <vector>

void print(const std::vector<int> &to_print) {
    for (auto item : to_print) {
        std::cout << item << " ";
    }
}

int main() {
    int n;
    std::cin >> n;

    std::vector<int> temperature_by_day(n);
    int average_temperature = 0;

    for (auto &item : temperature_by_day) {
        std::cin >> item;
        average_temperature += item;
    }
    average_temperature /= n;

    std::vector<int> result;

    for (int i = 0; i < n; i++) {
        if (temperature_by_day[i] > average_temperature)
            result.push_back(i);
    }

    std::cout << result.size() << std::endl;
    print(result);

    return 0;
}
