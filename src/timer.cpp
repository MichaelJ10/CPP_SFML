#include <timer.hpp>

list<Timer *> timers;

Timer::Timer(uint32_t delay, ActionPerformer actionPerformer)
{
    this->delay = delay;
    this->actionPerformer = actionPerformer;
    this->nextTick = 0;
    timers.push_back(this);
}

int Timer::performAction()
{
    if (currentTimeMillis() < nextTick)
    {
        return -2;
    }
    nextTick = (currentTimeMillis() + delay);
    int returnCode = actionPerformer();
    return returnCode; 
    
}

int Timer::millisTillNextTick() 
{
    return nextTick - currentTimeMillis();
}

uint64_t Timer::currentTimeMillis()
{
    using namespace std::chrono;

    // Get the current time point
    auto currentTime = system_clock::now();

    // Convert the time point to milliseconds since epoch
    auto duration = currentTime.time_since_epoch();
    auto millis = duration_cast<milliseconds>(duration).count();

    return millis;
}

int runAll()
{
    while (true)
    {
        for (Timer *timer : timers)
        {
            int returnCode = timer->performAction();
            if (returnCode >= 0)
            {
                return returnCode;
            }
        }
        sleep_for(milliseconds(1));
    }
    return 1;
}