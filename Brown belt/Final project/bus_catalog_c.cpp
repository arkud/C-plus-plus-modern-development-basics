//
// Created by ar_kud on 18.09.2020.
//

#include <iostream>
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include <variant>
#include <unordered_set>
#include <cmath>
#include <string>
#include <iomanip>

#include "test_runner.h"

using namespace std;

const double PI = 3.1415926535;

// Reads and removes read data
string_view ExtractToken(string_view &line, string_view delimeter = " ") {
    size_t pos = line.find_first_of(delimeter);
    string_view result = line.substr(0, pos);
    if (pos != string_view::npos) {
        line.remove_prefix(pos);
        line.remove_prefix(1);
    } else {
        line.remove_prefix(line.size());
    }
    return result;
}

string_view ReadToken(const string_view &line, string_view delimeter = " ") {
    size_t pos = line.find_first_of(delimeter);
    return line.substr(0, pos);
}

struct Coordinates {
    double latitude = 0.0;
    double longitude = 0.0;

    bool operator==(const Coordinates &other) const {
        return latitude == other.latitude && longitude == other.longitude;
    }
};

struct Bus {
    string bus_name;

    bool operator<(const Bus &other) const {
        return bus_name < other.bus_name;
    }
};

ostream &operator<<(ostream &out, const Bus &bus) {
    return out << bus.bus_name;
}

struct Stop {
    set<Bus> buses;
    unordered_map<string, double> distance_to_stops;
    string name;
    Coordinates coordinates;

    explicit Stop(string name) : name(move(name)) {}

    Stop(string name, Coordinates coordinates,
         const unordered_map<string, double>& distance_to_stops_) : name(move(name)), coordinates(coordinates), distance_to_stops(distance_to_stops_) {

    }

    void AddDistances(const unordered_map<string, double>& distance_to_stops_ ){
        for(const auto &[stop, distance] : distance_to_stops_){
            distance_to_stops[stop] = distance;
        }
    }

    bool operator==(const Stop &other) const {
        return name == other.name && coordinates == other.coordinates;
    }
};

struct Route {
    vector<shared_ptr<Stop>> stops;
    bool is_looped;

    auto begin() const {
        return stops.begin();
    }

    auto end() const {
        return stops.end();
    }

    auto begin() {
        return stops.begin();
    }

    auto end() {
        return stops.end();
    }
};

struct Response {
    enum class Type {
        BusResponse,
        StopResponse
    };

    explicit Response(Type type) : type_(type) {}

    virtual string StringResponse() const = 0;

    virtual ~Response() = default;

    Type type_;
};

struct RouteInfo : Response {
    string bus_number_;
    size_t stops_count_ = 0;
    size_t unique_stops_count_ = 0;
    //In meters
    double route_length_ = 0.0;
    double route_curvature_ = 0.0;

    RouteInfo() : Response(Type::BusResponse) {}

    void SetBusNumber(string bus_number) {
        bus_number_ = move(bus_number);
    }

    void FillInfo(const Route &route) {
        shared_ptr<Stop> prev = route.stops.front();

        unordered_set<string_view> unique_stops;
        for (const auto &stop : route) {
            unique_stops.insert(stop->name);

            if (stops_count_++ == 0)
                continue;

            const double lhs_lat = prev->coordinates.latitude * PI / 180.0;
            const double lhs_lon = prev->coordinates.longitude * PI / 180.0;
            const double rhs_lat = stop->coordinates.latitude * PI / 180.0;
            const double rhs_lon = stop->coordinates.longitude * PI / 180.0;
            route_curvature_ +=
                    acos(sin(lhs_lat) * sin(rhs_lat) + cos(lhs_lat) * cos(rhs_lat) * cos(abs(lhs_lon - rhs_lon))) *
                    6371000;

            route_length_ += prev->distance_to_stops[stop->name];
            if(!route.is_looped){
                route_length_ += stop->distance_to_stops[prev->name];
            }
            prev = stop;
        }
        if (!route.is_looped) {
            route_curvature_ *= 2;
            stops_count_ *= 2;
            stops_count_--;
        }

        unique_stops_count_ = unique_stops.size();
        route_curvature_ = route_length_ / route_curvature_;
    }

    string StringResponse() const override {
        stringstream output;
        output << "Bus " << bus_number_ << ": ";
        if (route_length_ != -1)
            output << stops_count_ << " stops on route, " <<
                   unique_stops_count_ << " unique stops, " << setprecision(6) << route_length_ << " route length, "
                   << route_curvature_ << " curvature";
        else
            output << "not found";

        return output.str();
    }
};

struct StopInfo : Response {
    shared_ptr<Stop> stop;
    bool is_found;

