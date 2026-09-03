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
    struct Worker {
        std::thread thread;
        size_t id;
        bool exited = false;

        Worker(ThreadPool* pool, size_t id) : id(id) {
            thread = std::thread(&ThreadPool::worker_thread, pool, id);
        }
    };

    size_t max_threads;
    std::vector<Worker> workers;
    size_t total_workers = 0;
    std::queue<std::move_only_function<void()>> tasks;
    std::mutex queue_lock;
    std::mutex shrink_lock;
    bool stop = false;
    std::condition_variable cv;
    std::condition_variable wait_cv;
    size_t running_tasks = 0;
    std::atomic<size_t> next_id = 1;

    void worker_thread(size_t id);
public:
    ThreadPool(unsigned int num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args);
    void resize(unsigned int num_threads);

    const int tasks_left() const {return tasks.size();};
    const int num_workers() const {return total_workers;};

    void wait_all();
    void stop_all(bool remove_tasks = false);
};

#include "ThreadPool.tpp"

#endif