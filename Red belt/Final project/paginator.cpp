#include "test_runner.h"
#include "iterator_range.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Реализуйте шаблон класса Paginator

template<typename Iterator>
class Paginator {
    size_t pf_page_size;
    vector<IteratorRange<Iterator>> pf_pages;

public:
    Paginator(const Iterator &begin, const Iterator &end, size_t page_size) : pf_page_size(page_size) {
        Iterator current = begin;

        while (current + page_size < end) {
            pf_pages.push_back(IteratorRange(current, current + page_size));
            current += page_size;
        }
        if (current != end)
            pf_pages.push_back(IteratorRange(current, end));
    }

    auto begin() const {
        return pf_pages.begin();
    }

    auto end() const {
        return pf_pages.end();
    }

    size_t size() const {
        return pf_pages.end() - pf_pages.begin();
    }
};


template<typename C>
auto Paginate(C &c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}