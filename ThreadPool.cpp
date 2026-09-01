#ifndef THREADPOOL_CPP
#define THREADPOOL_CPP

#include "ThreadPool.hpp"
#include <iostream>

void ThreadPool::worker_thread() {
    std::unique_lock lock(queue_lock);
    while (true) {
        cv.wait(lock, [this]{ return stop or !tasks.empty() or total_workers > max_threads;});
        {
            std::lock_guard shrink(shrink_lock);
            if (total_workers > max_threads) {
                --total_workers;
                break;
            }
        }
        if (stop && tasks.empty()) break;
        if (!stop && tasks.empty()) continue;

        auto task = std::move(tasks.front());
        tasks.pop();
        lock.unlock();

        task();
        lock.lock();
    }
    std::cout << "thread " << std::this_thread::get_id() << " stopped" << std::endl;
}

ThreadPool::ThreadPool(unsigned int num_threads) {
    max_threads = std::min(num_threads, std::thread::hardware_concurrency() - 1);

    for (int i = 0; i < max_threads; ++i) {
        workers.emplace_back(&ThreadPool::worker_thread, this);
        ++total_workers;
    }
}

ThreadPool::~ThreadPool() {
    stop_all();
}

void ThreadPool::resize(unsigned int num_threads) {
    max_threads = std::min(num_threads, std::thread::hardware_concurrency() - 1);

    while (workers.size() < max_threads) {
        workers.emplace_back(&ThreadPool::worker_thread, this);
        ++total_workers;
    }
    cv.notify_all();
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