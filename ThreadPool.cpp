#ifndef THREADPOOL_CPP
#define THREADPOOL_CPP

#include "ThreadPool.hpp"
#include <iostream>

void ThreadPool::worker_thread() {
    while (!stop) {
        std::unique_lock lock(queue_lock);
        cv.wait(lock, [this]{ return stop or !tasks.empty(); });
        if (stop && tasks.empty()) break;

        auto task = std::move(tasks.front());
        tasks.pop();

        task();
    }

    std::cout << "thread stopped" << std::endl;
}

ThreadPool::ThreadPool(unsigned int num_threads) {
    max_threads = std::min(num_threads, std::thread::hardware_concurrency());

    for (int i = 1; i < max_threads; ++i) {
        workers.emplace_back(std::thread(&ThreadPool::worker_thread, this));
        ++active_threads;
    }
}

std::future<void> ThreadPool::enqueue(std::function<void()> task) {
    std::packaged_task<void()> pt(task);
    auto fut = pt.get_future();
    {
        std::lock_guard lock(queue_lock);
        tasks.push(std::move(pt));
    }

    cv.notify_one();
    return fut;
}

void ThreadPool::stop_all() {
    {
        std::lock_guard lock(queue_lock);
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