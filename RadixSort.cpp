#include <iostream>
#include <random>
#include <chrono>
#include <cstdint>
#include "RuntimeArray.hpp"
#include "Benchmark.hpp"

using namespace std;

void radix_sort(auto& arr) {
    if (arr.size() <= 1) return;
    RuntimeArray<size_t> counts(256);
    RuntimeArray<int> temp(arr.size());

    uint64_t num_shift_size = sizeof(arr[0]) * 8;

    for (int shift = 0; shift < num_shift_size; shift += 8) {
        for (int num : arr) {
            uint8_t byte = (num >> shift) & 0xFF;
            if ((shift == 24 && num_shift_size == 32) || (shift == 56 && num_shift_size == 64)) byte ^= 0x80;
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
    const size_t arr_size = 10000000;
    RuntimeArray<int> arr(arr_size);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(-(arr_size * 5), arr_size * 5);

    for (size_t i = 0; i < arr_size; ++i) {
        arr[i] = dist(rng);
    }

    benchmark([&]() {radix_sort(arr);});

    cout << "array is " << (is_sorted(arr) ? "Sorted!" : "Not Sorted!") << endl;
}
