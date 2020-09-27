#pragma once

#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <vector>
#include <tuple>

#include "node.h"
#include "date.h"

using namespace std;

template<typename First, typename Second>
ostream &operator<<(ostream &os, std::pair<First, Second>);

class Database {
public:
    void Add(const Date &arg_date, const string &arg_event);

    template<typename Func>
    int RemoveIf(Func comp);

    template<typename Func>
    vector<pair<Date, string>> FindIf(Func comp) const;

    void Print(ostream &os) const;

    pair<Date, string> Last(const Date &date) const;

private:
    map<Date, vector<string>> pf_events_in_order;
    map<Date, set<string>> pf_events;
};


template<typename First, typename Second>
ostream &operator<<(ostream &os, std::pair<First, Second> p) {
    os << p.first << " " << p.second;
    return os;
}


template<typename Func>
int Database::RemoveIf(Func comp) {
    //cerr << "RemoveIf start" << endl;
    int counter = 0;
    std::vector<Date> to_delete;

    for (auto &[first, second] : pf_events_in_order) {
        auto it = stable_partition(second.begin(), second.end(), [comp, first1 = first](const string &f) {
            bool result = !comp(first1, f);
            return result;
        });

        for (; it != second.end();) {
            pf_events[first].erase(*it);
            second.erase(it);
            counter++;
        }
        if (pf_events[first].empty()) {
            pf_events.erase(first);
            to_delete.push_back(first);
        }
    }
    // cerr << "Norm";
    for (const auto &item:to_delete) {
        pf_events_in_order.erase(item);
    }
    //cerr << "RemoveIf end" << endl;
    return counter;
}

template<typename Func>
vector<pair<Date, string>> Database::FindIf(Func comp) const {
    //cerr << "FindIf start" << endl;
    vector<pair<Date, string>> result;

    for (auto &item : pf_events_in_order) {
        for (auto &s : item.second) {
            if (comp(item.first, s))
                result.emplace_back(item.first, s);
        }
    }
    //cerr << "FindIf end" << endl;
    return result;
}
