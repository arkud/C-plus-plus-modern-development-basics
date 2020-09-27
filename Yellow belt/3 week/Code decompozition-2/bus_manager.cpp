#include "bus_manager.h"

void BusManager::AddBus(const std::string &bus, const std::vector<std::string> &stops) {
    buses_to_stops[bus] = stops;
    for (const std::string &stop : stops) {
        stops_to_buses[stop].push_back(bus);
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string &stop) const {
    BusesForStopResponse resp;

    if (stops_to_buses.count(stop) == 0) {
        resp.any_stops = false;
    } else {
        resp.any_stops = true;
        for (const std::string &bus : stops_to_buses.at(stop)) {
            resp.buses.push_back(bus);
        }
    }

    return resp;
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string &bus) const {
    StopsForBusResponse resp;

    if (buses_to_stops.count(bus) == 0) {
        resp.any_buses = false;
    } else {
        resp.any_buses = true;

        int i = 0;
        for (const std::string &stop : buses_to_stops.at(bus)) {
            resp.stop_to_bus.push_back({stop, {}});
            if (stops_to_buses.at(stop).size() != 1) {
                for (const std::string &other_bus : stops_to_buses.at(stop)) {
                    if (bus != other_bus) {
                        resp.stop_to_bus[i].second.push_back(other_bus);
                    }
                }
            }
            i++;

        }
    }

    return resp;
}

AllBusesResponse BusManager::GetAllBuses() const {
    AllBusesResponse resp;

    if (buses_to_stops.empty()) {
        resp.any_buses = false;
    } else {
        resp.any_buses = true;

        int i = 0;
        for (const auto &bus_item : buses_to_stops) {
            resp.buses_to_stops.push_back({bus_item.first, {}});
            for (const std::string &stop : bus_item.second) {
                resp.buses_to_stops[i].second.push_back(stop);
            }
            i++;
        }
    }

    return resp;
}
