#pragma once

#include "http_request.h"

#include <string_view>
#include <map>

using namespace std;

class Stats {
public:
    Stats() {
        method_stats[unknown_method] = 0;
        for (const auto &method : available_methods) {
            method_stats[method] = 0;
        }

        uri_stats[unknown_uri] = 0;
        for (const auto &uri : available_uri) {
            uri_stats[uri] = 0;
        }
    }

    void AddMethod(string_view method);

    void AddUri(string_view uri);

    const map<string_view, int> &GetMethodStats() const;

    const map<string_view, int> &GetUriStats() const;

private:
    const array<string_view, 4> available_methods =
            {"GET", "POST", "PUT", "DELETE"};
    const array<string_view, 5> available_uri =
            {"/", "/order", "/product", "/basket", "/help"};
    const string_view unknown_method = "UNKNOWN";
    const string_view unknown_uri = "unknown";

    map<string_view, int> method_stats;
    map<string_view, int> uri_stats;
};

HttpRequest ParseRequest(string_view line);
