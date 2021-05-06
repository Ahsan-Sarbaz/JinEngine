#include "Time.h"
#include "Application.h"

Time* time;

const Time& GetTime()
{
    return *time;
}

double GetDeltaTime()
{
    return time->delta_time;
}

double GetDeltaTimeMs()
{
    return time->delta_time_ms;
}

double GetTimeSinceStart()
{
    return time->time_scale;
}

double GetTimeScale()
{
    return time->time_since_start;
}

void SetTimeScale(double scale)
{
    time->time_scale = scale;
}

Time** GetTimeInternal()
{
    return &time; 
}