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
	g_pFiberScheduler->ShutDown();
	delete g_pFiberScheduler;
}