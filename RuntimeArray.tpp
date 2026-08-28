#ifndef RUNTIME_ARRAY_TPP
#define RUNTIME_ARRAY_TPP

#include <stdexcept>

template <typename T>
RuntimeArray<T>::RuntimeArray(size_t s) : array_size(s), arr_data(new T[s]()) {}

template <typename T>
RuntimeArray<T>::~RuntimeArray() {
    delete[] arr_data;
    arr_data = nullptr;
}

template <typename T>
RuntimeArray<T>::RuntimeArray(const RuntimeArray<T>& other) : array_size(other.array_size), arr_data(new T[other.array_size]()) {
    for (size_t i = 0; i < array_size; ++i) {
        arr_data[i] = other.arr_data[i];
    }
}

template <typename T>
RuntimeArray<T>& RuntimeArray<T>::operator=(const RuntimeArray<T>& other) {
    if (this != &other) {
        T* new_data = new T[other.array_size];
        for (size_t i = 0; i < other.array_size; ++i) {
            new_data[i] = other.arr_data[i];
        }

        delete[] arr_data;

        array_size = other.array_size;
        arr_data = new_data;
    }
    return *this;
}

template <typename T>
RuntimeArray<T>::RuntimeArray(RuntimeArray<T>&& other) noexcept : array_size(other.array_size), arr_data(other.arr_data) {
    other.array_size = 0;
    other.arr_data = nullptr;
}

template <typename T>
RuntimeArray<T>& RuntimeArray<T>::operator=(RuntimeArray&& other) noexcept {
    if (this != &other) {
        delete[] arr_data;
        
        arr_data = other.arr_data;
        array_size = other.array_size;

        other.arr_data = nullptr;
        other.array_size = 0;
    }

    return *this;
}

template <typename T>
T& RuntimeArray<T>::at(size_t index) {
    if (index >= array_size) {
        throw std::out_of_range("Index out of range");
    }

    return arr_data[index];
}

template <typename T>
const T& RuntimeArray<T>::at(size_t index) const {
    if (index >= array_size) {
        throw std::out_of_range("index out of range");
    }

    return arr_data[index];
}

template <typename T>
T& RuntimeArray<T>::front() {
    if (array_size == 0) {
        throw std::runtime_error("Can't access front of empty array");
    }
    return arr_data[0];
}

template <typename T>
const T& RuntimeArray<T>::front() const {
    if (array_size == 0) {
        throw std::runtime_error("Can't access front of empty array");
    }
    return arr_data[0];
}

template <typename T>
T& RuntimeArray<T>::back() {
    if (array_size == 0) {
        throw std::runtime_error("Can't access back of empty array");
    }
    return arr_data[array_size - 1];
}

template <typename T>
const T& RuntimeArray<T>::back() const {
    if (array_size == 0) {
        throw std::runtime_error("Can't access back of empty array");
    }
    return arr_data[array_size - 1];
}

template <typename T>
void RuntimeArray<T>::fill(const T& value) {
    if (array_size == 0) {
        throw std::runtime_error("Can't fill an empty array");
    }
    for (size_t i = 0; i < array_size; ++i) {
        arr_data[i] = value;
    }
}

#endif