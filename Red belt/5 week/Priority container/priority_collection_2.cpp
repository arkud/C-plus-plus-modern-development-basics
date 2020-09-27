#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <deque>
#include <list>

using namespace std;

template<typename T>
class PriorityCollection {
public:
    using Id = int;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        data.push_back({move(object), 0});
        Id id = data.size() - 1;
        sorted.insert({0, id});
        ids.insert(id);
        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for (auto it = range_begin; it != range_end; it++) {
            *ids_begin = Add(move(*it));
            ids_begin++;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return ids.find(id) != ids.end();
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const {
        return data[id].first;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        sorted.erase({data[id].second, id});
        data[id].second++;
        sorted.insert({data[id].second, id});
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const {
        auto elem = prev(sorted.end());
        return data[elem->second];
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto it = prev(sorted.end());
        pair<T, int> result = {move(data[it->second].first), it->first};
        ids.erase(prev(sorted.end())->second);
        sorted.erase(prev(sorted.end()));
        return result;
    }

private:
    vector<pair<T, int>> data;
    set<Id> ids;
    set<pair<int, Id>> sorted;
};


class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable &) = delete;

    StringNonCopyable(StringNonCopyable &&) = default;

    StringNonCopyable &operator=(const StringNonCopyable &) = delete;

    StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

void TestSpeed() {
    PriorityCollection<StringNonCopyable> strings;
    vector<PriorityCollection<StringNonCopyable>::Id> ids;
    ids.reserve(10000);
    {
        LOG_DURATION("Add")
        for (int i = 0; i < 10000; i++)
            ids.push_back(strings.Add("white"));
    }

    {
        LOG_DURATION("Promote")
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 100000; i++) {
                strings.Promote(ids[j]);
            }
        }
    }


}

int main() {
    TestRunner tr;
   // RUN_TEST(tr, TestNoCopy);
   // RUN_TEST(tr, TestSpeed);
    return 0;
}
