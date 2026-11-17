#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include "RuntimeArray.hpp"
#include "InsertionSort.cpp"

using namespace std;

void insertion_swipe(auto& array, size_t start, size_t end) {
    if (start >= end) return;

    auto key = array[end];
    size_t j = end;
    while (j > start && array[j - 1] > key) {
        array[j] = array[j - 1];
        --j;
    }
    array[j] = key;
}

void merge(auto& array, size_t l_start, size_t l_end, size_t r_start, size_t r_end) {
    for (size_t j = r_end; j > l_end; --j) {
        if (array[j] < array[l_end]) {
            swap(array[j], array[l_end]);
            insertion_swipe(array, l_start, l_end);
        }
    }
}

void sort(auto& array) {
    if (array.size() <= 1) return;
    //rough sort
    size_t rough_start = 0;
    while (rough_start < array.size() - 1) {
        insertion_sort(array, rough_start, min(rough_start + 63, array.size() - 1));

        rough_start += 64;
    }
    if (array.size() <= 64) return;

    size_t buffer_size = 64;
    while (buffer_size < array.size()) {
        size_t l_start = 0;
        while (l_start < array.size() - 1) {
            size_t l_end = l_start + buffer_size - 1;
            size_t r_start = l_end + 1;
            size_t r_end = min((2 * buffer_size) + l_start - 1, array.size() - 1);

            merge(array, l_start, l_end, r_start, r_end);

            l_start += (2 * buffer_size);
        }

        buffer_size *= 2;
    }
}

bool is_sorted(const auto& array) {
    for (size_t i = 0; i < array.size() - 1; ++i) {
        if (array[i] > array[i + 1]) return false;
    }
    return true;
}

int main() {
    size_t size_array = 100000;
    RuntimeArray<int> array(size_array);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, size_array * 10);
    for (size_t i = 0; i < size_array; ++i) {
        array[i] = dist(rng);
    }

    auto start = chrono::steady_clock::now();
    sort(array);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Array is " << (is_sorted(array) ? "Sorted!" : "Not Sorted!") << endl;
    cout << "Sorting took: " << duration.count() << " ms" << endl;
}
