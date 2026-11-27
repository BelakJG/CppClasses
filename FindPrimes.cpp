#include <vector>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include "Benchmark.hpp"

using namespace std;

void find_primes(auto& container, size_t max_search) {
    for (size_t i = 3; i * i <= max_search; i += 2) {
        if (container[i]) {
            for (size_t j = i * i; j < max_search; j += i * 2) {
                container[j] = 0;
            }
        }
    }
}

int main() {
    const size_t max_search = 1000000;
    vector<uint8_t> primes(max_search, 0);
    
    primes[2] = 1;
    for (size_t i = 3; i < max_search; i += 2) {
        primes[i] = 1;
    }

    try {
        benchmark([&primes, max_search]() {
            find_primes(primes, max_search);

            size_t count = 1;
            for (size_t i = 3; i < max_search; i += 2) {
                count += primes[i];
            }
            cout << count << " Primes between 0 and " << max_search << endl;
        });
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}