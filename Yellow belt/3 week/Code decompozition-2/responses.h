#pragma once

#include <string>
#include <vector>
#include <iostream>

struct BusesForStopResponse {
    bool any_stops;
    std::vector<std::string> buses;
};

std::ostream &operator<<(std::ostream &os, const BusesForStopResponse &r);


struct StopsForBusResponse {
    bool any_buses;
    std::vector<std::pair<std::string, std::vector<std::string>>> stop_to_bus;
};

std::ostream &operator<<(std::ostream &os, const StopsForBusResponse &r);


struct AllBusesResponse {
    bool any_buses;
    std::vector<std::pair<std::string, std::vector<std::string>>> buses_to_stops;
};

std::ostream &operator<<(std::ostream &os, const AllBusesResponse &r);