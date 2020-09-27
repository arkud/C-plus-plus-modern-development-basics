#include <iostream>
#include <vector>
#include <map>

using namespace std;

template<typename T>
T Sqr(T v);

template<typename T>
std::vector<T> Sqr(std::vector<T> v);

template<typename First, typename Second>
std::map<First, Second> Sqr(std::map<First, Second> m);

template<typename First, typename Second>
std::pair<First, Second> Sqr(std::pair<First, Second> p);


template<typename T>
T Sqr(T v) {
    return v * v;
}

template<typename T>
std::vector<T> Sqr(std::vector<T> v) {
    for (auto &item : v) {
        item = Sqr(item);
    }
    return v;
}

template<typename First, typename Second>
std::map<First, Second> Sqr(std::map<First, Second> m) {
    for (auto &[first, second] : m) {
        second = Sqr(second);
    }
    return m;
}

template<typename First, typename Second>
std::pair<First, Second> Sqr(std::pair<First, Second> p) {
    p.first = Sqr(p.first);
    p.second = Sqr(p.second);
    return p;
}

int main() {
    // Пример вызова функции
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
        cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
            {4, {2, 2}},
            {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto &x : Sqr(map_of_pairs)) {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
}