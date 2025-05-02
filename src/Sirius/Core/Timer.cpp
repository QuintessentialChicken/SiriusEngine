//
// Created by Leon on 21/01/2025.
//

#include "Timer.h"


Timer::Timer() {
    last = std::chrono::steady_clock::now();
    start = last;
}

float Timer::Mark() {
    const auto old = last;
    last = std::chrono::steady_clock::now();
    const std::chrono::duration<float> duration = last - old;
    return duration.count();
}

float Timer::Peek() const {
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}

float Timer::Total() const {
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
}
