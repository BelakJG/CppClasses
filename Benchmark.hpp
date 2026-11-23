#pragma once

#include <iostream>
#include <chrono>
#include <functional>
#include <type_traits>
#include <utility>

template <typename Func, typename... Args>
decltype(auto) benchmark(Func&& func, Args&&... args) {
    using ReturnType = std::invoke_result_t<Func, Args...>;

    auto start = std::chrono::steady_clock::now();
    
    if constexpr (std::is_void_v<ReturnType>) {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Benchmark: Timed function took " << duration.count() << " ms" << std::endl;
    } else {
        auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Benchmark: Timed function took " << duration.count() << " ms" << std::endl;
        return result;
    }
}