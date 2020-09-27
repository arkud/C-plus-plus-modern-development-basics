#include <vector>

template<typename T>
class Deque {
    std::vector<T> front;
    std::vector<T> back;
public:

    Deque() {

    }

    bool Empty() const {
        return front.empty() && back.empty();
    }

    size_t Size() const {
        return front.size() + back.size();
    }

    T &operator[](size_t index) {
        return (index + 1 > front.size()) ?
               back[index - front.size()] : front[front.size() - index - 1];
    }

    const T &operator[](size_t index) const {
        return (index + 1 > front.size()) ?
               back[index - front.size()] : front[front.size() - index - 1];
    }

    T &At(size_t index) {
        return (index + 1 > front.size()) ?
               back.at(index - front.size()) : front.at(front.size() - index - 1);
    }

    const T &At(size_t index) const {
        return (index + 1 > front.size()) ?
               back.at(index - front.size()) : front.at(front.size() - index - 1);
    }

    T &Front() {
        return (!front.empty()) ? front.back() : back.front();
    }

    const T &Front() const {
        return (!front.empty()) ? front.back() : back.front();
    }

    T &Back() {
        return (!back.empty()) ? back.back() : front.front();
    }

    const T &Back() const {
        return (!back.empty()) ? back.back() : front.front();
    }

    void PushFront(const T &to_insert) {
        front.push_back(to_insert);
    }

    void PushBack(const T &to_insert) {
        back.push_back(to_insert);
    }

};
