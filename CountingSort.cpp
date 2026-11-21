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
