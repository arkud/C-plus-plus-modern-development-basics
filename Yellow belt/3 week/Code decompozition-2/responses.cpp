#include "responses.h"

std::ostream &operator<<(std::ostream &os, const BusesForStopResponse &r) {
    if (!r.any_stops) {
        os << "No stop";
    } else {
        for (const auto &item : r.buses) {
            os << item << " ";
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const StopsForBusResponse &r) {
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

std::ostream &operator<<(std::ostream &os, const AllBusesResponse &r) {

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