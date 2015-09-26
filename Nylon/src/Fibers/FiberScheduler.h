#ifndef __CFIBERSCHEDULER_H__
#define __CFIBERSCHEDULER_H__

#include <queue>
#include <map>
#include <string>

#include "Fiber.h"
#include "CoreThread.h"
#include "Util/Timer.h"
#include "Util/JobQueue.h"
#include <mutex>

#define __NYLON__GETJOBINFO static_cast<CFiber*>(GetFiberData())->GetJobInfo()->m_jobData

enum EFiberPriority
{
	eFP_High = 0,
	eFP_Normal,
	eFP_Low,
	eFP_Num
};


class CFiberScheduler
{
public:
	CFiberScheduler() {}
	~CFiberScheduler() {}
	
	void Initialise(const int maxFiberCount, const int maxRunningFibers);
	void Shutdown();

	CFiber* AcquireNextFiber(CFiber* pOldFiber);
	static void Schedule(SJobRequest& job, EFiberPriority prio, CFiberJobData& data, CFiberCounter* pCounter = NULL);
	void FiberYield(CFiber* pFiber, CFiberCounter* pCounter);

	void StartJobs();
	void AllocateJobs();
	bool IsActive();

	void PrintAverageJobTime();

private:
	typedef std::pair<SThreadInfo, CFiber*> TActiveFibers;
	typedef std::map<CFiber*, CFiberCounter*> TAtomicFiberMap;
	typedef std::pair<CFiber*, CFiberCounter*> TAtomicFiberPair;

	void UpdateActiveFibers(CFiber* pFiber);

	const static UINT16 k_maxFiberPool = 32;
	const static UINT16 k_maxRunningFibers = 4;

	typedef std::map<CFiber*, Timer> TTimerMap;
	TTimerMap m_activeTimers;
	std::vector<float> m_avgTimes;

	CFiber m_fiberPool[k_maxFiberPool];
	TActiveFibers m_activeFibers[k_maxRunningFibers];
	TAtomicFiberMap m_yieldedFibers;
	std::queue<SJobRequest> m_jobQueue[eFP_Num];
};

extern CFiberScheduler* g_pFiberScheduler;

#endif //~__CFIBERSCHEDULER_H__
