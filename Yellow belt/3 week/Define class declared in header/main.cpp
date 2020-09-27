#include <stdexcept>
#include "phone_number.h"

PhoneNumber::PhoneNumber(const string &international_number) {
    if (international_number[0] != '+')
        throw std::invalid_argument("+ is missing");

    size_t pos = 0;
    for (size_t i = 0; i < international_number.size(); i++) {
        if (international_number[i] == '-') {
            pos = i;
            break;
        }
        pos = i;
    }
    country_code_ = international_number.substr(1, pos - 1);

    size_t prev_pos = pos;
    for (size_t i = pos + 1; i < international_number.size(); i++) {
        if (international_number[i] == '-') {
            pos = i;
            break;
        }
        pos = i;
    }

    if (pos == international_number.size())
        throw std::invalid_argument("invalid input");

    city_code_ = international_number.substr(prev_pos + 1, pos - prev_pos - 1);
    local_number_ = international_number.substr(pos + 1, international_number.size() - pos - 1);

    if (country_code_.empty() || city_code_.empty() || local_number_.empty()) {
        throw std::invalid_argument("invalid input");
    }

}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}
