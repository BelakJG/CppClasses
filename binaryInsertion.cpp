namespace {
    size_t binary_search(auto& arr, auto value, size_t left, size_t right) {
        while (left < right) {
            size_t mid = ((right - left) / 2) + left;
            if (arr[mid] <= value) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }
}

void binary_insertion_sort(auto& arr, size_t left, size_t right) {
    if (arr.size() <= 1) return;

    for (size_t i = left + 1; i <= right; ++i) {
        auto key = arr[i];
        size_t pos = binary_search(arr, key, left, i);
        if (pos == i) continue;

        size_t j = i;
        while (j > pos) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[pos] = key;
    }
}
