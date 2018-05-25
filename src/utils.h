#pragma once
#include <time.h>

class Timer
    {
public:
    void Start();
    void Stop();
    void Clear();
    double GetDelta();
    double GetTotal();

private:
    clock_t m_totalTime = 0;
    clock_t m_deltaTime = 0;
    clock_t m_startTime = 0;
    };

void debug_printf (const char* pMsg, ...);
