#include <utility>
#include <algorithm>
#include <string>

template<typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end,
                                             const std::string &prefix) {
    std::string sec = prefix;
    sec.back()++;
    return {std::lower_bound(range_begin, range_end, prefix),
            std::lower_bound(range_begin, range_end, sec)
    };

}


