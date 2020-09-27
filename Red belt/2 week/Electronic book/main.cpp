#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

class ReadingManager {
public:
    ReadingManager() {}

    void Read(const int& user_id,const int& page_count) {
        pages_to_users[users_to_pages[user_id]].erase(user_id);
        if (pages_to_users[users_to_pages[user_id]].empty())
            pages_to_users.erase(users_to_pages[user_id]);
        users_to_pages[user_id] = page_count;
        pages_to_users[page_count].insert(user_id);
    }

    double Cheer(const int& user_id) const {
        if (users_to_pages.count(user_id) == 0) {
            return 0;
        }
        int user_count = users_to_pages.size();
        int position = 0;
        for (auto it = pages_to_users.begin(); it != pages_to_users.lower_bound(users_to_pages.at(user_id));
             it++) {
            position += it->second.size();
        }

        if (1 == user_count) {
            return 1;
        }

        if (position == user_count) {
            return 0;
        }

        return (position) * 1.0 / (user_count - 1);
    }

private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;

    map<int, int> users_to_pages;

    map<int, set<int>> pages_to_users;
};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << '\n';
        }
    }

    return 0;
}