#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include "RuntimeArray.hpp"
#include "InsertionSort.cpp"

using namespace std;

void shell_sort(auto& arr, size_t left = 0, size_t right = SIZE_MAX) {
    if (arr.size() <= 1 || right - left <= 1) return;
    if (right == SIZE_MAX) right = arr.size() - 1;

    size_t gap = (right - left + 1) / 2;
    while (gap >= 1) {
        for (size_t i = left + gap; i <= right; ++i) {
            size_t j = i;
            auto key = arr[i];
            while (j >= left + gap && arr[j - gap] > key) {
                arr[j] = arr[j - gap];
                if (j < gap) break;
                j -= gap;
            }
            arr[j] = key;
        }
        gap /= 2;
    }
}

bool is_sorted(const auto& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

int main() {
    size_t size_arr = 1000000;
    RuntimeArray<int> arr(size_arr);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, size_arr * 5);

    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = dist(rng);
    }

    auto start = chrono::steady_clock::now();
    shell_sort(arr);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << (is_sorted(arr) ? "Sorted!" : "Not Sorted!") << endl;
    cout << "Shell sort took " << duration.count() << " ms" << endl;
}
