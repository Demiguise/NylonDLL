// Nylon.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Nylon.h"
#include "NylonEngine.h"

CNylonEngine* g_pNylonEngine = NULL;

NYLON_API void Nylon::Init(const int fiberCount, const int maxThreads)
{
	g_pNylonEngine = new CNylonEngine;
	g_pNylonEngine->Initialise(fiberCount, maxThreads);
}

NYLON_API void Nylon::Shutdown()
{
	g_pNylonEngine->Shutdown();
	delete g_pNylonEngine;
}

NYLON_API Nylon::TJobID Nylon::QueueJob(LPFIBER_START_ROUTINE pJob, EJobPriority jobPriority, void* pJobData, TCounterID counterId /*= 0*/)
{
	SJobRequest job;
	job.m_pFunc = pJob;
	job.m_pJobData = pJobData;
	job.m_counterId = counterId;
	g_pNylonEngine->Schedule(job, jobPriority);

	return 0;
}

NYLON_API bool Nylon::CancelJob(const TJobID jobToCancel)
{
	return false;
}

NYLON_API void Nylon::SetLoggingCallback(TLoggingCallback callback)
{
	if (g_pNylonEngine)
	{
		g_pNylonEngine->SetLoggingCallback(callback);
	}
}