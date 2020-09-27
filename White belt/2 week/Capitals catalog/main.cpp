#include <iostream>
#include <map>

void change_capital(std::map<std::string, std::string> &arg_capitals_catalog, const std::string &arg_country,
                    const std::string &arg_new_capital) {
    if (arg_capitals_catalog.count(arg_country) == 0)
        std::cout << "Introduce new country " << arg_country << " with capital " << arg_new_capital << std::endl;
    else if (arg_capitals_catalog[arg_country] == arg_new_capital)
        std::cout << "Country " << arg_country << " hasn't changed its capital" << std::endl;
    else {
        std::cout << "Country " << arg_country << " has changed its capital from " << arg_capitals_catalog[arg_country]
                  << " to " << arg_new_capital << std::endl;
    }

    arg_capitals_catalog[arg_country] = arg_new_capital;


}

void rename(std::map<std::string, std::string> &arg_capitals_catalog, const std::string &arg_old_country_name,
            const std::string &arg_new_country_name) {

    if (arg_old_country_name == arg_new_country_name || arg_capitals_catalog.count(arg_old_country_name) == 0 ||
        arg_capitals_catalog.count(arg_new_country_name) == 1) {
        std::cout << "Incorrect rename, skip" << std::endl;
        return;
    }

    arg_capitals_catalog[arg_new_country_name] = arg_capitals_catalog[arg_old_country_name];
    arg_capitals_catalog.erase(arg_old_country_name);

    std::cout << "Country " << arg_old_country_name << " with capital " << arg_capitals_catalog[arg_new_country_name]
              << " has been renamed to " << arg_new_country_name << std::endl;
}

void about(std::map<std::string, std::string> &arg_capitals_catalog, const std::string &arg_country) {
    if (arg_capitals_catalog.count(arg_country) != 0)
        std::cout << "Country " << arg_country << " has capital " << arg_capitals_catalog[arg_country] << std::endl;
    else
        std::cout << "Country " << arg_country << " doesn't exist" << std::endl;
}

void dump(const std::map<std::string, std::string> &arg_capitals_catalog) {
    if (!arg_capitals_catalog.empty()) {
        for (const auto &[country, capital] : arg_capitals_catalog) {
            std::cout << country << "/" << capital << " ";
        }
        std::cout << std::endl;
    } else
        std::cout << "There are no countries in the world" << std::endl;
}

int main() {
    int n;
    std::cin >> n;

    std::map<std::string, std::string> capitals_catalog;

    for (int i = 0; i < n; i++) {
        std::string action;
        std::string first_parameter, second_parameter;

        std::cin >> action;

        if (action == "CHANGE_CAPITAL") {
            std::cin >> first_parameter >> second_parameter;
            change_capital(capitals_catalog, first_parameter, second_parameter);
        } else if (action == "RENAME") {
            std::cin >> first_parameter >> second_parameter;
            rename(capitals_catalog, first_parameter, second_parameter);
        } else if (action == "ABOUT") {
            std::cin >> first_parameter;
            about(capitals_catalog, first_parameter);
        } else {
            dump(capitals_catalog);
        }
    }

    return 0;
}
