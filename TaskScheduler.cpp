#include "include/TaskScheduler.h"
#include <chrono>
#include <ctime>
#include <iostream>

TaskScheduler::TaskScheduler(int threads, const std::string& log_path) {
    log_.open(log_path, std::ios::out | std::ios::trunc);
    pool_ = std::make_unique<ThreadPool>(threads, queue_, stop_,
                                         [this](const std::string& s){ log(s); });
    log("TaskScheduler started with " + std::to_string(threads) + " threads");
}

TaskScheduler::~TaskScheduler() {
    stop(true);
    if (log_.is_open()) {
        log("TaskScheduler stopped");
        log_.close();
    }
}

void TaskScheduler::add(Task task) {
    if (stop_.load()) return;
    queue_.push(task);
}

void TaskScheduler::stop(bool graceful) {
    if (stop_.exchange(true)) return;
    if (!graceful) {
        queue_.notify_all();
    } else {
        queue_.notify_all();
    }
    if (pool_) pool_->join();
}

void TaskScheduler::log(const std::string& msg) {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream line;
    line << "[" << std::put_time(std::localtime(&t), "%F %T") << "] " << msg;

    if (log_.is_open()) {
        log_ << line.str() << "\n";
        log_.flush();
    }

    std::cout << line.str() << std::endl;
}
