#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H

#include <wtypes.h>

#if PERF_LOGS_ON
#define PERFTIMER_FUNC() ScopedTimer funcTimer  = ScopedTimer(__FUNCTION__)
#define PERFTIMER(x) ScopedTimer perfTimer = ScopedTimer(__FUNCTION__ " (" x ")")
#else
#define PERFTIMER_FUNC()
#define PERFTIMER(x)
#endif

class ScopedTimer
{
public:
	ScopedTimer(const char* name);
	~ScopedTimer();
private:
	UINT64 m_startCount;
	const char* m_timerName;
};
#endif //~SCOPEDTIMER_H
