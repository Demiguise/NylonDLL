#ifndef __CFIBERSCHEDULER_H__
#define __CFIBERSCHEDULER_H__

#include <queue>
#include <map>
#include <string>

#include "Fiber.h"
#include "CoreThread.h"
#include "Util/Timer.h"
#include "Util/JobQueue.h"
#include "Nylon.h"
#include <mutex>

#define __NYLON__GETJOBINFO static_cast<CFiber*>(GetFiberData())->GetJobInfo()->m_jobData

class CFiberScheduler
{
public:
	CFiberScheduler();
	~CFiberScheduler() {}
	
	void Initialise(const int maxFiberCount, const int maxRunningFibers);
	void Shutdown();

	CFiber* AcquireNextFiber(CFiber* pOldFiber);
	void Schedule(SJobRequest& job, Nylon::EJobPriority prio, CFiberJobData& data, CFiberCounter* pCounter = NULL);
	void FiberYield(CFiber* pFiber, CFiberCounter* pCounter);

	void StartJobs();
	void AllocateJobs();
	bool IsActive();

	void PrintAverageJobTime();

	typedef void(*LoggingCallback) (int, const char*);
	void SetLoggingCallback(LoggingCallback callback);

private:
	typedef std::pair<SThreadInfo, CFiber*> TActiveFibers;
	typedef std::map<CFiber*, CFiberCounter*> TAtomicFiberMap;
	typedef std::pair<CFiber*, CFiberCounter*> TAtomicFiberPair;

	void UpdateActiveFibers(CFiber* pFiber);

	void Log(const int logLevel, const char* frmt, ...);

	UINT16 m_maxFibers;
	UINT16 m_maxRunningFibers;

	typedef std::map<CFiber*, Timer> TTimerMap;
	TTimerMap m_activeTimers;
	std::vector<float> m_avgTimes;

	CFiber* m_fiberPool;
	TActiveFibers* m_activeFibers;
	TAtomicFiberMap m_yieldedFibers;
	std::queue<SJobRequest> m_jobQueue[Nylon::eFP_Num];

	LoggingCallback m_logCallback;
};

#endif //~__CFIBERSCHEDULER_H__
