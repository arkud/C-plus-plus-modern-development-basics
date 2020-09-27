#include <utility>
#include <list>
#include <mutex>

#include "Common.h"

using namespace std;

class LruCache : public ICache {
    list<BookPtr> cache;
    size_t cache_size = 0;
    mutex m;

    Settings settings;
    shared_ptr<IBooksUnpacker> books_unpacker;
public:
    LruCache(
            shared_ptr<IBooksUnpacker> books_unpacker,
            const Settings &settings
    ) : settings(settings), books_unpacker(move(books_unpacker)) {
        // реализуйте метод
    }

    BookPtr GetBook(const string &book_name) override {
        {
            lock_guard<mutex> g(m);
            for (auto it = cache.begin(); it != cache.end(); it++) {
                if (it->get()->GetName() == book_name) {
                    cache.push_back(*it);
                    cache.erase(it);
                    return cache.back();
                }
            }
        }
        BookPtr book = books_unpacker->UnpackBook(book_name);

        if (book->GetContent().size() > settings.max_memory) {
            return book;
        } else {
            lock_guard<mutex> g(m);
            while (book->GetContent().size() > settings.max_memory - cache_size) {
                cache_size -= cache.front()->GetContent().size();
                cache.pop_front();
            }
            cache.push_back(book);
            cache_size += book->GetContent().size();
            return book;
        }
    }

};


unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings &settings
) {
    return make_unique<LruCache>(books_unpacker, settings);
}
