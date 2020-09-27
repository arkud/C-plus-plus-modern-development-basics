#include <iomanip>
#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values)                    \
    {map<string, string>::const_iterator it;                    \
    it = values.find(#field);                                  \
    if (it != values.end()) {                                   \
        istringstream is(it->second);                           \
        is >> ticket.field;                                      \
    }    }                           // Реализуйте этот макрос, а также необходимые операторы для классов Date и Time


bool operator<(const Date &lhs, const Date rhs) {
    return make_tuple(lhs.year, lhs.month, lhs.day) < make_tuple(rhs.year, rhs.month, rhs.day);
}

bool operator==(const Date &lhs, const Date rhs) {
    return make_tuple(lhs.year, lhs.month, lhs.day) == make_tuple(rhs.year, rhs.month, rhs.day);
}

ostream &operator<<(ostream &os, const Date &d) {
    os << setfill('0') << setw(4) << d.year << setw(2) << "-" << d.month << "-" << d.day;
    return os;
}


bool operator<(const Time &lhs, const Time rhs) {
    return make_tuple(lhs.hours, lhs.minutes) < make_tuple(rhs.hours, rhs.minutes);
}

bool operator==(const Time &lhs, const Time rhs) {
    return make_tuple(lhs.hours, lhs.minutes) == make_tuple(rhs.hours, rhs.minutes);
}

ostream &operator<<(ostream &os, const Time &t) {
    os << setfill('0') << setw(2) << t.hours << ":" << t.minutes;
    return os;
}

istream &operator>>(istream &is, Date &d) {
    string buffer;
    getline(is, buffer, '-');
    d.year = stoi(buffer);
    getline(is, buffer, '-');
    d.month = stoi(buffer);
    getline(is, buffer, ' ');
    d.day = stoi(buffer);
    return is;
}

istream &operator>>(istream &is, Time &t) {
    string buffer;
    getline(is, buffer, ':');
    t.hours = stoi(buffer);
    getline(is, buffer, ' ');
    t.minutes = stoi(buffer);
    return is;
}


void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
            {"departure_date", "2018-2-28"},
            {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
            {"price",        "12550"},
            {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}
