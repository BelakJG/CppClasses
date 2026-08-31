#include <iostream>
#include "ThreadPool.hpp"

using namespace std;

int main() {
    ThreadPool tp(8);

    auto t = tp.enqueue([]() {cout << "done" << endl;});
    t.get();
    tp.stop_all();
}