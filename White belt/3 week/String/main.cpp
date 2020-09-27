

#include <string>
#include <iostream>

class ReversibleString {
private:
    std::string str;

public:
    ReversibleString() {}

    ReversibleString(const std::string &to_init) {
        str = to_init;
    }

    void Reverse() {
        for (int i = 0, j = str.size() - 1; i < j; i++, j--) {
            std::swap(str[i], str[j]);
        }
    }

    std::string ToString() const {
        return str;
    }


};

int main() {
    ReversibleString s("live");
    s.Reverse();
    std::cout << s.ToString() << std::endl;

    s.Reverse();
    const ReversibleString &s_ref = s;
    std::string tmp = s_ref.ToString();
    std::cout << tmp << std::endl;

    ReversibleString empty;
    std::cout << '"' << empty.ToString() << '"' << std::endl;

    return 0;
}
