#include <iostream>
#include <vector>
#include <map>

int main() {
    std::map<std::vector<std::string>, int> stops_to_bus;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int num;
        std::cin >> num;

        std::vector<std::string> stops(num);

        for (auto &stop : stops) {
            std::cin >> stop;
        }

        if (stops_to_bus[stops] == 0) {
            stops_to_bus[stops] = stops_to_bus.size();
            std::cout << "New bus " << stops_to_bus.size() << std::endl;
        } else {
            std::cout << "Already exists for " << stops_to_bus[stops] << std::endl;
        }

    }
    return 0;
}
