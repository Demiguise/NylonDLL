#ifndef __NYLON_ENGINE_H__
#define __NYLON_ENGINE_H__

#include "Nylon.h"
#include "Fibers/FiberScheduler.h"
#include "Util/JobQueue.h"

//This is the internal side of the NylonDLL and deals with all the implementation
//Is the new version of the scheduler, without all the additional parts to it.

#define __NYLON_INFO__(frmt, ...) Debug::Log(Nylon::eLL_Info, frmt, __VA_ARGS__)
#define __NYLON_WARN__(frmt, ...) Debug::Log(Nylon::eLL_Warning, frmt, __VA_ARGS__)
#define __NYLON_ERR__(frmt, ...) Debug::Log(Nylon::eLL_Error, frmt, __VA_ARGS__)

class CNylonEngine
{
public:
	CNylonEngine() {}
	~CNylonEngine() {}


	void Initialise(const int numFibers, const int maxThreads);
	void Shutdown();

	void Schedule();

	void Log(Nylon::ELogLevel, const char* frmt, ...);

	void SetLoggingCallback(Nylon::TLoggingCallback callback);

private:
	static const int s_maxLineLength = 512;

	Nylon::TLoggingCallback m_loggingCallback;
	CFiberScheduler m_scheduler;
	CJobQueue m_jobQueue[Nylon::eFP_Num];
};

#endif //~__NYLON_ENGINE_H__