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
    unsigned int active_threads = 1;
    unsigned int max_threads;
    std::vector<std::thread> workers;
    std::queue<std::packaged_task<void()>> tasks;
    std::mutex queue_lock;
    bool stop = false;
    std::condition_variable cv;

    void worker_thread();
public:
    ThreadPool(unsigned int num_threads = 8);
    std::future<void> enqueue(std::function<void()> task);
    void stop_all();
};

#endif