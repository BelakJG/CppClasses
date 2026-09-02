#include <iostream>
#include <chrono>
#include <format>
#include <random>
#include <cstdint>

#include "ThreadPool.hpp"
#include "RuntimeArray.hpp"

using namespace std;

bool is_sorted(auto& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

void parallel_sort(auto& arr, auto& pool, size_t left, size_t right) {
    while(left < right) {
        if (right - left < 50000) {
            sort(arr.begin() + left, arr.begin() + right + 1);
            break;
        }

        size_t mid = ((right - left) / 2) + left;
        if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
        if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
        if (arr[left] > arr[right]) swap(arr[left], arr[right]);
        auto pivot = arr[mid];

        size_t i = left;
        size_t j = right;
        while (true) {
            while (arr[i] < pivot) ++i;
            while (arr[j] > pivot) --j;
            if (i >= j) break;

            swap(arr[i], arr[j]);
            ++i;
            --j;
        }

        pool.enqueue([&arr, &pool, left, j]() {parallel_sort(arr, pool, left, j);});
        left = j + 1;
    }
}

int main() {
    size_t num_workers = 500;
    ThreadPool pool(num_workers);

    size_t num_to_sort = 10000000;
    RuntimeArray<int> arr_multi(num_to_sort);

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> distrib(0, num_to_sort * 10);

    for (size_t i = 0; i < num_to_sort; ++i) {
        arr_multi[i] = distrib(gen);
    }
    auto arr_single = arr_multi;

    auto start = chrono::steady_clock::now();
    sort(arr_single.begin(), arr_single.end());
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << format("Sorting {} elements on one thread took {} miliseconds", num_to_sort, elapsed.count()) << endl;
    cout << (is_sorted(arr_single) ? "sorted" : "not sorted") << endl;

    cout << endl;
    start = chrono::steady_clock::now();
    parallel_sort(arr_multi, pool, 0, num_to_sort - 1);
    pool.wait_all();
    end = chrono::steady_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << format("Sorting {} elements using {} threads took {} miliseconds", num_to_sort, pool.num_workers() + 1, elapsed.count()) << endl;
    cout << (is_sorted(arr_multi) ? "sorted" : "not sorted") << endl;
}