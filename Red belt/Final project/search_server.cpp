#include "search_server.h"
#include "iterator_range.h"
#include "paginator.cpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>
#include <future>


void LeftStrip(string_view &sv) {
    while (!sv.empty() && isspace(sv[0])) {
        sv.remove_prefix(1);
    }
}

string_view ReadToken(string_view &sv) {
    LeftStrip(sv);

    auto pos = sv.find(' ');
    auto result = sv.substr(0, pos);
    sv.remove_prefix(pos != std::string_view::npos ? pos : sv.size());
    return result;
}

vector<string_view> SplitIntoWords(string_view str) {
    vector<string_view> result;

    for (string_view word = ReadToken(str); !word.empty(); word = ReadToken(str)) {
        result.push_back(word);
    }

    return result;
}

SearchServer::SearchServer(istream &document_input) {
    UpdateDocumentBase(document_input);
}

InvertedIndex::InvertedIndex(istream &document_input) {
    for (string current_document; getline(document_input, current_document);) {
        docs.push_back(move(current_document));

        const size_t docid = docs.size() - 1;
        for (string_view word : SplitIntoWords(docs.back())) {
            auto &docids = index[word];
            if (!docids.empty() && docids.back().first == docid) {
                ++docids.back().second;
            } else {
                docids.emplace_back(docid, 1);
            }
        }
    }
}

void SearchServer::UpdateDocumentBaseSingleThread(istream &document_input) {
    InvertedIndex new_index(document_input);

    lock_guard<mutex> g(m);
    index.GetAccess().ref_to_value = move(new_index);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
    if (is_first_doc) {
        UpdateDocumentBaseSingleThread(document_input);
        is_first_doc = false;
    } else {
        futures.push_back(async([this, &document_input] { UpdateDocumentBaseSingleThread(document_input); }));
    }
}


void SearchServer::AddQueriesStreamSingleThread(istream &query_input, ostream &search_results_output) {
    while (is_first_doc) {}

    size_t size;
    {
        lock_guard<mutex> g(m);
        size = index.GetAccess().ref_to_value.GetDocuments().size();
    }

    vector<size_t> docid_count(size);
    vector<size_t> docids(size);

    for (string current_query; getline(query_input, current_query);) {
        vector<string_view> words;
        words = SplitIntoWords(current_query);

        for (const auto &word : words) {
            lock_guard<mutex> g(m);
            for (const auto&[docid, hit_count] : index.GetAccess().ref_to_value.Lookup(word)) {
                docid_count[docid] += hit_count;
            }
        }

        iota(docids.begin(), docids.end(), 0);

        if (docid_count.size() <= 5) {
            sort(
                    begin(docids),
                    end(docids),
                    [&docid_count](const size_t &lhs, const size_t &rhs) {
                        int64_t lhs_docid = lhs;
                        int64_t rhs_docid = rhs;
                        return make_pair(docid_count[lhs], -lhs_docid) > make_pair(docid_count[rhs], -rhs_docid);
                    }
            );
        } else {
            partial_sort(
                    begin(docids),
                    begin(docids) + 5,
                    end(docids),
                    [&docid_count](const size_t &lhs, const size_t &rhs) {
                        int64_t lhs_docid = lhs;
                        int64_t rhs_docid = rhs;
                        return make_pair(docid_count[lhs], -lhs_docid) > make_pair(docid_count[rhs], -rhs_docid);
                    }
            );
        }

        search_results_output << current_query << ':';
        for (const auto &docid : Head(docids, 5)) {
            const auto hitcount = docid_count[docid];
            if (hitcount == 0)
                break;
            search_results_output << " {docid: " << docid << ", hitcount: " << hitcount << '}';
        }
        search_results_output<< '\n';
        docid_count.assign(docid_count.size(), 0);
    }
}

void SearchServer::AddQueriesStream(
        istream &query_input, ostream &search_results_output
) {
    /*futures.push_back(async([this, &query_input, &search_results_output] {
        AddQueriesStreamSingleThread(query_input, search_results_output);
    }));*/
    futures.push_back(async(launch::async, &SearchServer::AddQueriesStreamSingleThread,
                            this, ref(query_input), ref(search_results_output)));
}

void InvertedIndex::Add(string document) {
    /* docs.push_back(move(document));

     const size_t docid = docs.size() - 1;
     for (string_view word : SplitIntoWords(docs.back())) {
         index[word].push_back(docid);
     }*/
}

vector<pair<size_t, size_t>> InvertedIndex::Lookup(string_view word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return {};
    }
}
