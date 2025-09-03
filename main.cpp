#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "include/TaskScheduler.h"

int main() {
    TaskScheduler scheduler(4);

    auto now = std::chrono::steady_clock::now();
    std::default_random_engine rng{static_cast<unsigned>(std::time(nullptr))};
    std::uniform_int_distribution<int> prio(1, 10);
    std::uniform_int_distribution<int> delay(1, 5);

    for (int i = 1; i <= 50; ++i) {
        int p = prio(rng);
        int d = delay(rng);
        Task t{
                .id = i,
                .name = "Task #" + std::to_string(i) + " (p=" + std::to_string(p) + ")",
                .priority = p,
                .deadline = now + std::chrono::seconds(d),
                .callback = [i]{
                    std::this_thread::sleep_for(std::chrono::milliseconds(30));
                    std::cout << "[task " << i << "] executed payload\n";
                }
        };
        scheduler.add(std::move(t));
    }

    std::this_thread::sleep_for(std::chrono::seconds(6));
    scheduler.stop(true);
    return 0;
}
