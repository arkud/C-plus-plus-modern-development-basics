#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <deque>
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <future>

using namespace std;

template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T())
            : value(move(initial))
    {
    }

    struct Access {
        T& ref_to_value;
        lock_guard<mutex> guard;
    };

    Access GetAccess() {
        return {value, lock_guard(m)};
    }

private:
    T value;
    mutex m;
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    InvertedIndex(istream &document_input);

    void Add(string document);

    vector<pair<size_t, size_t>> Lookup(string_view word) const;

    const string &GetDocument(size_t id) const {
        return docs[id];
    }

    const auto &GetDocuments() const {
        return docs;
    }

private:
    deque<string> docs;
    map<string_view, vector<pair<size_t,size_t>>> index;
};

class SearchServer {
public:
    SearchServer() = default;

    explicit SearchServer(istream &document_input);

    void UpdateDocumentBaseSingleThread(istream &document_input);

    void UpdateDocumentBase(istream &document_input);

    void AddQueriesStreamSingleThread(istream &query_input, ostream &search_results_output);

    void AddQueriesStream(istream &query_input, ostream &search_results_output);

private:
    Synchronized<InvertedIndex> index;
    vector<future<void>> futures;
    mutex m;
    bool is_first_doc =true;
};
