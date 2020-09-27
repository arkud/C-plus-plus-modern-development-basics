#include <algorithm>
#include "stats.h"

HttpRequest ParseRequest(string_view line) {
    HttpRequest result;

    line.remove_prefix(line.find_first_not_of(' '));

    size_t buffer = line.find_first_of(' ');
    result.method = line.substr(0, buffer);
    line.remove_prefix(buffer + 1);

    buffer = line.find_first_of(' ');
    result.uri = line.substr(0, buffer);
    line.remove_prefix(buffer + 1);

    result.protocol = line.substr(0);

    return result;
}

void Stats::AddMethod(string_view method) {
    auto it = find(available_methods.begin(), available_methods.end(), method);
    if (it == available_methods.end()) {
        method = unknown_method;
    } else
        method = *it;

    if (method_stats.count(method) == 0)
        method_stats[method] = 0;

    method_stats[method]++;
}

void Stats::AddUri(string_view uri) {
    auto it = find(available_uri.begin(), available_uri.end(), uri);
    if (it == available_uri.end()) {
        uri = unknown_uri;
    } else
        uri = *it;

    if (uri_stats.count(uri) == 0)
        uri_stats[uri] = 0;

    uri_stats[uri]++;
}

const map<string_view, int> &Stats::GetMethodStats() const {
    return method_stats;
}

const map<string_view, int> &Stats::GetUriStats() const {
    return uri_stats;
}