    explicit StopInfo(const shared_ptr<Stop> &stop, bool is_found) : Response(Type::StopResponse), stop(stop),
                                                                     is_found(is_found) {}

    string StringResponse() const override {
        stringstream output;
        output << "Stop " << stop->name << ": ";
        if (is_found) {
            if (stop->buses.empty())
                output << "no buses";
            else {
                output << "buses";
                for (const auto &bus : stop->buses) {
                    output << " " << bus;
                }
            }
        } else
            output << "not found";

        return output.str();
    }
};

class BusManager {
public:
    BusManager() = default;

    void AddStop(const string& stop, const Coordinates &coordinates, const unordered_map<string, double>& distance_to_stops) {
        if (bus_stops.count(stop) == 0) {
            bus_stops[stop] = make_shared<Stop>(stop, coordinates, distance_to_stops);
        }
        else {
            bus_stops[stop]->coordinates = coordinates;
            bus_stops[stop]->AddDistances(distance_to_stops);
        }


        for(const auto& [other_stop, distance] : distance_to_stops){
            if(bus_stops.count(other_stop) == 0)
                AddStop(other_stop, {0, 0}, {});
            auto& other_stop_dist = bus_stops.at(other_stop)->distance_to_stops;
            if(other_stop_dist.count(stop) == 0)
                other_stop_dist[stop] = distance;
        }
    }

    void AddBus(string bus_number, const vector<string> &route, bool is_looped) {
        Route to_insert;
        to_insert.is_looped = is_looped;
        for (const auto &stop : route) {
            if (bus_stops.count(stop) == 0)
                AddStop(stop, {0, 0}, {});
            auto stop_ptr = bus_stops[stop];
            stop_ptr->buses.insert(Bus{bus_number});
            to_insert.stops.push_back(stop_ptr);
        }

        buses_to_route[move(bus_number)] = move(to_insert);
    }

    unique_ptr<RouteInfo> FindBus(const string &bus_number) const {
        auto result = make_unique<RouteInfo>();
        result->SetBusNumber(bus_number);
        if (buses_to_route.count(bus_number))
            result->FillInfo(buses_to_route.at(bus_number));
        else
            result->route_length_ = -1;

        return result;
    }

    unique_ptr<StopInfo> FindStop(const string &stop_name) const {
        if (bus_stops.count(stop_name))
            return make_unique<StopInfo>(bus_stops.at(stop_name), true);
        else {
            auto stop_not_found = make_shared<Stop>(stop_name);
            return make_unique<StopInfo>(stop_not_found, false);
        }
    }

private:
    unordered_map<string, shared_ptr<Stop>> bus_stops;
    unordered_map<string, Route> buses_to_route;
};


struct Request {
    enum class Type {
        CreateStop,
        FindStop,
        CreateBus,
        FindBus,
        Bus,
        Stop
    };

    explicit Request(Type type) : type_(type) {}

    static unique_ptr<Request> Create(Type request_type);

    virtual void ParseFrom(string_view input) = 0;

    virtual ~Request() = default;

    const Type type_;
};

struct ModifyRequest : Request {
    using Request::Request;

    virtual void Process(BusManager &manager) const = 0;
};

template<typename ResultType>
struct ReadRequest : Request {
    using Request::Request;

    virtual ResultType Process(const BusManager &manager) const = 0;
};

struct CreateStopRequest : ModifyRequest {
    CreateStopRequest() : ModifyRequest(Type::CreateStop) {}

    void Process(BusManager &manager) const override {
        manager.AddStop(stop_name, coordinates, distance_to_stops);
    }

    void ParseFrom(string_view input) override {
        stop_name = string(ExtractToken(input, ":"));
        input.remove_prefix(1);
        coordinates.latitude = stod(string(ExtractToken(input, ",")));
        input.remove_prefix(1);
        coordinates.longitude = stod(string(ExtractToken(input, ",")));
        if(!input.empty())
        input.remove_prefix(1);

        string stop;
        double distance;
        while (!input.empty()) {
            distance = stod(string(ExtractToken(input, "m")));
            input.remove_prefix(1);
            ExtractToken(input, " ");
            stop = string(ExtractToken(input, ",\0\n"));
            distance_to_stops[move(stop)] = distance;
        }

    }

private:
    string stop_name;
    unordered_map<string, double> distance_to_stops = {};
    Coordinates coordinates;
};

struct CreateBusRequest : ModifyRequest {
    CreateBusRequest() : ModifyRequest(Type::CreateBus) {}

    void ParseFrom(string_view input) override {
        bus_number = string(ExtractToken(input, ":"));
        input.remove_prefix(1);
        if (input.find(">") != string_view::npos) {
            is_looped = true;
        } else {
            is_looped = false;
        }
        while (!input.empty()) {
            string_view token = ExtractToken(input, ">-");
            if (!input.empty())
                token.remove_suffix(1);
            route.emplace_back(token);
            if (!input.empty())
                input.remove_prefix(1);
        }
    }

