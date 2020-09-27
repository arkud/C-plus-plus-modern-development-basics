#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
    T *Allocate() {
        T *result;
        if (released.empty()) {
            result = new T;
        } else {
            result = released.front();
            released.pop();
        }
        allocated.insert(result);
        return result;
    }

    T *TryAllocate() {
        T *result;
        if (released.empty()) {
            return nullptr;
        } else {
            result = released.front();
            released.pop();
        }
        allocated.insert(result);
        return result;
    }

    void Deallocate(T *object) {
        if (allocated.find(object) == allocated.end()) {
            throw invalid_argument("");
        } else {
            released.push(object);
            allocated.erase(object);
        }
    }

    ~ObjectPool() {
        for (auto &item : allocated) {
            delete item;
        }

        while (!released.empty()) {
            delete released.front();
            released.pop();
        }

    }

private:
    queue<T *> released;
    set<T *> allocated;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
