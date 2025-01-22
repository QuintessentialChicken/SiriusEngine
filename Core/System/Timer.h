//
// Created by Leon on 21/01/2025.
//

#ifndef TIMER_H
#define TIMER_H
#include <chrono>


class Timer {
public:
    Timer();

    float Mark();

    float Peek() const;

private:
    std::chrono::steady_clock::time_point last;
};


#endif //TIMER_H
