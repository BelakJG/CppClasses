#ifndef THREADPOOL_CPP
#define THREADPOOL_CPP

#include "ThreadPool.hpp"

void ThreadPool::worker_thread(size_t id) {
    std::unique_lock lock(queue_lock);
    while (true) {
        cv.wait(lock, [this]{ return stop or !tasks.empty() or total_workers > max_threads;});
        {
            std::lock_guard shrink(shrink_lock);
            if (total_workers > max_threads) {
                --total_workers;
                for (auto& w : workers) {
                    if (w.id == id) {
                        w.exited = true;
                        break;
                    }
                }
                if (total_workers == max_threads) {
                    wait_cv.notify_all();
                }
                break;
            }
        }
        if (stop && tasks.empty()) break;
        if (!stop && tasks.empty()) continue;

        auto task = std::move(tasks.front());
        tasks.pop();
        ++running_tasks;
        lock.unlock();

        task();
        lock.lock();
        --running_tasks;
        if (running_tasks == 0 and tasks.empty()) wait_cv.notify_all();
    }
}

ThreadPool::ThreadPool(unsigned int num_threads) {
    max_threads = std::min(num_threads, std::thread::hardware_concurrency() - 1);

    for (int i = 0; i < max_threads; ++i) {
        workers.emplace_back(this, next_id.fetch_add(1));
        ++total_workers;
    }
}

ThreadPool::~ThreadPool() {
    stop_all();
}

void ThreadPool::resize(unsigned int num_threads) {
    std::unique_lock lock(shrink_lock);
    if (num_threads == max_threads) return;
    max_threads = std::min(num_threads, std::thread::hardware_concurrency() - 1);
    if (total_workers < max_threads) {
        while (workers.size() < max_threads) {
            workers.emplace_back(this, next_id.fetch_add(1));
            ++total_workers;
        }
    } else {
        cv.notify_all();
        wait_cv.wait(lock, [this]{return total_workers == max_threads;});
        std::erase_if(workers, [](Worker& w) {
            if (w.exited) {
                if (w.thread.joinable()) {
                    w.thread.join();
                }
                return true;
            }
            return false;
        });
    }
}

void ThreadPool::wait_all() {
    std::unique_lock lock(queue_lock);
    wait_cv.wait(lock, [this]{return tasks.empty() and running_tasks == 0;});
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

    for (auto& w : workers) {
        if (w.thread.joinable()) {
            w.thread.join();
        }
    }
}

#endif