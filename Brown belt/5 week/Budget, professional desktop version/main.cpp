//
// Created by ar_kud on 04.09.2020.
//

#include <ctime>
#include <tuple>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include "test_runner.h"
#include "profile.h"

using namespace std;

class Date {
    int day_, month_, year_;

public:
    Date(string_view date) {
        year_ = stoi(string(date.substr(0, 4)));
        date.remove_prefix(5);

        month_ = stoi(string(date.substr(0, 2)));
        date.remove_prefix(3);

        day_ = stoi(string(date));
    }

    string ToString() const {
        stringstream ss;
        ss << to_string(year_) << "-" << setw(2) << setfill('0')
           << to_string(month_) << "-" << setw(2) << to_string(day_);
        return ss.str();
    }

    time_t AsTimestamp() const {
        std::tm t;
        t.tm_sec = 0;
        t.tm_min = 0;
        t.tm_hour = 0;
        t.tm_mday = day_;
        t.tm_mon = month_ - 1;
        t.tm_year = year_ - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }

    void NextDay() {
        if (day_ < 28) {
            day_++;
            return;
        }

        if (month_ != 2) {
            if (month_ == 1 || month_ == 3 || month_ == 5 || month_ == 7 || month_ == 8 || month_ == 10 ||
                month_ == 12) {
                if (day_ == 31) {
                    day_ = 1;
                    if (month_ != 12) {
                        month_++;
                    } else {
                        month_ = 1;
                        year_++;
                    }
                } else {
                    day_++;
                }
            } else {
                if (day_ == 30) {
                    day_ = 1;
                    month_++;
                } else {
                    day_++;
                }
            }
            return;
        }

        if (day_ == 29) {
            month_++;
            day_ = 1;
            return;
        }

        if (year_ % 4 == 0) {
            day_++;
        } else {
            day_ = 1;
            month_++;
        }
    }

    friend bool operator<(const Date &lhs, const Date &rhs);
};

bool operator<(const Date &lhs, const Date &rhs) {
    return make_tuple(lhs.year_, lhs.month_, lhs.day_) < make_tuple(rhs.year_, rhs.month_, rhs.day_);
}

int ComputeDaysDiff(const Date &date_to, const Date &date_from) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

class BudgetManager {
    struct Money {
        double earnings = 0;
        double spendings = 0;
    };

    map<Date, Money> money;

    template<typename Callback>
    void IterateThroughDates(const Date &from, const Date &to, Callback callback) {
        const auto it_from = money.lower_bound(from);
        const auto it_to = money.upper_bound(to);

        for (auto it = it_from; it != it_to; it++) {
            callback(it);
        }
    }

public:
    BudgetManager() {}

    double ComputeIncome(const Date &from, const Date &to) {
        double result = 0;

        IterateThroughDates(from, to, [&result](const map<Date, Money>::iterator &item) {
            result += item->second.earnings - item->second.spendings;
        });

        return result;
    }

    void Earn(const Date &from, const Date &to, double amount) {
        const int day_count = ComputeDaysDiff(to, from) + 1;
        const double earnings_per_day = amount / day_count;

        Date current_date = from;

        for (int i = 0; i < day_count; current_date.NextDay(), i++) {
            money[current_date].earnings += earnings_per_day;
        }
    }

    void PayTax(const Date &from, const Date &to, int percentage) {
        IterateThroughDates(from, to, [this, percentage](map<Date, Money>::iterator &item) {
            item->second.earnings *= (double) (100 - percentage) / 100;
        });
    }

    void Spend(const Date &from, const Date &to, double amount) {
        const int day_count = ComputeDaysDiff(to, from) + 1;
        const double spendings_per_day = amount / day_count;

        Date current_date = from;

        for (int i = 0; i < day_count; current_date.NextDay(), i++) {
            money[current_date].spendings += spendings_per_day;
        }
    }


};

class BudgetRequestHandler {
    BudgetManager &bm;
public:
    explicit BudgetRequestHandler(BudgetManager &bm) : bm(bm) {}

