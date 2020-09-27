#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2)
        return;

    int size = range_end - range_begin;
    vector<typename RandomIt::value_type> copy(make_move_iterator(range_begin), make_move_iterator(range_end));

    MergeSort(copy.begin(), copy.begin() + size / 3);
    MergeSort(copy.begin() + size / 3, copy.begin() + 2 * size / 3);
    MergeSort(copy.begin() + 2 * size / 3, copy.end());

    vector<typename RandomIt::value_type> buff;
    merge(make_move_iterator(copy.begin()), make_move_iterator(copy.begin() + size / 3),
          make_move_iterator(copy.begin() + size / 3), make_move_iterator(copy.begin() + 2 * size / 3),
          back_inserter(buff));

    merge(make_move_iterator(buff.begin()), make_move_iterator(buff.end()),
          make_move_iterator(copy.begin() + 2 * size / 3), make_move_iterator(copy.end()),
          range_begin);
}

void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}
