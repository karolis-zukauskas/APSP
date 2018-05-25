#include "utils.h"
#include <stdarg.h>
#include <stdio.h>


void Timer::Start()
    {
    m_startTime = clock();
    }

void Timer::Stop()
    {
    clock_t endTime = clock();
    m_deltaTime = endTime - m_startTime;
    m_totalTime += m_deltaTime;
    }

void Timer::Clear()
    {
    m_totalTime = 0;
    m_deltaTime = 0;
    }

double Timer::GetDelta()
    {
    return ((double)m_deltaTime / (double)CLOCKS_PER_SEC);
    }

double Timer::GetTotal()
    {
    return ((double)m_totalTime / (double)CLOCKS_PER_SEC);
    }


void debug_printf (const char* pMsg, ...)
    {
#ifdef DEBUG
    if (pMsg == nullptr)
        return;
    va_list argList;
    va_start (argList, pMsg);
    vfprintf (stdout, pMsg, argList);
    va_end(argList);
#endif
    }