    optional<double> ProcessRequest(string_view line) {
        int pos = line.find(' ');
        string_view request = line.substr(0, pos);
        line.remove_prefix(pos + 1);

        pos = line.find(' ');
        Date from(line.substr(0, pos));
        line.remove_prefix(pos + 1);

        pos = line.find(' ');
        Date to(line.substr(0, pos));
        line.remove_prefix(pos + 1);

        if (request == "Earn") {
            int amount = atoi(string(line).c_str());
            bm.Earn(from, to, amount);
        } else if (request == "ComputeIncome") {
            return bm.ComputeIncome(from, to);
        } else if (request == "PayTax") {
            int tax_percentage = atoi(string(line).c_str());
            bm.PayTax(from, to, tax_percentage);
        } else if (request == "Spend") {
            int amount = atoi(string(line).c_str());
            bm.Spend(from, to, amount);
        }

        return nullopt;
    }
};

void TestDate() {
    {
        Date date("2000-01-02");

        ASSERT_EQUAL(date.ToString(), "2000-01-02")
        date.NextDay();
        ASSERT_EQUAL(date.ToString(), "2000-01-03")
    }

    {
        Date date("2020-02-29");
        date.NextDay();
        ASSERT_EQUAL(date.ToString(), "2020-03-01")
    }

    {
        Date date1("2020-01-05");
        Date date2("2020-01-15");

        ASSERT_EQUAL(10, ComputeDaysDiff(date2, date1))
    }

    {
        Date date1("2004-02-29");
        Date date2("2004-03-01");

        ASSERT_EQUAL(1, ComputeDaysDiff(date2, date1))
        ASSERT_EQUAL(0, ComputeDaysDiff(date2, date2))
    }

    {
        Date date("2004-02-27");
        date.NextDay();

        ASSERT_EQUAL(date.ToString(), "2004-02-28")
        date.NextDay();
        ASSERT_EQUAL(date.ToString(), "2004-02-29")
        date.NextDay();
        ASSERT_EQUAL(date.ToString(), "2004-03-01")
    }

    {
        Date date1("2010-01-20");
        Date date2("2026-01-01");

        ASSERT_EQUAL(true, date1 < date2)
    }

    {
        Date date1("2010-01-01");
        Date date2("2009-12-31");
        date2.NextDay();

        ASSERT_EQUAL(date1.ToString(), date2.ToString())
    }

}

void TestBudget() {
    BudgetManager bm;

    Date date1("2020-02-03");
    Date date2("2020-03-03");
    Date date3("2020-02-20");

    ASSERT_EQUAL(bm.ComputeIncome(date1, date2), 0);
    bm.Earn(date1, date3, 100);

    ASSERT_EQUAL(bm.ComputeIncome(date1, date1), (double) 100 / 18);

    bm.PayTax(date1, date1, 13);
    ASSERT_EQUAL(bm.ComputeIncome(date1, date1), (double) 100 / 18 * 0.87);


}

void TestExample() {
    stringstream request;
    request <<
            "Earn 2000-01-02 2000-01-06 20\n"
            "ComputeIncome 2000-01-01 2001-01-01\n"
            "PayTax 2000-01-02 2000-01-03 13\n"
            "ComputeIncome 2000-01-01 2001-01-01\n"
            "Spend 2000-12-30 2001-01-02 14\n"
            "ComputeIncome 2000-01-01 2001-01-01\n"
            "PayTax 2000-12-30 2000-12-30 13\n"
            "ComputeIncome 2000-01-01 2001-01-01";

    BudgetManager bm;
    BudgetRequestHandler handler(bm);
    stringstream sm;

    string line;
    for (int i = 0; i < 8; i++) {
        getline(request, line);
        {
            LOG_DURATION(line);
            auto result = handler.ProcessRequest(line);
            if (result) {
                sm << result.value() << "\n";
            }
        }
    }

    ASSERT_EQUAL(sm.str(), "20\n18.96\n8.46\n8.46\n")
}

void TestSpeed() {
    {
        LOG_DURATION("TestSpeed")

        BudgetManager bm;
        BudgetRequestHandler handler(bm);

        for (int i = 0; i < 1; i++)
            handler.ProcessRequest("Earn 2000-01-01 2050-12-12 21000");
    }
}

void Test() {
    TestRunner tr;
    RUN_TEST(tr, TestDate);
    RUN_TEST(tr, TestBudget);
    RUN_TEST(tr, TestExample);
    RUN_TEST(tr, TestSpeed);
}

int main() {
    Test();
    int queries;
    cin >> queries;
    cin.ignore(1);

    cout.precision(25);

    string line;
    BudgetManager bm;
    BudgetRequestHandler handler(bm);

    for (int i = 0; i < queries; i++) {
        getline(cin, line);
        auto result = handler.ProcessRequest(line);
        if (result) {
            cout << result.value() << "\n";
        }
    }
}