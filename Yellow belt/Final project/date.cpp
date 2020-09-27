#include <tuple>
#include <iomanip>
#include "date.h"


Date ParseDate(istream &is) {
    Date result;

    string buffer;
    getline(is, buffer, '-');
    result.year = stoi(buffer);

    getline(is, buffer, '-');
    result.month = stoi(buffer);

    getline(is, buffer, ' ');
    result.day = stoi(buffer);

    return result;
}

bool operator<(const Date &left, const Date &right) {
    return make_tuple(left.year, left.month, left.day) < make_tuple(right.year, right.month, right.day);
}

bool operator==(const Date &left, const Date &right) {
    return make_tuple(left.year, left.month, left.day) == make_tuple(right.year, right.month, right.day);
}

bool operator<=(const Date &left, const Date &right) {
    return make_tuple(left.year, left.month, left.day) <= make_tuple(right.year, right.month, right.day);
}

bool operator>=(const Date &left, const Date &right) {
    return make_tuple(left.year, left.month, left.day) >= make_tuple(right.year, right.month, right.day);
}

bool operator!=(const Date &left, const Date &right) {
    return make_tuple(left.year, left.month, left.day) != make_tuple(right.year, right.month, right.day);
}

bool operator>(const Date &left, const Date &right) {
    return make_tuple(left.year, left.month, left.day) > make_tuple(right.year, right.month, right.day);
}

ostream &operator<<(ostream &os, const Date &date) {
    os << std::setfill('0') << std::setw(4) << date.year << "-" << std::setw(2) << date.month << "-"
       << std::setw(2) << date.day;
    return os;
}

