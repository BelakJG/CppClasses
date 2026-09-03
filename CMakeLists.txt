cmake_minimum_required(VERSION 3.10.0)
project(CppClasses VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(pool_bench pool_benchmark.cpp ThreadPool.cpp)
