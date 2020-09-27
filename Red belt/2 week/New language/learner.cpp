#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
private:
    set<string> dict;

public:
    int Learn(const vector<string> &words) {
        size_t prev_size = dict.size();
        for (const auto &word : words) {
            dict.insert(word);
        }
        return dict.size() - prev_size;
    }

    vector<string> KnownWords() {
        return vector<string>(dict.begin(), dict.end());
    }
};
