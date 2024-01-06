#include <stopWatch.hpp>

StopWatch::StopWatch() : StopWatch("")
{
}

StopWatch::StopWatch(string name)
{
    total = 0;
    numStarts = 0;
    this->name = name;
    hasStarted = false;
}

void StopWatch::start()
{
    if (hasStarted)
        return;
    hasStarted = true;
    lastStart = currentTimeMillis();
}

void StopWatch::stop()
{
    if (!hasStarted)
        return;
    hasStarted = false;
    total += currentTimeMillis() - lastStart;
    numStarts++;
}

void StopWatch::reset()
{
    hasStarted = false;
    numStarts = 0;
    total = 0;
}

uint32_t StopWatch::getTotal()
{
    return total;
}

uint32_t StopWatch::getAverage()
{
    return ((numStarts == 0) ? 0 : (total / numStarts));
}

string StopWatch::getName()
{
    return name;
}

void StopWatch::printTotals()
{
    cout << getName() << ": " << getTotal() << "\n";
}

void StopWatch::printAverages()
{
    cout << getName() << ": " << getAverage() << "\n";
}

uint64_t StopWatch::currentTimeMillis()
{
    using namespace std::chrono;

    // Get the current time point
    auto currentTime = system_clock::now();

    // Convert the time point to milliseconds since epoch
    auto duration = currentTime.time_since_epoch();
    auto millis = duration_cast<milliseconds>(duration).count();

    return millis;
}