    void Process(BusManager &manager) const override {
        manager.AddBus(bus_number, route, is_looped);
    }

private:
    bool is_looped;
    string bus_number;
    vector<string> route;
};

struct FindBusRequest : ReadRequest<unique_ptr<RouteInfo>> {
    FindBusRequest() : ReadRequest(Type::FindBus) {}

    void ParseFrom(string_view input) override {
        bus_number = string(string(input));
    }

    unique_ptr<RouteInfo> Process(const BusManager &manager) const override {
        return manager.GetBus(bus_number);
    }

    string bus_number;
};

struct FindStopRequest : ReadRequest<unique_ptr<StopInfo>> {
    FindStopRequest() : ReadRequest(Type::FindStop) {}

    void ParseFrom(string_view input) override {
        stop_name = string(string(input));
    }

    unique_ptr<StopInfo> Process(const BusManager &manager) const override {
        return manager.GetStop(stop_name);
    }

    string stop_name;
};

unique_ptr<Request> Request::Create(Request::Type request_type) {
    switch (request_type) {
        case Request::Type::CreateStop:
            return make_unique<CreateStopRequest>();
        case Request::Type::CreateBus:
            return make_unique<CreateBusRequest>();
        case Request::Type::FindBus:
            return make_unique<FindBusRequest>();
        case Request::Type::FindStop:
            return make_unique<FindStopRequest>();
        default:
            return nullptr;
    }
}


const unordered_multimap<string_view, Request::Type> STR_TO_REQUEST_TYPE = {
        {"Stop", Request::Type::Stop},
        {"Bus",  Request::Type::Bus}
};

optional<Request::Type> ConvertRequestTypeFromString(string_view type_str) {
    if (const auto it = STR_TO_REQUEST_TYPE.find(type_str);
            it != STR_TO_REQUEST_TYPE.end()) {
        return it->second;
    } else {
        return nullopt;
    }
}


unique_ptr<Request> ParseRequest(string_view request_str) {
    auto request_type = ConvertRequestTypeFromString(ExtractToken(request_str));

    if (!request_type) {
        return nullptr;
    }
    if (request_type == Request::Type::Bus) {
        if (request_str.find(":") != string_view::npos)
            request_type = Request::Type::CreateBus;
        else
            request_type = Request::Type::FindBus;
    } else if (request_type == Request::Type::Stop) {
        if (request_str.find(":") != string_view::npos)
            request_type = Request::Type::CreateStop;
        else
            request_type = Request::Type::FindStop;
    }

    unique_ptr<Request> request = Request::Create(*request_type);
    if (request) {
        request->ParseFrom(request_str);
    };
    return request;
}

vector<unique_ptr<Request>> ReadRequests(istream &in = cin) {
    vector<unique_ptr<Request>> result;

    for (int j = 0; j < 2; j++) {
        const size_t request_count = [&in] {
            size_t result;
            in >> result;
            in.ignore(1);
            return result;
        }();

        for (size_t i = 0; i < request_count; i++) {
            string line;
            getline(in, line);

            //cerr << line << "\n";
            if (auto request = ParseRequest(line)) {
                result.push_back(move(request));
            }
        }
    }

    return result;
}

vector<unique_ptr<Response>> ProcessRequests(const vector<unique_ptr<Request>> &requests) {
    vector<unique_ptr<Response>> result;
    BusManager bm;

    for (const auto &request : requests) {
        if (request->type_ == Request::Type::FindBus) {
            const auto &request_ = static_cast<const FindBusRequest &>(*request);
            result.push_back(move(request_.Process(bm)));
        } else if (request->type_ == Request::Type::FindStop) {
            const auto &request_ = static_cast<const FindStopRequest &>(*request);
            result.push_back(move(request_.Process(bm)));
        } else {
            const auto &request_ = static_cast<const ModifyRequest &>(*request);
            request_.Process(bm);
        }
    }
    return result;
}

void PrintResponses(const vector<unique_ptr<Response>> &responses, ostream &output = cout) {
    for (const auto &response : responses) {
        if (response->type_ == Response::Type::BusResponse) {
            output << static_cast<const RouteInfo &>(*response).StringResponse() << "\n";
        } else {
            output << static_cast<const StopInfo &>(*response).StringResponse() << "\n";
        }
    }
}

