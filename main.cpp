#include <iostream>
#include "ThreadPool.hpp"
#include <chrono>

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
    ThreadPool tp(4);
    cout << tp.num_workers() << endl;
    tp.resize(8);
    cout << tp.num_workers() << endl;
    tp.resize(6);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    cout << tp.num_workers() << endl;
    cout << "done pausing" << endl;
}