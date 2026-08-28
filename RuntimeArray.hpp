#ifndef RUNTIME_ARRAY_HPP
#define RUNTIME_ARRAY_HPP

#include <cstddef>

template <typename T>
class RuntimeArray{
private:
    size_t array_size;
    T* arr_data;
public:
    RuntimeArray(size_t s = 0);
    ~RuntimeArray();
    RuntimeArray(const RuntimeArray& other);
    RuntimeArray& operator=(const RuntimeArray& other);
    RuntimeArray(RuntimeArray&& other) noexcept;
    RuntimeArray& operator=(RuntimeArray&& other) noexcept;

    size_t size() {return array_size;}
    bool empty() {return array_size == 0;}

    T& operator[](size_t index) {return arr_data[index];}
    const T& operator[](size_t index) const {return arr_data[index];}

    T& at(size_t index);
    const T& at(size_t index) const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T* data() {return arr_data;}

    void fill(const T& value);
};

#include "RuntimeArray.tpp"

#endif