void TestBasics() {
    stringstream ss;
    ss << "13\n"
          "Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n"
          "Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka\n"
          "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
          "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
          "Stop Rasskazovka: 55.632761, 37.333324\n"
          "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n"
          "Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n"
          "Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n"
          "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n"
          "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n"
          "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
          "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
          "Stop Prazhskaya: 55.611678, 37.603831\n"
          "6\n"
          "Bus 256\n"
          "Bus 750\n"
          "Bus 751\n"
          "Stop Samara\n"
          "Stop Prazhskaya\n"
          "Stop Biryulyovo Zapadnoye\n";

    const auto requests = ReadRequests(ss);
    const auto responses = ProcessRequests(requests);

    stringstream out;
    PrintResponses(responses, out);

    ASSERT_EQUAL(out.str(), "Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature\n"
                            "Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.31808 curvature\n"
                            "Bus 751: not found\n"
                            "Stop Samara: not found\n"
                            "Stop Prazhskaya: no buses\n"
                            "Stop Biryulyovo Zapadnoye: buses 256 828\n")

}

void TestDistance(){
    stringstream input;
    input << "16\n"
            "Bus Looped_Single_Intrastop_Distance: A > B > C > A\n"
            "Stop A: 55.574371, 37.6517, 100m to B\n"
            "Stop B: 55.581065, 37.64839, 200m to C\n"
            "Stop C: 55.587655, 37.645687, 300m to A\n"

            "Bus Looped_Double_Intrastop_Distance: D > E > F > D\n"
            "Stop D: 55.574371, 37.6517, 100m to E, 400m to F\n"
            "Stop E: 55.581065, 37.64839, 200m to F, 500m to D\n"
            "Stop F: 55.587655, 37.645687, 300m to D, 600m to E\n"

            "Bus Straight_Single_Intrastop_Distance: G - H - I\n"
            "Stop G: 55.574371, 37.6517, 100m to H\n"
            "Stop H: 55.581065, 37.64839, 200m to I\n"
            "Stop I: 55.587655, 37.645687, 300m to H\n"

            "Bus Straight_Double_Intrastop_Distance: J - K - L\n"
            "Stop J: 55.574371, 37.6517, 100m to K, 400m to L\n"
            "Stop K: 55.581065, 37.64839, 200m to L, 500m to J\n"
            "Stop L: 55.587655, 37.645687, 300m to J, 600m to K\n"

            "4\n"
            "Bus Looped_Single_Intrastop_Distance\n"
            "Bus Looped_Double_Intrastop_Distance\n"
            "Bus Straight_Single_Intrastop_Distance\n"
            "Bus Straight_Double_Intrastop_Distance\n";

    const auto requests = ReadRequests(input);
    const auto responses = ProcessRequests(requests);

    stringstream ss_o;
    PrintResponses(responses,ss_o);

    ASSERT_EQUAL(ss_o.str(),
                 "Bus Looped_Single_Intrastop_Distance: 4 stops on route, 3 unique stops, 600 route length, 0.196736 curvature\n"
                 "Bus Looped_Double_Intrastop_Distance: 4 stops on route, 3 unique stops, 600 route length, 0.196736 curvature\n"
                 "Bus Straight_Single_Intrastop_Distance: 5 stops on route, 3 unique stops, 700 route length, 0.229497 curvature\n"
                 "Bus Straight_Double_Intrastop_Distance: 5 stops on route, 3 unique stops, 1400 route length, 0.458993 curvature\n"
    );
}

void TestSmth(){
    stringstream input;
    input << "3\n"
             "Stop A: 0.5, 1, 100000m to B\n"
             "Stop B: 0, 1\n"
             "Bus 256: B - A\n"
             "4\n"
             "Bus 256\n"
             "Stop A\n"
             "Stop B\n"
             "Stop C";

    const auto requests = ReadRequests(input);
    const auto responses = ProcessRequests(requests);

    stringstream ss_o;
    PrintResponses(responses,ss_o);

    ASSERT_EQUAL(ss_o.str(),
                 "Bus Looped_Single_Intrastop_Distance: 4 stops on route, 3 unique stops, 600 route length, 0.196736 curvature\n"
                 "Bus Looped_Double_Intrastop_Distance: 4 stops on route, 3 unique stops, 600 route length, 0.196736 curvature\n"
                 "Bus Straight_Single_Intrastop_Distance: 5 stops on route, 3 unique stops, 700 route length, 0.229497 curvature\n"
                 "Bus Straight_Double_Intrastop_Distance: 5 stops on route, 3 unique stops, 1400 route length, 0.458993 curvature\n"
    );
}

void Test() {
    TestRunner tr;

    //RUN_TEST(tr, TestBasics);
    //RUN_TEST(tr, TestDistance);
    RUN_TEST(tr, TestSmth);
}


int main() {
    Test();
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(6);

    const auto requests = ReadRequests();
    const auto responses = ProcessRequests(requests);
    PrintResponses(responses);
}

