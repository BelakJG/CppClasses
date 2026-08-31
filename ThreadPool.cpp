#ifndef THREADPOOL_CPP
#define THREADPOOL_CPP

#include "ThreadPool.hpp"
#include <iostream>

void ThreadPool::worker_thread() {
    while (true) {
        std::unique_lock lock(queue_lock);
        cv.wait(lock, [this]{ return stop or !tasks.empty(); });
        if (stop && tasks.empty()) break;

        auto task = std::move(tasks.front());
        tasks.pop();
        lock.unlock();

        task();
    }

    std::lock_guard lock(queue_lock);
    std::cout << "thread " << std::this_thread::get_id() << " stopped" << std::endl;
}

ThreadPool::ThreadPool(unsigned int num_threads) {
    max_threads = std::min(num_threads, std::thread::hardware_concurrency());

    for (int i = 0; i < max_threads; ++i) {
        workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool() {
    stop_all();
}

void ThreadPool::stop_all(bool remove_tasks) {
    {
        std::lock_guard lock(queue_lock);
        if (remove_tasks) {
            tasks = {};
        }
        stop = true;
    }
    cv.notify_all();

    for (auto& t : workers) {
        if (t.joinable()) {
            t.join();
        }
    }
}

#endif