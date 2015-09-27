// Nylon.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Nylon.h"
#include "Fibers/FiberScheduler.h"

CFiberScheduler* g_pFiberScheduler = NULL;

NYLON_API void Nylon::Init(const int fiberCount, const int maxRunningFibers)
{
	g_pFiberScheduler = new CFiberScheduler;
	g_pFiberScheduler->Initialise(fiberCount, maxRunningFibers);
}

NYLON_API void Nylon::Shutdown()
{
	g_pFiberScheduler->Shutdown();
	delete g_pFiberScheduler;
}

NYLON_API Nylon::TJobID Nylon::QueueJob(LPFIBER_START_ROUTINE pJob, EJobPriority jobPriority, void* pJobData)
{

}

NYLON_API bool Nylon::CancelJob(const TJobID jobToCancel)
{

}

NYLON_API void Nylon::SetLoggingCallback()
{

}