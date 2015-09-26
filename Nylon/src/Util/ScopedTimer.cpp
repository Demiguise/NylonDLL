#include "stdafx.h"
#include "ScopedTimer.h"
#include "PerfDB.h"

#define LOG_SCOPED_TIMER 0

ScopedTimer::ScopedTimer(const char* name)
{
#if USE_RDTSC
	m_startCount = __rdtsc();
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startCount);
#endif
	memcpy(&m_timerName, &name, sizeof(char*));
}

ScopedTimer::~ScopedTimer()
{
	UINT64 endcount;
#if USE_RDTSC
	endCount = __rdtsc();
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&endcount);
#endif
	g_pPerfDB->AddData(m_timerName, (endcount - m_startCount));
}

