#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

using namespace std;

class DomainFilter {
private:
    unordered_set<string> banned_domains;
public:
    DomainFilter() {

    }

    void ban_domain(string domain) {
        banned_domains.insert(move(domain));
    }

    bool is_banned(string_view domain) {
        while (!domain.empty()) {
            if (banned_domains.find(string(domain)) != banned_domains.end())
                return true;
            size_t pos = domain.find('.');
            if (pos == string::npos)
                break;
            domain.remove_prefix(pos + 1);
        }

        return false;
    }
};

DomainFilter ReadBannedDomains(istream &in = std::cin) {
    size_t count;
    in >> count;
    in.ignore();

    DomainFilter domains;
    for (size_t i = 0; i < count; ++i) {
        string domain;
        getline(in, domain);
        domains.ban_domain(move(domain));
    }
    return domains;
}


int main() {
    DomainFilter filter = ReadBannedDomains(std::cin);

    size_t count;
    cin >> count;

    for (size_t i = 0; i < count; i++) {
        string domain;
        cin >> domain;
        if (filter.is_banned(domain)) {
            cout << "Bad\n";
        } else {
            cout << "Good\n";
        }
    }
    return 0;
}
