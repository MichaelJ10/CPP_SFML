#ifndef STOP_WATCH
#define STOP_WATCH
#include <string>
#include <list>
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <memory>

using std::cout;
using std::list;
using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;

class StopWatch
{
public:
    StopWatch();
    StopWatch(string name);

    void start();
    void stop();
    void reset();

    uint32_t getTotal();
    uint32_t getAverage();

    string getName();

    void printTotals();
    void printAverages();

private:
    uint16_t numStarts;
    uint32_t total;
    uint64_t lastStart;
    bool hasStarted;
    string name;

    static uint64_t currentTimeMillis();
};

#endif