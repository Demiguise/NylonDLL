#include "stdafx.h"
#include "FiberScheduler.h"
#include "CoreThread.h"
#include "Util/Spinlock.h"

CSpinlock enQueueLock;

CFiberScheduler::CFiberScheduler()
	: m_nextJobId(0)
{
}

void CFiberScheduler::Initialise(const int maxFiberCount, const int maxThreads)
{
	m_fiberPool = new CFiber[maxFiberCount];
	m_activeFibers = new TActiveFibers[maxThreads];

	for (int i = 0; i < maxFiberCount; ++i)
	{
		m_fiberPool[i].Init(i, 0);
	}
}

void CFiberScheduler::Shutdown()
{
	delete m_fiberPool;
	delete m_activeFibers;
}

CFiber* CFiberScheduler::AcquireNextFiber(CFiber* pOldFiber)
{
	CFiber* boundFiber = NULL;

	// Check if there are any yielded fibers waiting with a 0 counter
	TAtomicFiberMap::iterator it = m_yieldedFibers.begin();
	while (it != m_yieldedFibers.end())
	{
		if (it->second->IsComplete())
		{
			boundFiber = it->first;
			m_yieldedFibers.erase(it);
			boundFiber->SetPrevious(pOldFiber);
			return boundFiber;
		}
		it++;
	}

	for (int i = 0 ; i < m_maxFibers ; ++i)
	{
		CFiber& fiber = m_fiberPool[i];
		if (fiber.AtomicStateSwitch(CFiber::eFS_InActive, CFiber::eFS_WaitingForJob))
		{
			boundFiber = &fiber;
			break;
		}
	}

	for (int i = 0 ; i < Nylon::eFP_Num ; ++i)
	{
		CScopedLock lock(&enQueueLock);
		if (!m_jobQueue[i].empty())
		{
			SJobRequest job = m_jobQueue[i].front();
			boundFiber->Bind(job);
			m_jobQueue[i].pop();
			break;
		}
	}

	if (!boundFiber->InState(CFiber::eFS_Bound))
	{
		DebugLog("Fiber (%d) failed to bind.", boundFiber->GetID());
		return NULL;
	}

	boundFiber->SetPrevious(pOldFiber);

	return boundFiber;
}

void CFiberScheduler::StartJobs()
{
	for (int i = 0 ; i< m_maxRunningFibers; ++i)
	{
		SThreadInfo& info = m_activeFibers[i].first;
		CFiber* pInitialFiber = AcquireNextFiber(NULL);
		pInitialFiber->AtomicStateSwitch(CFiber::eFS_InActive, CFiber::eFS_Bound);
		m_activeFibers[i].second = pInitialFiber;
		info.m_pStartingFiber = pInitialFiber;
		CCoreThread thread(&info);
	}
}

void CFiberScheduler::AllocateJobs()
{
	for (int i = 0 ; i < m_maxRunningFibers; ++i)
	{
		if (CFiber* pFiber = m_activeFibers[i].second)
		{
			if (pFiber->InState(CFiber::eFS_Yielded) || pFiber->InState(CFiber::eFS_WaitingForJob))
			{
				if (CFiber* newFiber = AcquireNextFiber(pFiber))
				{
					TTimerMap::iterator it = m_activeTimers.find(pFiber);
					if (it != m_activeTimers.end())
					{
						m_avgTimes.push_back(it->second.elapsedTime());
						m_activeTimers.erase(it);
					}

					pFiber->SetNextFiber(newFiber);
					Timer& newT = m_activeTimers[newFiber];
					newT.startTimer();
					m_activeFibers[i].second = newFiber;
				}
				break;
			}
		}
	}
}

bool CFiberScheduler::IsActive()
{
	for (int i = 0; i < m_maxRunningFibers; ++i)
	{
		CFiber* pActiveFiber = m_activeFibers[i].second;
		if (!pActiveFiber->InState(CFiber::eFS_WaitingForJob))
		{
			return true;
		}
	}
	for (int i = 0; i < Nylon::eFP_Num; ++i)
	{
		if (!m_jobQueue[i].empty())
		{
			return true;
		}
	}
	return false;
}

Nylon::TJobID CFiberScheduler::Schedule(SJobRequest& job, Nylon::EJobPriority prio)
{
	CScopedLock lock(&enQueueLock);
	Nylon::TJobID newID = m_nextJobId++;
	m_jobQueue[prio].push(job);
	return m_nextJobId;
}

void CFiberScheduler::CancelJob(Nylon::TJobID jobID)
{
	for (int i = 0; i < m_maxRunningFibers; ++i)
	{
		CFiber* pActiveFiber = m_activeFibers[i].second;
		if (pActiveFiber->GetJobInfo()->m_pJobID)
	}
}

void CFiberScheduler::FiberYield(CFiber* pFiber, CJobCounter* pCounter)
{
	pFiber->SetState(CFiber::eFS_Yielded);
	g_pFiberScheduler->m_yieldedFibers.insert(TAtomicFiberPair(pFiber, pCounter));
}

void CFiberScheduler::UpdateActiveFibers(CFiber* pFiber)
{
	DWORD curThreadId = GetCurrentThreadId();
	for (int i = 0; i < m_maxRunningFibers; ++i)
	{
		if (m_activeFibers[i].first.m_threadID == curThreadId)
		{
			m_activeFibers[i].second = pFiber;
			break;
		}
	}
}

void CFiberScheduler::Log(const int logLevel, const char* frmt, ...)
{
	char buffer[512];
	const char* newLogLine = AppendNewlineChar(frmt);
	va_list args;
	va_start(args, frmt);
	vsnprintf_s(buffer, 512, newLogLine, args);
	va_end(args);

	if (m_logCallback)
	{
		m_logCallback(logLevel, newLogLine);
	}
	else
	{
		OutputDebugStringA(newLogLine);
	}

	delete newLogLine;
}

void CFiberScheduler::PrintAverageJobTime()
{
	float avg = 0.f;
	float highest = 0.f;
	float lowest = 100.f;
	const int numJobs = m_avgTimes.size();
	for (int i = 0 ; i < numJobs ; ++i)
	{
		float& time = m_avgTimes[i];
		avg += m_avgTimes[i];
		if (time > highest)
		{
			highest = time;
		}
		else if (time < lowest)
		{
			lowest = time;
		}
	}

	avg /= numJobs;
	DebugLog("JPS: %d | ATPJ: %f | (%f...%f) | Total:%f", (int)numJobs, avg, lowest, highest, (numJobs * avg) / m_maxRunningFibers);
	m_avgTimes.clear();
}

void CFiberScheduler::SetLoggingCallback(LoggingCallback callback)
{
	m_logCallback = callback;
}