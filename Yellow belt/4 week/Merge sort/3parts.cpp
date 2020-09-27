#include <vector>
#include <algorithm>

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) {
        return;
    }

    std::vector<typename RandomIt::value_type> elements(range_begin, range_end);

    int one_third = (range_end - range_begin) / 3;

    MergeSort(elements.begin(), elements.begin() + one_third);
    MergeSort(elements.begin() + one_third, elements.begin() + 2 * one_third);
    MergeSort(elements.end() - one_third, elements.end());

    std::vector<typename RandomIt::value_type> result1;
    std::merge(elements.begin(), elements.begin() + one_third, elements.begin() + one_third,
                                                                elements.begin() + 2 * one_third,
               std::back_inserter(result1));

    std::merge(result1.begin(), result1.end(), elements.end() - one_third, elements.end(), range_begin);
}