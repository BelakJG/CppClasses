#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include "RuntimeArray.hpp"

using namespace std;

void radix_sort(auto& arr) {
    if (arr.size() <= 1) return;
    RuntimeArray<size_t> counts(256);
    RuntimeArray<int> temp(arr.size());

    for (int shift = 0; shift < 32; shift += 8) {
        for (int num : arr) {
            uint8_t byte = (num >> shift) & 0xFF;
            if (shift == 24) byte ^= 0x80;
            counts[byte] += 1;
        }

        size_t offset = 0;
        for (int i = 0; i < 256; ++i) {
            size_t current = counts[i];
            counts[i] = offset;
            offset += current;
        }

        for (size_t i = 0; i < arr.size(); ++i) {
            uint8_t byte = (arr[i] >> shift) & 0xFF;
            if (shift == 24) byte ^= 0x80;
            temp[counts[byte]++] = arr[i];
        }

        swap(arr, temp);
        counts.fill(0);
    }
}

bool is_sorted(const auto& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

int main() {
    const size_t arr_size = 1000000;
    RuntimeArray<int> arr(arr_size);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(-(arr_size * 5), arr_size * 5);

    for (size_t i = 0; i < arr_size; ++i) {
        arr[i] = dist(rng);
    }

    auto start = chrono::steady_clock::now();
    radix_sort(arr);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Sorting took " << duration.count() << " ms!" << endl;
    cout << "array is " << (is_sorted(arr) ? "Sorted!" : "Not Sorted!") << endl;
}
