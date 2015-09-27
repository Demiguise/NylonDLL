// Nylon.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Nylon.h"
#include "Fibers/FiberScheduler.h"

CFiberScheduler* g_pFiberScheduler = NULL;

NYLON_API void Nylon::Init(const int fiberCount, const int maxThreads)
{
	g_pFiberScheduler = new CFiberScheduler;
	g_pFiberScheduler->Initialise(fiberCount, maxThreads);
}

NYLON_API void Nylon::Shutdown()
{
	g_pFiberScheduler->Shutdown();
	delete g_pFiberScheduler;
}

NYLON_API Nylon::TJobID Nylon::QueueJob(LPFIBER_START_ROUTINE pJob, EJobPriority jobPriority, void* pJobData, CJobCounter* pCounter /*=NULL*/)
{
	SJobRequest job;
	job.m_pFunc = pJob;
	job.m_jobData = pJobData;
	job.m_pCounter = pCounter;
}

NYLON_API bool Nylon::CancelJob(const TJobID jobToCancel)
{

}

NYLON_API void Nylon::SetLoggingCallback()
{

}