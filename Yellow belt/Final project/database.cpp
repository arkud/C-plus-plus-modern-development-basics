#include "database.h"

void Database::Add(const Date &arg_date, const string &arg_event) {
    //cerr << "Add start" << endl;
    //cerr << arg_date << " " << arg_event << std::endl;
    if (pf_events[arg_date].find(arg_event) == pf_events[arg_date].end()) {
        pf_events_in_order[arg_date].push_back(arg_event);
        pf_events[arg_date].insert(arg_event);
    }
    //cerr << "Add end" << endl;
}

void Database::Print(ostream &os) const {
    //cerr << "Print start" << endl;
    int j = 0;
    for (const auto&[date, events] : pf_events_in_order) {
        int i = 0;
        for (const auto &event : events) {
            os << std::setfill('0') << std::setw(4) << date.year << "-" << std::setw(2) << date.month << "-"
               << std::setw(2) << date.day << " " << event;
            i++;
            if (i != events.size())
                os << std::endl;
        }
        j++;
        os << std::endl;
    }
    //cerr << "Print end" << endl;
}

pair<Date, string> Database::Last(const Date &date) const {
    //cerr << "Last start" << endl;
    auto it = pf_events_in_order.upper_bound(date);

    if (it == pf_events_in_order.begin() && date != it->first)
        throw invalid_argument("No entries");
    it--;

    Date last_date = it->first;

    pair<Date, string> result(last_date, pf_events_in_order.at(last_date).back());
    //cerr << "Last end" << endl;
    return result;
}

