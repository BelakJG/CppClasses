#include <iostream>
#include <random>
#include <chrono>
#include "RuntimeArray.hpp"
#include "InsertionSort.cpp"

using namespace std;

RuntimeArray<int> merge(auto left, auto right) {
    RuntimeArray<int> merged(left.size() + right.size());

    size_t merged_index = 0;
    size_t left_index = 0;
    size_t right_index = 0;
    while (left_index < left.size() && right_index < right.size()) {
        if (left[left_index] < right[right_index]) {
            merged[merged_index] = left[left_index];
            ++left_index;
        } else {
            merged[merged_index] = right[right_index];
            ++right_index;
        }
        ++merged_index;
    }

    while(left_index < left.size()) {
        merged[merged_index] = left[left_index];
        ++left_index;
        ++merged_index;
    }
    while(right_index < right.size()) {
        merged[merged_index] = right[right_index];
        ++right_index;
        ++merged_index;
    }

    return merged;
}

RuntimeArray<int> merge_sort(auto& array) {
    if (array.size() <= 1) return array;
    if (array.size() <= 64) {
        insertion_sort(array, 0, array.size() - 1);
        return array;
    }

    size_t mid = array.size() / 2;

    RuntimeArray<int> left(mid);
    for (int l = 0; l < mid; ++l) {
        left[l] = array[l];
    }

    RuntimeArray<int> right(array.size() - mid);
    for (int r = mid; r < array.size(); ++r) {
        right[r - mid] = array[r];
    }

    return merge(merge_sort(left), merge_sort(right));
}

bool is_sorted(auto& array) {
    for (int i = 0; i < array.size() - 1; ++i) {
        if (array[i] > array[i + 1]) return false;
    }
    return true;
}

int main() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, 5000000);

    size_t array_size = 1000000;
    RuntimeArray<int> unsorted_array(array_size);
    for (int i = 0; i < array_size; ++i) {
        unsorted_array[i] = dist(rng);
    }

    auto start = chrono::steady_clock::now();
    RuntimeArray<int> sorted = merge_sort(unsorted_array);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Sorting took: " << duration.count() << "ms" << endl;

    cout << "Array is " << ((is_sorted(sorted) && sorted.size() == unsorted_array.size())? "Sorted!" : "Not Sorted!") << endl;
}