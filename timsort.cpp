#include <iostream>
#include <random>
#include <queue>
#include "RuntimeArray.hpp"
#include "binaryInsertion.cpp"
#include "Benchmark.hpp"

using namespace std;

namespace {
    size_t find_run_end(auto& arr, size_t run_start) {
        size_t run_end = run_start;
        while (run_end < arr.size() - 1) {
            if (arr[run_end] < arr[run_start]) break;
            ++run_end;
        }
        return run_end;
    }

    struct Run {
        size_t start;
        size_t end;
    };

    void merge(auto& arr, auto& aux, Run left, Run right) {
        size_t aux_index = left.start;
        size_t i = left.start;
        size_t j = right.start;

        while (i <= left.end && j <= right.end) {
            if (arr[i] <= arr[j]) {
                aux[aux_index] = arr[i];
                ++i;
            } else {
                aux[aux_index] = arr[j];
                ++j;
            }
            ++aux_index;
        }

        while (i <= left.end) {
            aux[aux_index] = arr[i];
            ++i;
            ++aux_index;
        }
        while (j <= right.end) {
            aux[aux_index] = arr[j];
            ++j;
            ++aux_index;
        }

        for (i = left.start; i <= right.end; ++i) {
            arr[i] = aux[i];
        }
    }
}

void timsort(auto& arr) {
    if (arr.size() <= 1) return;
    if (arr.size() <= 64) {
        binary_insertion_sort(arr, 0, arr.size() - 1);
        return;
    }

    RuntimeArray<int> aux = arr;
    queue<Run> runs;

    size_t min_run = 64;
    size_t start = 0;
    while (start <= arr.size() - 1) {
        size_t end = find_run_end(arr, start);
        if (end - start < min_run) {
            end = min(start + min_run - 1, arr.size() - 1);
        }
        binary_insertion_sort(arr, start, end);
        runs.emplace(start, end);
        start = end + 1;
    }

    while (runs.size() > 1) {
        auto left = runs.front();
        runs.pop();
        if (left.end == arr.size() - 1) {
            runs.push(left);
            left = runs.front();
            runs.pop();
        }
        auto right = runs.front();
        runs.pop();

        merge(arr, aux, left, right);
        runs.emplace(left.start, right.end);
    }
}

bool is_sorted(auto& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

int main() {
    size_t size_arr = 500000;
    RuntimeArray<int> arr(size_arr);

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, size_arr * 5);

    for (size_t i = 0; i < size_arr; ++i) {
        arr[i] = dist(rng);
    }

    benchmark([&]() {timsort(arr);});

    cout << "Array is " << (is_sorted(arr) ? "Sorted!" : "Not Sorted!") << endl;
}
