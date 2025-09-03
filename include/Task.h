#pragma once
#include <string>
#include <functional>
#include <chrono>

struct Task {
    int id{};
    std::string name;
    int priority{0};
    std::chrono::steady_clock::time_point deadline{};
    std::function<void()> callback;

    inline std::string Short() const {
        return "id=" + std::to_string(id) + ", name=\"" + name + "\", p=" + std::to_string(priority);
    }

    bool operator<(const Task& other) const noexcept {
        if (priority != other.priority) return priority < other.priority;
        return deadline > other.deadline;
    }
};


