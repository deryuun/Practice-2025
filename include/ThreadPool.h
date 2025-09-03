#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include "ConcurrentPriorityQueue.h"
#include "Task.h"

class ThreadPool {
public:
    using Logger = std::function<void(const std::string&)>;

    ThreadPool(int threads,
               ConcurrentPriorityQueue<Task>& queue,
               std::atomic_bool& stop_flag,
               Logger logger);

    ~ThreadPool();

    void join();

private:
    void worker_loop(int idx);

    ConcurrentPriorityQueue<Task>& queue_;
    std::vector<std::thread> workers_;
    std::atomic_bool& stop_;
    Logger log_;
};

