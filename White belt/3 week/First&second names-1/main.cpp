#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

struct Name {
    std::string first_name;
    std::string last_name;
};

class Person {
private:
    std::map<int, Name> pf_year_to_name;
    std::vector<int> pf_years;
    std::map<int, std::pair<bool, bool>> pf_is_name_set;
    int pf_birth_year;

public:

    Person(const std::string &arg_first_name, const std::string &arg_last_name, const int &arg_birth_year) {
        pf_year_to_name[arg_birth_year].first_name = arg_first_name;
        pf_year_to_name[arg_birth_year].last_name = arg_last_name;
        pf_years.push_back(arg_birth_year);
        pf_birth_year = arg_birth_year;
        pf_is_name_set[arg_birth_year].first = true;
        pf_is_name_set[arg_birth_year].second = true;
    }

    void ChangeFirstName(int year, const std::string &first_name) {
        if (year < pf_birth_year)
            return;

        UpdateYear(year);

        pf_year_to_name[year].first_name = first_name;
        pf_is_name_set[year].first = true;

        UpdateData();
    }

    void ChangeLastName(int year, const std::string &last_name) {
        UpdateYear(year);

        pf_year_to_name[year].last_name = last_name;
        pf_is_name_set[year].second = true;

        UpdateData();
    }

    std::string GetFullName(int year) const{
        if (year < pf_birth_year)
            return "No person";

        if (pf_years.empty() || year < pf_years.front())
            return "Incognito";
        if (year >= pf_years.back())
            year = pf_years.back();

        if (pf_year_to_name.count(year) == 0) {
            for (int i = 0; i < pf_years.size(); i++) {
                if (pf_years[i] <= year && pf_years[i + 1] > year)
                    year = pf_years[i];
            }
        }

        if (pf_year_to_name.at(year).first_name.empty())
            return pf_year_to_name.at(year).last_name + " with unknown first name";
        else if (pf_year_to_name.at(year).last_name.empty())
            return pf_year_to_name.at(year).first_name + " with unknown last name";
        else
            return pf_year_to_name.at(year).first_name + " " + pf_year_to_name.at(year).last_name;
    }

    std::string GetFullNameWithHistory(int year) const{
        if (year < pf_birth_year)
            return "No person";

        if (pf_years.empty() || year <= pf_years.front())
            return GetFullName(year);
        if (year >= pf_years.back()) {
            year = pf_years.back();
        }

        if (pf_year_to_name.count(year) == 0) {
            for (int i = 0; i < pf_years.size(); i++) {
                if (pf_years[i] <= year && pf_years[i + 1] > year)
                    year = pf_years[i];
            }
        }

        std::string first_name = pf_year_to_name.at(year).first_name + " (";
        std::string last_name = pf_year_to_name.at(year).last_name + " (";

        std::string prev_first_name = pf_year_to_name.at(year).first_name;
        std::string prev_last_name = pf_year_to_name.at(year).last_name;
        for (int i = pf_years.size() - 1; i >= 0; i--) {
            if (pf_years[i] < year) {
                if (pf_is_name_set.at(pf_years[i]).first && prev_first_name != pf_year_to_name.at(pf_years[i]).first_name) {
                    first_name += pf_year_to_name.at(pf_years[i]).first_name + ", ";
                    prev_first_name = pf_year_to_name.at(pf_years[i]).first_name;
                }
                if (pf_is_name_set.at(pf_years[i]).second && prev_last_name != pf_year_to_name.at(pf_years[i]).last_name) {
                    last_name += pf_year_to_name.at(pf_years[i]).last_name + ", ";
                    prev_last_name = pf_year_to_name.at(pf_years[i]).last_name;
                }
            }
        }

        first_name.erase(first_name.end() - 2, first_name.end());
        last_name.erase(last_name.end() - 2, last_name.end());

        if (first_name.size() > pf_year_to_name.at(year).first_name.size())
            first_name += ")";
        if (last_name.size() > pf_year_to_name.at(year).last_name.size())
            last_name += ")";

        if (first_name.empty())
            return last_name + " with unknown first name";
        else if (last_name.empty())
            return first_name + " with unknown last name";
        else
            return first_name + " " + last_name;


    }

private:
    void UpdateYear(int year) {
        if (pf_year_to_name.count(year) == 0) {
            pf_years.push_back(year);
            std::sort(pf_years.begin(), pf_years.end());
        }
    }

    void UpdateData() {
        int i = 0;
        for (const auto&[item, second] : pf_is_name_set) {
            if (i == 0) {
                i++;
                continue;
            }
            if (!pf_is_name_set[item].first)
                pf_year_to_name[pf_years[i]].first_name = pf_year_to_name[pf_years[i - 1]].first_name;
            if (!pf_is_name_set[item].second)
                pf_year_to_name[pf_years[i]].last_name = pf_year_to_name[pf_years[i - 1]].last_name;
            i++;
        }
    }
};


int main() {
    Person person("Polina", "Sergeeva", 1960);
    for (int year : {1959, 1960}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    person.ChangeFirstName(1965, "Appolinaria");
    person.ChangeLastName(1967, "Ivanova");
    for (int year : {1965, 1967}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    return 0;
}




