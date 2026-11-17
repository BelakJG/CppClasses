void insertion_sort(auto& array, size_t left, size_t right) {
    if (left >= right) return;
    
    for (size_t i = left + 1; i <= right; ++i) {
        auto key = array[i];
        size_t j = i;
        while (j > left && array[j - 1] > key) {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = key;
    }
}