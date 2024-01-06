#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <memory>
#include <chrono>
#include <list>
#include <functional>
#include <thread>

using std::weak_ptr;
using std::list;
using std::function;
using std::cout;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

typedef function<int()> ActionPerformer;

class Timer
{
public:
    Timer(uint32_t delay, ActionPerformer actionPerformer);

    int performAction();
    int millisTillNextTick();

private:
    ActionPerformer actionPerformer;
    uint32_t delay;
    uint64_t nextTick;

    static uint64_t currentTimeMillis();
};

extern list<Timer *> timers;

int runAll();

#endif