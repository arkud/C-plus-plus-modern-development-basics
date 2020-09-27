#include <iostream>
#include <set>
#include <map>

int main() {
    std::map<std::set<std::string>, int> buses;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int num;
        std::cin >> num;

        std::set<std::string> to_insert;
        for (int j = 0; j < num; j++) {
            std::string stop;
            std::cin >> stop;

            to_insert.insert(stop);
        }

        if (buses[to_insert] == 0) {
            buses[to_insert] = buses.size();
            std::cout << "New bus " << buses.size() << std::endl;
        } else {
            std::cout << "Already exists for " << buses[to_insert] << std::endl;
        }
    }
    return 0;
}
