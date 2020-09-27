//
// Created by ar_kud on 19.09.2020.
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
#include "json.h"
#include "router.h"

using namespace std;
using namespace Json;

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
    string name;

    bool operator<(const Bus &other) const {
        return name < other.name;
    }
};

ostream &operator<<(ostream &out, const Bus &bus) {
    return out << bus.name;
}

struct Stop {
    set<Bus> buses;
    unordered_map<string, double> distance_to_stops;
    string name;
    Coordinates coordinates;

    explicit Stop(string name) : name(move(name)) {}

    Stop(string name, Coordinates coordinates,
         const unordered_map<string, double> &distance_to_stops_) : name(move(name)), coordinates(coordinates),
                                                                    distance_to_stops(distance_to_stops_) {

    }

    void AddDistances(const unordered_map<string, double> &distance_to_stops_) {
        for (const auto &[stop, distance] : distance_to_stops_) {
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
        StopResponse,
        RouteResponse
    };

    explicit Response(Type type) : type_(type) {}

    virtual string JsonResponse() const = 0;

    virtual ~Response() = default;

    Type type_;
    size_t request_id;

    Response &SetId(size_t request_id_) {
        request_id = request_id_;
        return *this;
    }
};

struct BusInfo : Response {
    string bus_number_;
    size_t stops_count_ = 0;
    size_t unique_stops_count_ = 0;
    //In meters
    double route_length_ = 0.0;
    double route_curvature_ = 0.0;

    BusInfo() : Response(Type::BusResponse) {}

