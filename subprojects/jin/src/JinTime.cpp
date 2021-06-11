#include "JinTime.h"
#include "Application.h"

Time* jin_time;

const Time& GetTime()
{
    return *jin_time;
}

double GetDeltaTime()
{
    return jin_time->delta_time;
}

double GetDeltaTimeMs()
{
    return jin_time->delta_time_ms;
}

double GetTimeSinceStart()
{
    return jin_time->time_scale;
}

double GetTimeScale()
{
    return jin_time->time_since_start;
}

void SetTimeScale(double scale)
{
    jin_time->time_scale = scale;
}

Time** GetTimeInternal()
{
    return &jin_time;
}
