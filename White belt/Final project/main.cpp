#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <stdexcept>

struct Date {
    int year;
    int month;
    int day;
};

bool operator<(const Date &left, const Date &right) {
    if (left.year < right.year)
        return true;
    else if (left.year == right.year) {
        if (left.month < right.month)
            return true;
        else if (left.month == right.month) {
            if (left.day < right.day)
                return true;
        }
    }

    return false;
}

class Database {
public:
    Date parse_date(const std::string &arg_date) const {
        std::stringstream stream(arg_date);

        if (arg_date.size() < 5)
            throw std::invalid_argument("Wrong date format: " + arg_date);
        if (arg_date.substr(0, 2) == "--" || arg_date.substr(0, 2) == "++")
            throw std::invalid_argument("Wrong date format: " + arg_date);

        for (int i = 0, j = 0; i < arg_date.size(); i++) {
            if (!isdigit(arg_date[i])) {
                if (arg_date[i] == '-') {
                    if (i != 0 && arg_date[i - 1] != '-' && i != arg_date.size() - 1 && arg_date[i + 1] != '-')
                        j++;
                    if (i == arg_date.size() - 1)
                        throw std::invalid_argument("Wrong date format: " + arg_date);
                    if (arg_date[i - 1] == '-' && i != arg_date.size() - 1 && arg_date[i + 1] == '+')
                        throw std::invalid_argument("Wrong date format: " + arg_date);
                    if (arg_date[i - 1] == '-' && i != arg_date.size() - 1 && arg_date[i + 1] == '-')
                        throw std::invalid_argument("Wrong date format: " + arg_date);
                } else {
                    if (arg_date[i] == '+') {
                        if (i == arg_date.size() - 1)
                            throw std::invalid_argument("Wrong date format: " + arg_date);
                        if (arg_date[i - 1] == '+' || (i != arg_date.size() - 1 && arg_date[i + 1] == '-'))
                            throw std::invalid_argument("Wrong date format: " + arg_date);
                    } else
                        throw std::invalid_argument("Wrong date format: " + arg_date);
                }
            }
            if (j > 2)
                throw std::invalid_argument("Wrong date format: " + arg_date);
        }

        Date result{};

        stream >> result.year;
        stream.ignore(1);

        stream >> result.month;
        stream.ignore(1);

        if (result.month <= 0 || result.month > 12)
            throw std::invalid_argument("Month value is invalid: " + std::to_string(result.month));

        stream >> result.day;

        if (result.day <= 0 || result.day > 31)
            throw std::invalid_argument("Day value is invalid: " + std::to_string(result.day));

        return result;
    }

    void add(const std::string &arg_date, const std::string &arg_event) {
        pf_events[parse_date(arg_date)].insert(arg_event);
    }

    void del(const std::string &arg_date, const std::string &arg_event) {
        Date date = parse_date(arg_date);
        if (pf_events.count(date) == 0 || pf_events[date].count(arg_event) == 0) {
            std::cout << "Event not found";
            return;
        }

        pf_events[date].erase(arg_event);
        std::cout << "Deleted successfully";
    }

    void del(const std::string &arg_date) {
        Date date = parse_date(arg_date);
        if (pf_events.count(date) == 0) {
            std::cout << "Deleted 0 events";
            return;
        }

        int events_count = pf_events[date].size();
        pf_events.erase(date);
        std::cout << "Deleted " << events_count << " events";
    }

    void find(const std::string &arg_date) const {
        Date date = parse_date(arg_date);
        if (pf_events.count(date) == 0)
            return;

        int i = 0;
        for (const auto &event : pf_events.at(date)) {
            std::cout << event;
            i++;
            if (i != pf_events.at(date).size())
                std::cout << std::endl;
        }
    }

    void print() const {
        int j = 0;
        for (const auto&[date, events] : pf_events) {
            int i = 0;
            for (const auto &event : events) {
                std::cout << std::setfill('0') << std::setw(4) << date.year << "-" << std::setw(2) << date.month << "-"
                          << std::setw(2) << date.day << " " << event;
                i++;
                if (i != events.size())
                    std::cout << std::endl;
            }
            j++;
            if (j != pf_events.size())
                std::cout << std::endl;
        }
    }

private:
    std::map<Date, std::set<std::string>> pf_events;
};

void execute_command(Database &arg_database, const std::string &arg_command) {
    std::stringstream stream(arg_command);

    std::string action;
    stream >> action;

    std::string date;
    std::string event;

    if (action == "Add") {
        stream >> date >> event;

        arg_database.add(date, event);
    } else if (action == "Del") {
        stream >> date;
        stream >> event;

        if (stream) {
            arg_database.del(date, event);
        } else {
            arg_database.del(date);
        }

    } else if (action == "Print") {
        arg_database.print();
    } else if (action == "Find") {
        stream >> date;
        arg_database.find(date);
    } else
        throw std::invalid_argument("Unknown command: " + action);
}


int main() {
    Database db;

    try {
        std::string command;
        int i = 0;
        while (getline(std::cin, command)) {
            if (!command.empty()) {
                if (i != 0 && command.substr(0, 4) != "Add ") {
                    std::cout << std::endl;
                }
                if (command.substr(0, 4) != "Add ")
                    i++;
                execute_command(db, command);
            }
        }
    } catch (std::invalid_argument &err) {
        std::cout << err.what();
    }
    std::cout << std::endl;

    return 0;
}