    BusInfo &SetBusNumber(string bus_number) {
        bus_number_ = move(bus_number);
        return *this;
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
            if (!route.is_looped) {
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

    string JsonResponse() const override {
        stringstream out;
        out << "{\n"
            << "\"request_id\": " << request_id << ",\n";
        if (route_length_ != -1) {
            out << "\"route_length\": " << route_length_ << ",\n"
                << "\"curvature\": " << route_curvature_ << ",\n"
                << "\"stop_count\": " << stops_count_ << ",\n"
                << "\"unique_stop_count\": " << unique_stops_count_ << "\n";
        } else {
            out << R"("error_message": "not found")" << "\n";
        }
        out << "}";
        return out.str();
    }
};

struct StopInfo : Response {
    shared_ptr<Stop> stop;
    bool is_found;

    explicit StopInfo(const shared_ptr<Stop> &stop, bool is_found) : Response(Type::StopResponse), stop(stop),
                                                                     is_found(is_found) {}

    string JsonResponse() const override {
        stringstream out;
        out << "{\n"
            << "\"request_id\": " << request_id << ",\n";
        if (is_found) {
            out << "\"buses\": [";
            size_t i = 1;
            for (const auto &bus : stop->buses) {
                out << "\n\"" << bus << "\"";
                if (i++ != stop->buses.size()) {
                    out << ",";
                } else {
                    out << "\n";
                }
            }
            out << "]\n";
        } else {
            out << R"("error_message": "not found")" << "\n";
        }
        out << "}";
        return out.str();
    }
};

template<typename Weight>
struct RouteInfo : Response {
    RouteInfo() : Response(Response::Type::RouteResponse) {}

    string JsonResponse() const override {
        stringstream out;
        out << "{\n";
        if (is_found) {
            out << "\"total_time\": " << total_time << ", \n"
                << "\"items\": [";
            if(!weights.empty())
                out << "\n";
            for(int i = 0; i < weights.size(); i++){
                out << "{\n"
                    << "\"time\": " << wait_time << ",\n"
                    << "\"type\": \"Wait\",\n"
                    << "\"stop_name\": \""<< route.stops[i]->name << "\"\n"
                    <<"},\n";
                out << "{\n"
                    << "\"span_count\": " << weights[i].span_count << ",\n"
                    << "\"bus\": " << "\"" << weights[i].bus.name << "\",\n"
                    << "\"type\": \"Bus\",\n"
                    << "\"time\": "<< weights[i].time - wait_time << "\n";
                if(i + 1 != weights.size())
                    out << "},\n";
                else
                    out << "}\n";
            }
            out << "],\n";
        } else {
            out << R"("error_message": "not found",)" << "\n";
        }
        out  << "\"request_id\": " << request_id << "\n";
        out << "}";
        return out.str();
    }

    Route route;
    bool is_found = true;
    vector<Weight> weights;
    double total_time;
    double wait_time;
};

struct Settings {
    //in minutes
    uint8_t bus_wait_time;
    //in km/h
    double bus_velocity;
};

class BusManager {
public:
    BusManager() = default;

    void SetSettings(Settings settings_) {
        settings = settings_;
    }

    void AddStop(const string &stop, const Coordinates &coordinates,
                 const unordered_map<string, double> &distance_to_stops) {
        if (bus_stops.count(stop) == 0) {
            bus_stops[stop] = make_shared<Stop>(stop, coordinates, distance_to_stops);
        } else {
            bus_stops[stop]->coordinates = coordinates;
            bus_stops[stop]->AddDistances(distance_to_stops);
        }


        for (const auto&[other_stop, distance] : distance_to_stops) {
            if (bus_stops.count(other_stop) == 0)
                AddStop(other_stop, {0, 0}, {});
            auto &other_stop_dist = bus_stops.at(other_stop)->distance_to_stops;
            if (other_stop_dist.count(stop) == 0)
                other_stop_dist[stop] = distance;
        }
    }

    void AddBus(const string &bus_number, const vector<string> &route, bool is_looped) {
        Route to_insert;
        to_insert.is_looped = is_looped;

        auto first_stop = route.front();
        for (const auto &stop : route) {
            if (bus_stops.count(stop) == 0)
                AddStop(stop, {0, 0}, {});
            auto stop_ptr = bus_stops[stop];
            stop_ptr->buses.insert(Bus{bus_number});
            to_insert.stops.push_back(stop_ptr);
        }

        buses_to_route[bus_number] = move(to_insert);
    }

    unique_ptr<BusInfo> FindBus(const string &bus_number) const {
        auto result = make_unique<BusInfo>();
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

    struct Weight {
        Bus bus;
        size_t span_count;
        double time = 0.0;

        Weight() = default;

        Weight(int time) : time(time) {}

        bool operator<(const Weight &other) const {
            return time < other.time;
        }

        bool operator>(const Weight& other) const {
            return time > other.time;
        }

        Weight operator+(const Weight& other) const {
            Weight result;
            result.time = time + other.time;
            return result;
        }

        bool operator>= (int other) const {
            return time >= other;
        }
    };

    unique_ptr<RouteInfo<Weight>> FindRoute(const string &stop_from, const string &stop_to) const {
        auto result = make_unique<RouteInfo<Weight>>();
        if(stop_from == stop_to)
            return result;
        result->wait_time = settings.bus_wait_time;

        if (!is_graph_constructed) {
            stops_graph = ConstructGraph();
            is_graph_constructed = true;
        }
        Graph::Router router(stops_graph.value());
        auto route = router.BuildRoute(bus_stop_to_vertex.at(stop_from), bus_stop_to_vertex.at(stop_to));

        if(!route){
            result->is_found = false;
            return result;
        }

        for (int i = 0; i < route->edge_count; i++) {
            auto route_edge = stops_graph->GetEdge(router.GetRouteEdge(route->id, i));
            result->weights.push_back(route_edge.weight);
            result->total_time += route_edge.weight.time;
            if (i == 0)
                result->route.stops.push_back(bus_stops.at(vertex_to_bus_stop.at(route_edge.from)));
            result->route.stops.push_back(bus_stops.at(vertex_to_bus_stop.at(route_edge.to)));
            if (route_edge.to == bus_stop_to_vertex.at(stop_to))
                break;
        }

        return move(result);
    }

private:
    Graph::DirectedWeightedGraph<BusManager::Weight> ConstructGraph() const {
        Graph::DirectedWeightedGraph<BusManager::Weight> result(100);
        for (const auto&[stop, ptr] : bus_stops) {
            Graph::VertexId vertex = bus_stop_to_vertex.size();
            bus_stop_to_vertex[stop] = vertex;
            vertex_to_bus_stop[vertex] = stop;
        }

        Graph::Edge<Weight> edge;
        for (const auto&[bus, route] : buses_to_route) {
            edge.weight.bus.name = bus;

            for(int i = 0;i < route.stops.size(); i++) {
                auto prev_stop = route.stops[i];
                edge.from = bus_stop_to_vertex.at(prev_stop->name);
                double time = settings.bus_wait_time;
                int f = 0;
                for (int j = i + 1; j < route.stops.size();j++) {
                    auto stop = route.stops[j];
                    edge.to = bus_stop_to_vertex.at(stop->name);
                    time += prev_stop->distance_to_stops[stop->name] / (1000 * settings.bus_velocity / 60.0);
                    prev_stop = stop;
                    edge.weight.time = time;
                    edge.weight.span_count = ++f;
                    result.AddEdge(edge);
                }
            }
        }
        return result;
    }
    unordered_map<string, shared_ptr<Stop>> bus_stops;
    unordered_map<string, Route> buses_to_route;
    Settings settings;

    mutable unordered_map<string, Graph::VertexId> bus_stop_to_vertex;
    mutable unordered_map<Graph::VertexId, string> vertex_to_bus_stop;
    mutable optional<Graph::DirectedWeightedGraph<BusManager::Weight>> stops_graph;
    mutable bool is_graph_constructed = false;
};

struct Request {
    enum class Type {
        CreateStop,
        FindStop,
        CreateBus,
        FindBus,
        RoutingSettings,
        FindRoute
    };

    enum class Class {
        Create,
        Find,
        Settings
    };

    explicit Request(Type type) : type_(type) {}

    static unique_ptr<Request> Create(Type request_type);

    virtual void ParseFrom(const map<string, Node> &field_to_value) = 0;

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

    void ParseFrom(const map<string, Node> &field_to_value) override {
        if (holds_alternative<double>(field_to_value.at("longitude")))
            coordinates.longitude = field_to_value.at("longitude").AsDouble();
        else
            coordinates.longitude = field_to_value.at("longitude").AsInt();


        if (holds_alternative<double>(field_to_value.at("latitude")))
            coordinates.latitude = field_to_value.at("latitude").AsDouble();
        else
            coordinates.latitude = field_to_value.at("latitude").AsInt();

        stop_name = field_to_value.at("name").AsString();
        for (const auto&[stop, distance] : field_to_value.at("road_distances").AsMap()) {
            distance_to_stops[stop] = distance.AsInt();
        }
    }

private:
    string stop_name;
    unordered_map<string, double> distance_to_stops = {};
    Coordinates coordinates;
};

struct CreateBusRequest : ModifyRequest {
    CreateBusRequest() : ModifyRequest(Type::CreateBus) {}

    void ParseFrom(const map<string, Node> &field_to_value) override {
        bus_number = field_to_value.at("name").AsString();
        is_looped = field_to_value.at("is_roundtrip").AsBool();
        for (const auto &stop : field_to_value.at("stops").AsArray()) {
            route.push_back(stop.AsString());
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

struct RoutingSettingsRequest : ModifyRequest {
    RoutingSettingsRequest() : ModifyRequest(Type::RoutingSettings) {}

    void ParseFrom(const map<string, Node> &field_to_value) override {
        settings.bus_wait_time = field_to_value.at("bus_wait_time").AsInt();
        if (holds_alternative<double>(field_to_value.at("bus_velocity")))
            settings.bus_velocity = field_to_value.at("bus_velocity").AsDouble();
        else
            settings.bus_velocity = field_to_value.at("bus_velocity").AsInt();
    }

    void Process(BusManager &manager) const override {
        manager.SetSettings(settings);
    }

    Settings settings;
};

struct FindBusRequest : ReadRequest<unique_ptr<BusInfo>> {
    FindBusRequest() : ReadRequest(Type::FindBus) {}

    void ParseFrom(const map<string, Node> &field_to_value) override {
        bus_number = field_to_value.at("name").AsString();
        request_id = field_to_value.at("id").AsInt();
    }

    unique_ptr<BusInfo> Process(const BusManager &manager) const override {
        auto result = manager.FindBus(bus_number);
        result->SetId(request_id);
        return result;
    }

    string bus_number;
    size_t request_id;
};

struct FindStopRequest : ReadRequest<unique_ptr<StopInfo>> {
    FindStopRequest() : ReadRequest(Type::FindStop) {}

    void ParseFrom(const map<string, Node> &field_to_value) override {
        stop_name = field_to_value.at("name").AsString();
        request_id = field_to_value.at("id").AsInt();
    }

    unique_ptr<StopInfo> Process(const BusManager &manager) const override {
        auto result = manager.FindStop(stop_name);
        result->SetId(request_id);
        return result;
    }

    string stop_name;
    size_t request_id;
};

struct FindRouteRequest : ReadRequest<unique_ptr<RouteInfo<BusManager::Weight>>> {
    FindRouteRequest() : ReadRequest(Type::FindRoute) {}

    void ParseFrom(const map<string, Node> &field_to_value) override {
        stop_from = field_to_value.at("from").AsString();
        stop_to = field_to_value.at("to").AsString();
        request_id = field_to_value.at("id").AsInt();
    }

    unique_ptr<RouteInfo<BusManager::Weight>> Process(const BusManager &manager) const override {
        auto result = manager.FindRoute(stop_from, stop_to);
        result->SetId(request_id);
        return result;
    }

    string stop_from, stop_to;
    size_t request_id;
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
        case Request::Type::RoutingSettings:
            return make_unique<RoutingSettingsRequest>();
        case Request::Type::FindRoute:
            return make_unique<FindRouteRequest>();
        default:
            return nullptr;
    }
}

const unordered_multimap<string_view, Request::Class> STR_TO_REQUEST_CLASS = {
        {"base_requests",    Request::Class::Create},
        {"stat_requests",    Request::Class::Find},
        {"routing_settings", Request::Class::Settings}
};

optional<Request::Class> ConvertRequestClassFromString(string_view class_str) {
    if (const auto it = STR_TO_REQUEST_CLASS.find(class_str);
            it != STR_TO_REQUEST_CLASS.end()) {
        return it->second;
    } else {
        return nullopt;
    }
}

const unordered_multimap<string_view, Request::Type> STR_TO_REQUEST_CREATE_TYPE = {
        {"Bus",  Request::Type::CreateBus},
        {"Stop", Request::Type::CreateStop}
};
const unordered_multimap<string_view, Request::Type> STR_TO_REQUEST_FIND_TYPE = {
        {"Bus",   Request::Type::FindBus},
        {"Stop",  Request::Type::FindStop},
        {"Route", Request::Type::FindRoute}
};

optional<Request::Type>
ConvertRequestTypeFromString(string_view type_str, const optional<Request::Class> &type_class = nullopt) {
    if (type_class == Request::Class::Find) {
        if (const auto it = STR_TO_REQUEST_FIND_TYPE.find(type_str);
                it != STR_TO_REQUEST_FIND_TYPE.end()) {
            return it->second;
        }
    } else if (type_class == Request::Class::Create) {
        if (const auto it = STR_TO_REQUEST_CREATE_TYPE.find(type_str);
                it != STR_TO_REQUEST_CREATE_TYPE.end()) {
            return it->second;
        }
    } else if (type_class == Request::Class::Settings) {
        return Request::Type::RoutingSettings;
    }
    return nullopt;
}

unique_ptr<Request> ParseRequest(const Node &request_node, const optional<Request::Class> &request_class = nullopt) {
    map<string, Node> field_to_value = request_node.AsMap();
    auto request_type = ConvertRequestTypeFromString(field_to_value["type"].AsString(), request_class);

    if (!request_type) {
        return nullptr;
    }

    unique_ptr<Request> request = Request::Create(*request_type);
    if (request) {
        request->ParseFrom(field_to_value);
    };
    return request;
}

vector<unique_ptr<Request>> ReadRequests(istream &in = cin) {
    vector<unique_ptr<Request>> result;
    const map<string, Node> requests_holder = Load(in).GetRoot().AsMap();

    for (const auto&[request_type, array] : requests_holder) {
        if (holds_alternative<map<string, Node>>(array)) {
            auto request_type = Request::Type::RoutingSettings;
            unique_ptr<Request> request = Request::Create(request_type);
            if (request) {
                request->ParseFrom(array.AsMap());
                result.push_back(move(request));
            };
            continue;
        }
        for (const auto &node: array.AsArray()) {
            if (auto request = ParseRequest(node, ConvertRequestClassFromString(request_type))) {
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
        } else if(request->type_ == Request::Type::FindRoute){
            const auto &request_ = static_cast<const FindRouteRequest &>(*request);
            result.push_back(move(request_.Process(bm)));
        } else {
            const auto &request_ = static_cast<const ModifyRequest &>(*request);
            request_.Process(bm);
        }
    }
    return result;
}

void PrintResponses(const vector<unique_ptr<Response>> &responses, ostream &output = cout) {
    output << "[\n";
    for (const auto &response : responses) {
        if (response->type_ == Response::Type::BusResponse) {
            output << static_cast<const BusInfo &>(*response).JsonResponse();
        } else {
            output << static_cast<const StopInfo &>(*response).JsonResponse();
        }
        if (response != responses.back()) {
            output << ",";
        }
        output << "\n";
    }
    output << "]";
}

void TestBasics() {
    stringstream ss;
    ss << R"({"routing_settings": {"bus_wait_time": 490, "bus_velocity": 280}, "base_requests": [{"type": "Stop", "name": "Vnukovo", "latitude": 55.572308, "longitude": 37.58821, "road_distances": {"Troparyovo": 315031, "Kiyevskoye sh 10": 736717, "Kiyevskoye sh 20": 85080, "Kiyevskoye sh 30": 426639, "Kiyevskoye sh 40": 141884, "Kiyevskoye sh 50": 224766, "Kiyevskoye sh 60": 902131, "Kiyevskoye sh 70": 328591, "Kiyevskoye sh 80": 805100, "Kiyevskoye sh 90": 855121, "Kiyevskoye sh 100": 737402}}, {"type": "Stop", "name": "Kiyevskoye sh 10", "latitude": 55.575724434343435, "longitude": 37.58821, "road_distances": {"Troparyovo": 525298, "Kiyevskoye sh 20": 106788, "Kiyevskoye sh 30": 490517, "Kiyevskoye sh 40": 322450, "Kiyevskoye sh 50": 600182, "Kiyevskoye sh 60": 550985, "Kiyevskoye sh 70": 677645, "Kiyevskoye sh 80": 728727, "Kiyevskoye sh 90": 463120, "Kiyevskoye sh 100": 445277}}, {"type": "Stop", "name": "Kiyevskoye sh 20", "latitude": 55.57914086868687, "longitude": 37.58821, "road_distances": {"Troparyovo": 172308, "Kiyevskoye sh 30": 105345, "Kiyevskoye sh 40": 667602, "Kiyevskoye sh 50": 645868, "Kiyevskoye sh 60": 246041, "Kiyevskoye sh 70": 22704, "Kiyevskoye sh 80": 235980, "Kiyevskoye sh 90": 407045, "Kiyevskoye sh 100": 937377}}, {"type": "Stop", "name": "Kiyevskoye sh 30", "latitude": 55.582557303030306, "longitude": 37.58821, "road_distances": {"Troparyovo": 73536, "Kiyevskoye sh 40": 910439, "Kiyevskoye sh 50": 36945, "Kiyevskoye sh 60": 353820, "Kiyevskoye sh 70": 772561, "Kiyevskoye sh 80": 484228, "Kiyevskoye sh 90": 509332, "Kiyevskoye sh 100": 152038}}, {"type": "Stop", "name": "Kiyevskoye sh 40", "latitude": 55.585973737373735, "longitude": 37.58821, "road_distances": {"Troparyovo": 934646, "Kiyevskoye sh 50": 87601, "Kiyevskoye sh 60": 501890, "Kiyevskoye sh 70": 758953, "Kiyevskoye sh 80": 692055, "Kiyevskoye sh 90": 929215, "Kiyevskoye sh 100": 242694}}, {"type": "Stop", "name": "Kiyevskoye sh 50", "latitude": 55.58939017171717, "longitude": 37.58821, "road_distances": {"Troparyovo": 547802, "Kiyevskoye sh 60": 282245, "Kiyevskoye sh 70": 26703, "Kiyevskoye sh 80": 479166, "Kiyevskoye sh 90": 74238, "Kiyevskoye sh 100": 324219}}, {"type": "Stop", "name": "Kiyevskoye sh 60", "latitude": 55.592806606060606, "longitude": 37.58821, "road_distances": {"Troparyovo": 232275, "Kiyevskoye sh 70 ... 8", "30", "32", "34", "36", "38k", "40k", "42", "44", "46k", "48k", "52", "54", "56k", "60", "68k", "70k", "72k", "74k", "80k", "86k", "88k"]}, {"request_id": 1377234840, "total_time": 2279.140785714286, "items": [{"type": "Wait", "stop_name": "Kiyevskoye sh 20", "time": 490}, {"type": "Bus", "bus": "1", "time": 18.23142857142857, "span_count": 1}, {"type": "Wait", "stop_name": "Vnukovo", "time": 490}, {"type": "Bus", "bus": "69k", "time": 1280.909357142857, "span_count": 10}]}, {"request_id": 1668940893, "buses": ["1", "13", "17", "31k", "33k", "35", "37k", "39k", "3k", "41", "43k", "45k", "51k", "53k", "59", "61k", "63", "65k", "75k", "77k", "81", "89", "93k", "97", "9k"]}, {"request_id": 1607260023, "total_time": 667.8642142857143, "items": [{"type": "Wait", "stop_name": "Troparyovo", "time": 490}, {"type": "Bus", "bus": "6k", "time": 177.86421428571427, "span_count": 2}]}, {"request_id": 1984277915, "total_time": 1423.6170714285713, "items": [{"type": "Wait", "stop_name": "Mokhovaya ul 80", "time": 490}, {"type": "Bus", "bus": "21k", "time": 202.66071428571428, "span_count": 2}, {"type": "Wait", "stop_name": "Belorusskiy vokzal", "time": 490}, {"type": "Bus", "bus": "20", "time": 240.95635714285712, "span_count": 2}]}, {"request_id": 76281933, "buses": ["100k", "12", "14", "18", "28", "2k", "30", "32", "34", "38k", "4", "46k", "48k", "52", "54", "56k", "58", "60", "62", "64k", "68k", "72k", "74k", "76k", "8", "98"]}, {"request_id": 1306747782, "total_time": 1769.9972857142857, "items": [{"type": "Wait", "stop_name": "Komsomolskiy pr 60", "time": 490}, {"type": "Bus", "bus": "81", "time": 640.8040714285714, "span_count": 5}, {"type": "Wait", "stop_name": "Belorusskiy vokzal", "time": 490}, {"type": "Bus", "bus": "14", "time": 149.19321428571428, "span_count": 2}]}, {"request_id": 121475302, "total_time": 1102.8060714285714, "items": [{"type": "Wait", "stop_name": "Komsomolskiy pr 20", "time": 490}, {"type": "Bus", "bus": "11", "time": 33.47142857142857, "span_count": 1}, {"type": "Wait", "stop_name": "Luzhniki", "time": 490}, {"type": "Bus", "bus": "16k", "time": 89.33464285714285, "span_count": 3}]}, {"request_id": 685610034, "total_time": 904.8955, "items": [{"type": "Wait", "stop_name": "Komsomolskiy pr 30", "time": 490}, {"type": "Bus", "bus": "81", "time": 414.89549999999997, "span_count": 4}]}])";

    const auto requests = ReadRequests(ss);
    const auto responses = ProcessRequests(requests);

    stringstream out;
    PrintResponses(responses, out);

    ASSERT_EQUAL(out.str(), "")

}

void Test() {
    TestRunner tr;

    RUN_TEST(tr, TestBasics);
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

