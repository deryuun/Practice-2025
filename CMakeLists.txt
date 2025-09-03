cmake_minimum_required(VERSION 3.28)
project(taskScheduler)

set(CMAKE_CXX_STANDARD 17)

add_executable(taskScheduler main.cpp
        include/ConcurrentPriorityQueue.h
        include/Task.h
        include/TaskScheduler.h
        include/ThreadPool.h
        ConcurrentPriorityQueue.cpp
        TaskScheduler.cpp
        ThreadPool.cpp)
