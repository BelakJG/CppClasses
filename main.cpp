#include <iostream>
#include "ThreadPool.hpp"

using namespace std;

int main() {
    ThreadPool tp(8);

    auto t = tp.enqueue([]() {cout << "done" << endl;});
    auto num = tp.enqueue([](int a, double b) { return a * b; }, 2, 25);
    t.get();
    cout << num.get() << endl;
}