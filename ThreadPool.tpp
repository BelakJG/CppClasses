#ifndef THREADPOOL_TPP
#define THREADPOOL_TPP

template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) {
    using ReturnType = std::invoke_result_t<F, Args...>;

    std::packaged_task<ReturnType()> pt(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    auto fut = pt.get_future();
    {
        std::lock_guard lock(queue_lock);
        tasks.emplace([pt = std::move(pt)]() mutable {pt();});
    }

    cv.notify_one();
    return fut;
}

#endif