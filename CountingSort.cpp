#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include "RuntimeArray.hpp"

using namespace std;

RuntimeArray<unsigned int> CountingSort(const auto& array) {
    unsigned int max = 0;
    for (unsigned int num : array) {
        if (num > max) max = num;
    }

    RuntimeArray<unsigned int> sorted(array.size());
    RuntimeArray<unsigned int> count(max + 1);

    for (auto num : array) {
        count[num] += 1;
    }

    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    for (size_t i = array.size() - 1; i >= 0; --i) {
        auto num = array[i];
        sorted[count[num] - 1] = num;
        count[num] -= 1;

        if (i == 0) break;
    }

    return sorted;
}

void is_sorted(const auto& array) {
    for (size_t i = 0; i < array.size() - 1; ++i) {
        if (array[i] > array[i + 1]) {
            cout << "array is not sorted!" << endl;
            return;
        }
    }
    cout << "array is sorted!" << endl;
}

int main() {
    size_t array_size = 1000000;
    RuntimeArray<unsigned int> array(array_size);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<unsigned int> dist(0, array_size * 5);
    
    for (size_t i = 0; i < array.size(); ++i) {
        array[i] = dist(rng);
    }

    auto start = chrono::steady_clock::now();
    auto sorted = CountingSort(array);
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    is_sorted(sorted);
    cout << "Sorting took: " << duration.count() << " ms";
}
