#include <iostream>
#include <vector>


void add(std::vector<std::vector<std::string>> &arg_v, int arg_i, const std::string &arg_s) {
    arg_v[arg_i].push_back(arg_s);
}

void dump(std::vector<std::vector<std::string>> &arg_v, int arg_i) {

    std::cout << arg_v[arg_i].size() << " ";
    for (const auto &item: arg_v[arg_i]) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

}

void next(std::vector<std::vector<std::string>> &arg_v, int &current_month) {
    std::vector<int> days_in_months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int next_month = current_month + 1;
    if (next_month == 12)
        next_month = 0;

    if (days_in_months[current_month] > days_in_months[next_month]) {
        for (int i = days_in_months[next_month]; i < days_in_months[current_month]; i++) {
            arg_v[days_in_months[next_month] - 1].insert(arg_v[days_in_months[next_month] - 1].end(),
                                                         arg_v[i].begin(),
                                                         arg_v[i].end());
        }
    }
    arg_v.resize(days_in_months[next_month]);

    current_month = next_month;
}

int main() {
    std::vector<std::vector<std::string>> monthly_concerns(31);

    int n;
    std::cin >> n;

    int current_month = 0;
    for (int i = 0; i < n; i++) {
        std::string action;
        std::cin >> action;

        int index;
        std::string concern;

        if (action == "ADD") {
            std::cin >> index;
            std::cin >> concern;
            add(monthly_concerns, index - 1, concern);
        } else if (action == "DUMP") {
            std::cin >> index;
            dump(monthly_concerns, index - 1);
        } else {
            next(monthly_concerns, current_month);
        }
    }


    return 0;
}
