#include <iostream>
#include "ThreadPool.hpp"

using namespace std;

double find_root(double num) {
    double prev = num / 4;
    while (true) {
        double next = num / prev;
        prev = (prev + next) / 2;
        if (prev - next <= .0000001) break;
    }
    return prev;
}

int main() {
    ThreadPool tp(8);

    auto t = tp.enqueue([]() {cout << "done" << endl;});
    auto num = tp.enqueue([](int a, double b) { return a * b; }, 2, 25);
    auto root = tp.enqueue(find_root, 37.192);
    t.get();
    cout << num.get() << endl;
    cout << root.get() << endl;
}