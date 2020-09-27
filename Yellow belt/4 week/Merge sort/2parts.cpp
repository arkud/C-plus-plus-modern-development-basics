#include <algorithm>
#include <vector>

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) {
        return;
    }

    std::vector<typename RandomIt::value_type> elements(range_begin, range_end);

    int middle = (range_end - range_begin) / 2;
    MergeSort(elements.begin(), elements.begin() + middle);
    MergeSort(elements.end() - middle, elements.end());

    std::merge(elements.begin(), elements.begin() + middle, elements.end() - middle, elements.end(), range_begin);
}

