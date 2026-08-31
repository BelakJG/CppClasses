#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <vector>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <type_traits>

class ThreadPool {
private:
    unsigned int max_threads;
    std::vector<std::thread> workers;
    std::queue<std::move_only_function<void()>> tasks;
    std::mutex queue_lock;
    bool stop = false;
    std::condition_variable cv;

    void worker_thread();
public:
    ThreadPool(unsigned int num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args);

    void stop_all(bool remove_tasks = false);
};

#include "ThreadPool.tpp"

#endif