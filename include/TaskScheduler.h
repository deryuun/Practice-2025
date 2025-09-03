#pragma once
#include <atomic>
#include <fstream>
#include <memory>
#include <iomanip>
#include "ConcurrentPriorityQueue.h"
#include "ThreadPool.h"
#include "Task.h"

class TaskScheduler {
public:
    explicit TaskScheduler(int threads, const std::string& log_path = "tasks.log");
    ~TaskScheduler();

    void add(Task task);
    void stop(bool graceful = true);

private:
    void log(const std::string& msg);

    std::atomic_bool stop_{false};
    ConcurrentPriorityQueue<Task> queue_;
    std::unique_ptr<ThreadPool> pool_;
    std::ofstream log_;
};

