#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <vector>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
private:
    unsigned int max_threads;
    std::vector<std::thread> workers;
    std::queue<std::packaged_task<void()>> tasks;
    std::mutex queue_lock;
    bool stop = false;
    std::condition_variable cv;

    void worker_thread();
public:
    ThreadPool(unsigned int num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();
    std::future<void> enqueue(std::function<void()> task);
    void stop_all(bool remove_tasks = false);
};

#endif