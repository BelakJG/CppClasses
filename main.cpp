#include <iostream>
#include "RuntimeArray.hpp"

using namespace std;

int main() {
    RuntimeArray<RuntimeArray<int>> arr(5);
    for (int i = 0; i < 5; ++i) {
        arr[i] = RuntimeArray<int>(8);
        arr[i].fill(i);
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}