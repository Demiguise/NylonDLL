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

class FiberGraphNode;


class CFiberScheduler
{
public:
	CFiberScheduler();
	~CFiberScheduler() {}
	
	void Initialise(const int maxFiberCount, const int maxThreads);
	void Shutdown();

	CFiber* AcquireNextFiber(CFiber* pOldFiber);

	Nylon::TJobID Schedule(SJobRequest& job, Nylon::EJobPriority prio);
	void CancelJob(Nylon::TJobID jobID);

	static void FiberYield(CFiber* pFiber, CJobCounter* pCounter);

	void StartJobs();
	void AllocateJobs();
	bool IsActive();

	void PrintAverageJobTime();

	typedef void(*LoggingCallback) (int, const char*);
	void SetLoggingCallback(LoggingCallback callback);
private:
	typedef std::pair<SThreadInfo, CFiber*> TActiveFibers;
	typedef std::map<CFiber*, CJobCounter*> TAtomicFiberMap;
	typedef std::pair<CFiber*, CJobCounter*> TAtomicFiberPair;

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

	Nylon::TJobID m_nextJobId;

	LoggingCallback m_logCallback;
};

extern CFiberScheduler* g_pFiberScheduler;

#endif //~__CFIBERSCHEDULER_H__
