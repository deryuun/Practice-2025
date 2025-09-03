#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class ConcurrentPriorityQueue {
public:
    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lk(mu_);
            q_.push(item);
        }
        cv_.notify_one();
    }

    template <class Pred>
    std::optional<T> wait_and_pop(Pred should_stop) {
        std::unique_lock<std::mutex> lk(mu_);
        cv_.wait(lk, [&]{ return !q_.empty() || should_stop(); });
        if (q_.empty()) return std::nullopt;
        T out = q_.top();
        q_.pop();
        return out;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mu_);
        return q_.empty();
    }

    void notify_all() { cv_.notify_all(); }

private:
    mutable std::mutex mu_;
    std::condition_variable cv_;
    std::priority_queue<T> q_;
};

