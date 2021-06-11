#pragma once

struct Time
{
    double time_since_start = 0;
    double delta_time = 0;
    double delta_time_ms = 0;
    double time_scale = 1;
};

const Time& GetTime();
double GetDeltaTime();
double GetDeltaTimeMs();
double GetTimeSinceStart();
double GetTimeScale();
void SetTimeScale(double scale);

//Internal Function
Time** GetTimeInternal();