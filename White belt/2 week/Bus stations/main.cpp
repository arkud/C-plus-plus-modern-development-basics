#include <iostream>
#include <map>
#include <vector>


void new_bus(std::map<std::string, std::vector<std::string>> &arg_bus_stops,
             std::map<std::string, std::vector<std::string>> &arg_bus_numbers, const std::string &arg_bus,
             const std::vector<std::string> &arg_stops) {
    arg_bus_stops[arg_bus] = arg_stops;

    for (const auto &stop : arg_stops)
        arg_bus_numbers[stop].push_back(arg_bus);
}

void buses_for_stop(std::map<std::string, std::vector<std::string>> &arg_bus_numbers, const std::string &arg_stop) {

    if (arg_bus_numbers.count(arg_stop) == 0) {
        std::cout << "No stop" << std::endl;
        return;
    }

    for (const auto &bus : arg_bus_numbers[arg_stop])
        std::cout << bus << " ";
    std::cout << std::endl;
}

void stops_for_bus(std::map<std::string, std::vector<std::string>> &arg_bus_stops,
                   std::map<std::string, std::vector<std::string>> &arg_bus_numbers, const std::string &arg_bus) {

    if (arg_bus_stops.count(arg_bus) == 0) {
        std::cout << "No bus" << std::endl;
        return;
    }

    for (const auto &stop : arg_bus_stops[arg_bus]) {
        std::cout << "Stop " << stop << ": ";

        for (const auto &bus : arg_bus_numbers[stop]) {
            if (arg_bus != bus)
                std::cout << bus << " ";
        }
        if (arg_bus_numbers[stop].size() == 1)
            std::cout << "no interchange";

        std::cout << std::endl;
    }
}

void all_buses(std::map<std::string, std::vector<std::string>> &arg_bus_stops) {
    if (arg_bus_stops.empty()) {
        std::cout << "No buses" << std::endl;
        return;
    }

    for (const auto &[bus, stops] : arg_bus_stops) {
        std::cout << "Bus " << bus << ": ";
        for (const auto &stop:stops) {
            std::cout << stop << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::map<std::string, std::vector<std::string>> bus_stops;
    std::map<std::string, std::vector<std::string>> bus_numbers;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string action;
        std::cin >> action;

        if (action == "NEW_BUS") {
            std::string bus;
            int num;
            std::cin >> bus >> num;

            std::vector<std::string> stops(num);
            for (auto &stop : stops) {
                std::cin >> stop;
            }

            new_bus(bus_stops, bus_numbers, bus, stops);
        } else if (action == "BUSES_FOR_STOP") {
            std::string stop;
            std::cin >> stop;

            buses_for_stop(bus_numbers, stop);
        } else if (action == "STOPS_FOR_BUS") {
            std::string bus;
            std::cin >> bus;

            stops_for_bus(bus_stops, bus_numbers, bus);
        } else {
            all_buses(bus_stops);
        }
    }

    return 0;
}
