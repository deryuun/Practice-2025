#include "include/ThreadPool.h"
#include <chrono>
#include <sstream>

ThreadPool::ThreadPool(int threads,
                       ConcurrentPriorityQueue<Task>& queue,
                       std::atomic_bool& stop_flag,
                       Logger logger)
    : queue_(queue), stop_(stop_flag), log_(std::move(logger)) {
    workers_.reserve(threads);
    for (int i = 0; i < threads; ++i) {
        workers_.emplace_back([this, i]{ worker_loop(i); });
    }
}

ThreadPool::~ThreadPool() {

}

void ThreadPool::join() {
    for (auto& t : workers_) {
        if (t.joinable()) t.join();
    }
}

void ThreadPool::worker_loop(int idx) {
    auto should_stop = [this] { return stop_.load(std::memory_order_acquire); };
    while (!should_stop()) {
        auto next = queue_.wait_and_pop(should_stop);
        if (!next.has_value()) break;

        const auto& task = *next;
        auto now = std::chrono::steady_clock::now();
        auto ms_left = std::chrono::duration_cast<std::chrono::milliseconds>(task.deadline - now).count();

        if (task.deadline != std::chrono::steady_clock::time_point{} && now > task.deadline) {
            if (log_) log_(
                        "[thr=" + std::to_string(idx) + "] "
                                                        "SKIP expired: " + task.Short()
                );
            continue;
        }

        if (log_) log_(
                    "[thr=" + std::to_string(idx) + "] "
                                                    "RUN " + task.Short() + ", deadline_ms_left=" + std::to_string(ms_left)
            );

        try {
            task.callback();
            if (log_) log_(
                        "[thr=" + std::to_string(idx) + "] "
                                                        "DONE " + task.Short()
                );
        } catch (const std::exception& e) {
            if (log_) log_(
                        "[thr=" + std::to_string(idx) + "] "
                                                        "ERROR " + task.Short() + ", what=" + e.what()
                );
        } catch (...) {
            if (log_) log_(
                        "[thr=" + std::to_string(idx) + "] "
                                                        "ERROR " + task.Short() + ", what=unknown"
                );
        }
    }
    if (log_) log_("[thr=" + std::to_string(idx) + "] exit worker");
}
