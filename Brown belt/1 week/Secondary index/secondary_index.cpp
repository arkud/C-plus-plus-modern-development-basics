#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class RecordHasher {
    hash<string> s_hasher;
    hash<int> i_hasher;
public:
    size_t operator()(const Record &record) const {
        return s_hasher(record.id);
    }
};

bool operator<(const Record &lhs, const Record &rhs) {
    return lhs.id < rhs.id &&
           lhs.timestamp < rhs.timestamp &&
           lhs.user < rhs.user &&
           lhs.karma < rhs.karma &&
           lhs.title < rhs.title;
}

bool operator==(const Record &lhs, const Record &rhs) {
    return lhs.id == rhs.id &&
           lhs.timestamp == rhs.timestamp &&
           lhs.user == rhs.user &&
           lhs.karma == rhs.karma &&
           lhs.title == rhs.title;
}

// Реализуйте этот класс
class Database {
private:
    unordered_map<string, Record> id_to_record;
    map<int, set<Record>> timestamp_to_record;
    map<int, set<Record>> karma_to_record;
    map<string, set<Record>> user_to_record;

private:
    template<typename Callback, typename It>
    void IterateThroughRange(It begin, It end, Callback callback) const {
        for (auto it = begin; it != end; it++) {
            for (const auto &item : it->second) {
                if (!callback(item))
                    return;
            }
        }
    }

public:
    bool Put(const Record &record) {
        if (id_to_record.find(record.id) != id_to_record.end())
            return false;
        id_to_record[record.id] = record;
        timestamp_to_record[record.timestamp].insert(record);
        karma_to_record[record.karma].insert(record);
        user_to_record[record.user].insert(record);
        return true;
    }

    const Record *GetById(const string &id) const {
        try {
            return &id_to_record.at(id);
        }
        catch (exception &) {
            return nullptr;
        }
    }

    bool Erase(const string &id) {
        auto it = id_to_record.find(id);
        if (it == id_to_record.end())
            return false;
        timestamp_to_record[it->second.timestamp].erase(it->second);
        karma_to_record[it->second.karma].erase(it->second);
        user_to_record[it->second.user].erase(it->second);
        id_to_record.erase(it);
        return true;
    }

    template<typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto begin = timestamp_to_record.lower_bound(low);
        if (begin == timestamp_to_record.end())
            return;
        auto end = timestamp_to_record.upper_bound(high);
        IterateThroughRange(begin, end, callback);
    }

    template<typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto begin = karma_to_record.lower_bound(low);
        if (begin == karma_to_record.end())
            return;
        auto end = karma_to_record.upper_bound(high);
        IterateThroughRange(begin, end, callback);
    }

    template<typename Callback>
    void AllByUser(const string &user, Callback callback) const {
        auto item = user_to_record.find(user);
        if (item == user_to_record.end())
            return;
        IterateThroughRange(item, next(item), callback);
    }
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record &) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record &) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}
