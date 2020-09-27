#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <algorithm>
#include <vector>

using namespace std;

template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
            : first(begin), last(end), size_(distance(first, last)) {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

    size_t size() const {
        return size_;
    }

private:
    Iterator first, last;
    size_t size_;
};

template<typename Iterator>
class Paginator {
private:
    vector<IteratorRange<Iterator>> pages;

public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = distance(begin, end); left > 0;) {
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin, current_page_size);
            pages.push_back({begin, current_page_end});

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }

    size_t size() const {
        return pages.size();
    }
};

template<typename C>
auto Paginate(C &c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats &other) {
        for (const auto &[first, second]: other.word_frequences) {
            word_frequences[first] += second;
        }
    }
};

Stats ExploreLine(const set<string> &key_words, const string &line) {
    Stats result;
    string_view view = line;

    int pos = view.find_first_of(' ');
    while (pos != string::npos) {
        string word = string(view.substr(0, pos));
        view.remove_prefix(pos + 1);
        int count = key_words.count(word);

        if (count != 0)
            result.word_frequences[move(word)] += count;

        pos = view.find(' ');
    }
    string word = string(view.substr(0, string::npos));
    int count = key_words.count(word);

    if (count != 0)
        result.word_frequences[move(word)] += count;


    return move(result);
}

template<typename T>
Stats ExploreKeyWordsSingleThread(
        const set<string> &key_words, const T &strings
) {
    Stats result;
    for (const auto &line:strings) {
        result += ExploreLine(key_words, line);
    }
    return move(result);
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
    vector<future<Stats>> buffer;
    Stats result;
    vector<string> strings;

    {
        LOG_DURATION("Create vector")
        for (string line; getline(input, line);) {
            strings.push_back(move(line));
        }
    }

    {
        LOG_DURATION("async")
        for (const auto &page : Paginate(strings, strings.size() / 4)) {
            buffer.push_back(async([&key_words, page] { return ExploreKeyWordsSingleThread(key_words, page); }));
        }
    }

    {
        LOG_DURATION("get")
        for (future<Stats> &item : buffer) {
            result += item.get();
        }
    }

    return move(result);
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    for (int i = 0; i < 1; i++) {
        ss << "this new yangle service really rocks\n";
        ss << "It sucks when yangle isn't available\n";
        ss << "10 reasons why yangle is the best IT company\n";
        ss << "yangle rocks others suck\n";
        ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
    }
    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks",  2},
            {"sucks",  1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
   // RUN_TEST(tr, TestBasic);
}
