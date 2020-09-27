#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Date {
    int year = 0;
    int month = 0;
    int day = 0;

    Date(){

    }

    Date(int year, int month, int day) : year(year), month(month), day(day) {

    }
};

Date ParseDate(istream &is);

bool operator<(const Date &left, const Date &right);

bool operator==(const Date &left, const Date &right);

bool operator<=(const Date &left, const Date &right);

bool operator>=(const Date &left, const Date &right);

bool operator!=(const Date &left, const Date &right);

bool operator>(const Date &left, const Date &right);

ostream &operator<<(ostream &os, const Date &date);