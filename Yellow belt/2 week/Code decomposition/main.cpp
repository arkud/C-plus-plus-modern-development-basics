#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream &operator>>(istream &is, Query &q) {
    string operation_code;
    is >> operation_code;

    if (operation_code == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;

        q.stops.resize(stop_count);
        for (auto &item : q.stops) {
            is >> item;
        }
    } else if (operation_code == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (operation_code == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (operation_code == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    bool any_stops;
    std::vector<string> buses;
};

ostream &operator<<(ostream &os, const BusesForStopResponse &r) {
    if (!r.any_stops) {
        os << "No stop";
    } else {
        for (const auto &item : r.buses) {
            os << item << " ";
        }
    }
    return os;
}

struct StopsForBusResponse {
    bool any_buses;
    std::vector<std::pair<std::string, std::vector<std::string>>> stop_to_bus;
};

ostream &operator<<(ostream &os, const StopsForBusResponse &r) {
    if (!r.any_buses) {
        os << "No bus";
    } else {
        for (const auto &[first, second]:r.stop_to_bus) {
            os << "Stop " << first << ": ";
            if (second.empty()) {
                os << "no interchange";
            } else {
                for (const auto item:second) {
                    os << item << " ";
                }
            }
            if (r.stop_to_bus.back().first != first)
                os << std::endl;
        }
    }
    return os;
}

struct AllBusesResponse {
    bool any_buses;
    std::vector<std::pair<std::string, std::vector<std::string>>> buses_to_stops;
};

ostream &operator<<(ostream &os, const AllBusesResponse &r) {

    if (!r.any_buses) {
        os << "No buses";
    } else {
        for (const auto&[first, second] : r.buses_to_stops) {
            os << "Bus " << first << ": ";
            for (const auto &item: second) {
                os << item << " ";
            }
            if (r.buses_to_stops.back().first != first)
                os << std::endl;
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string &bus, const vector<string> &stops) {
        buses_to_stops[bus] = stops;
        for (const string &stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string &stop) const {
        BusesForStopResponse resp;

        if (stops_to_buses.count(stop) == 0) {
            resp.any_stops = false;
        } else {
            resp.any_stops = true;
            for (const string &bus : stops_to_buses.at(stop)) {
                resp.buses.push_back(bus);
            }
        }

        return resp;
    }

    StopsForBusResponse GetStopsForBus(const string &bus) const {
        StopsForBusResponse resp;

        if (buses_to_stops.count(bus) == 0) {
            resp.any_buses = false;
        } else {
            resp.any_buses = true;

            int i = 0;
            for (const string &stop : buses_to_stops.at(bus)) {
                resp.stop_to_bus.push_back({stop, {}});
                if (stops_to_buses.at(stop).size() != 1) {
                    for (const string &other_bus : stops_to_buses.at(stop)) {
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

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse resp;

        if (buses_to_stops.empty()) {
            resp.any_buses = false;
        } else {
            resp.any_buses = true;

            int i = 0;
            for (const auto &bus_item : buses_to_stops) {
                resp.buses_to_stops.push_back({bus_item.first, {}});
                for (const string &stop : bus_item.second) {
                    resp.buses_to_stops[i].second.push_back(stop);
                }
                i++;
            }
        }

        return resp;
    }

private:
    map<string, vector<string>> buses_to_stops;
    map<string, vector<string>> stops_to_buses;
};


int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; i++) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

    return 0;
}