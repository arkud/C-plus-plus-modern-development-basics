#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template<typename T>
class SimpleVector {
public:
    SimpleVector();

    explicit SimpleVector(size_t size);

    ~SimpleVector();

    T &operator[](size_t index);

    T *begin();

    T *end();

    size_t Size() const;

    size_t Capacity() const;

    void PushBack(const T &value);

private:
    T *pf_begin;
    T *pf_end;
    size_t pf_capacity;
    size_t pf_size;
};

template<typename T>
SimpleVector<T>::SimpleVector() : pf_begin(nullptr), pf_end(nullptr), pf_size(0), pf_capacity(0) {}

template<typename T>
SimpleVector<T>::SimpleVector(size_t size) :pf_capacity(size), pf_size(size) {
    pf_begin = new T[size];
    pf_end = pf_begin + size;
}

template<typename T>
SimpleVector<T>::~SimpleVector() {
    delete[] pf_begin;
}

template<typename T>
T *SimpleVector<T>::begin() {
    return pf_begin;
}

template<typename T>
T *SimpleVector<T>::end() {
    return pf_end;
}

template<typename T>
size_t SimpleVector<T>::Capacity() const {
    return pf_capacity;
}

template<typename T>
size_t SimpleVector<T>::Size() const {
    return pf_size;
}

template<typename T>
T &SimpleVector<T>::operator[](size_t index) {
    return *(pf_begin + index);
}

template<typename T>
void SimpleVector<T>::PushBack(const T &value) {
    pf_size++;
    if (pf_capacity == 0) {
        pf_begin = new T[1];
        pf_capacity = 1;
    }

    if (pf_size - 1 == pf_capacity) {
        T *new_begin = new T[pf_capacity * 2];
        pf_capacity *= 2;
        std::copy(pf_begin, pf_end, new_begin);
        delete[] pf_begin;

        pf_begin = new_begin;
    }

    pf_end = pf_begin + pf_size;
    *(pf_end - 1) = value;
}
