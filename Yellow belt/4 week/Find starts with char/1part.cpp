#include <utility>
#include <algorithm>
#include <string>

template<typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end,
                                             char prefix) {
    return {std::lower_bound(range_begin, range_end, std::string(1, prefix++)),
            std::lower_bound(range_begin, range_end, std::string(1, prefix))
    };

}


