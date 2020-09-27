#include <iostream>
#include <string>
#include <set>
#include <map>
#include <deque>

struct Customer {
    unsigned int client_id;
    int room_count;
    int64_t time;

    Customer(const unsigned int &client_id, const int &room_count, const int64_t &time) : client_id(client_id),
                                                                                          room_count(room_count),
                                                                                          time(time) {}
};

class Hotel {
    std::deque<Customer> customers;
    std::map<unsigned int, size_t> unique_customers;
    size_t booked_rooms = 0;

    void update_time(const int64_t &current_time) {
        auto it = customers.begin();
        for (;it != customers.end() && current_time - it->time >= 86400; it++) {
            unique_customers[it->client_id]--;
            if (unique_customers[it->client_id] == 0) {
                unique_customers.erase(it->client_id);
            }
            booked_rooms -= it->room_count;
        }
        customers.erase(customers.begin(), it);
    }

public:
    explicit Hotel() = default;

    void book(const unsigned int &client_id, const int &room_count, const int64_t &time) {
        customers.emplace_back(client_id, room_count, time);
        unique_customers[client_id]++;
        booked_rooms += room_count;
    }

    size_t size(const int64_t &time) {
        if (!customers.empty())
            update_time(time);
        return unique_customers.size();
    }

    size_t rooms(const int64_t &time) {
        if (!customers.empty())
            update_time(time);
        return booked_rooms;
    }
};

class HotelManager {
    std::map<std::string, Hotel> name_to_hotel;
    int64_t current_time{};
public:
    HotelManager() = default;

    void
    book(const int64_t &time, const std::string &hotel_name, const unsigned int &client_id, const int &room_count) {
        name_to_hotel[hotel_name].book(client_id, room_count, time);
        current_time = time;
    }

    size_t clients(const std::string &hotel_name) {
        try {
            return name_to_hotel.at(hotel_name).size(current_time);
        }
        catch (...) {
            return 0;
        }
    }

    size_t rooms(const std::string &hotel_name) {
        try {
            return name_to_hotel.at(hotel_name).rooms(current_time);
        }
        catch (...) {
            return 0;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;

    HotelManager hm;
    for (int i = 0; i < q; i++) {
        std::string command;
        std::cin >> command;
        if (command == "BOOK") {
            int64_t time;
            std::string hotel_name;
            unsigned int client_id;
            int room_count;

            std::cin >> time >> hotel_name >> client_id >> room_count;
            hm.book(time, hotel_name, client_id, room_count);
        } else if (command == "CLIENTS") {
            std::string hotel_name;
            std::cin >> hotel_name;
            std::cout << hm.clients(hotel_name) << '\n';
        } else if (command == "ROOMS") {
            std::string hotel_name;
            std::cin >> hotel_name;
            std::cout << hm.rooms(hotel_name) << '\n';
        }
    }

    return 0;
}
/*
4
BOOK 0 A 1 100
BOOK 10800 A 1 100
BOOK 90000 A 1 100
CLIENTS A*/
