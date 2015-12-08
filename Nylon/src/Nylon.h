#ifndef __NYLON_DLL_H__
#define __NYLON_DLL_H__
//API Outline
//Memory
//Initialising the NylonAPI via the Nylon::Init call will create the relevant objects in the DLL's memory space(? I think, unsure how DLL Memory actually works).
//This is because I can't guarentee the static nature of the queue/number of threads. If the user gave me some memory, I might overrun it if they shove a shit-ton of job requests into.
//End user will, however, handle the job contexts (aka workspaces) for each of the queued functions.

//Jobs
//Jobs are individual requests to run a specific function together with a priority, stack space size and job context.
//Job contexts are essentially function parameters but removed from the function, so instead of:
//Job::FuncA(int someInt, bool someBool)
//It now reads:
//Job::FuncA(LPVOID lpParam)
//Where lpParam is a Long Pointer VOID to some memory containing relevant information. In this example we would then have:
//struct SFuncAContext
//{
//	int someInt;
//	bool someBool;
//};
//FuncA now has access to the same function parameters as the non fibre version. The manner will be explained below in more detail under "Fibres"

//Job Scheduling
//Jobs are 'Scheduled' in a FIFO (First in, first out) manner with a lockless queue.
//For each priority (Low, Normal, High) there is a queue that jobs will be placed into once they are requested.
//Once a Fibre is started, via the starting of the system, it will run through it's current job.
//When that job is complete, it will return and iterate through each of the priority queues (Highest to lowest) and attempt to bind to a job.
//Then, if the binding is successful, the fibre will switch over to the new function and begin doing the next job.

//Fibres
//Fibres are smaller versions of threads, they come with their own stack space and local memory for easy access. In addition, they are extremely easy to switch between.
//Fibres do, however, run on threads and are started via a thread with a call to ConvertThreadToFiber.
//Fibres have to be run in a chain continously. If they need to wait until there are available jobs then they will wait in a spinlock.
//If the fibre somehow exits without having another one ready then the thread running the fibre will be destroyed.

//Nylon Fibre Implementation
//Nylon implements wrapper objects to handle the instantiation and proper destruction of fibre stack space.
//Whenever a fiber finishes, it grabs an unused Nylon Fibre and binds a new job to it. Then, after it has properly bound it switches over to that new Fibre.
//That new Fibre then releases the previously finished one so everything clears up.

//Fibre state follows this rough procedure.
// Inactive - Isn't doing anything.
// WaitingForJob - Has no job, currently waiting on one.
// Bound - Has a job bound to it, but hasn't started the job.
// Active - Has a job bound to it and is currently working through it.
// Finished - Has finished the bound job
// HasNextFiber - Has taken a job off of the queue and is ready to switch
// Releasing - Has passed over to new fibre and is ready to be released
// Yielded - Has yielded to another fibre, until a linked counter has been decremented to zero

//Rough Enduser workflow
//
//1) Initialise via Init API call, setting up the workspace. If in debug, setup logging callback.
//2) Declare jobs descriptions in their local area.
//3) Set up context for job and make sure it has a long enough lifetime. Memory, etc.
//4) Queue the job via API call, recieve back ID for job.
//5) Let job run.
//6) Get callback? (Dunno, haven't thought about that too much.)
//7) If we need to cancel, call the API function to delete that job from the queue or force it to stop.
//8) If we're quitting the application, shutdown the engine.
//9) During periods of inactivity, we should probably have the ability to let each fiber die so we're not wasting CPU. (Some kind of "Stop" or "Pause" function?)

//Atomic Counters
//The counters need to be instantiated and kept behind the scenes. They are quite small so shoudn't take up all that much space.
//Should these be perhaps limited to each fiber? Should they have a link in some way to the fibre? 
//AKA, Fiber 1 uses Counter 1 which is in the 0th index, Fiber 2 uses counter 2, etc etc.
//This would re-use the counters whenever possible, but I'm worried about stuff getting really out of sync if someone cancels a job and causes all sorts to happen.

//Issues/Concerns I need to think about
//Pointer caching to certain parts might be terribad, depending. Don't want to crash it.
//		Maybe solution is to create a NULL counter that just spams out errors instead of crashing?
//		How robust should the library be against crashing? Probably as much as possibly can do. Cuz, y'know. Crashing in someone else's library is annoying.
//How should cancelling a job work?
//		Have to be concerned about job heirarchies. How do they link together? In a graph perhaps? Once I kill the parent job, I guess the child jobs should also stop.
//		This means that this system needs to have a graph representation of everything and how each job interacts with the others. This could be kind of cool actually.
//		I would also need to rework how each fiber looks/works/grabs jobs I think. I want there to be a detailed understand of which jobs are doing what at which time.
//		Then I can output that to an enduser under some debug conditions that would make it muuuuuuch easier for them to understand what's happening with their data.
//		This does have to be a special case though as the memory usage would then increase quuiiitteee a bit. The fiber scheduler is going to have to keep track of all this graphing.

#ifdef NYLON_EXPORTS
#define NYLON_API _declspec(dllexport)
#else
#define NYLON_API _declspec(dllimport)
#endif

class CJobCounter;

// This is the external side of the NylonDLL and handles the user's requests.

namespace Nylon
{
	typedef unsigned int TJobID;
	typedef unsigned int TCounterID;

	enum EJobPriority
	{
		eFP_High = 0,
		eFP_Normal,
		eFP_Low,
		eFP_Num
	};

	enum ELogLevel
	{
		eLL_Info,
		eLL_Warning,
		eLL_Error
	};

	//Init - fiberCount is the number of available Fiber objects, maxThreads is the number of concurrently active fibers.
	NYLON_API void Init(const int fiberCount, const int maxThreads);
	NYLON_API void Shutdown();

	NYLON_API TCounterID CreateJobCounter();

	//QueueJob - Puts a new job request into the the queue with the specified priority, data and counter.
	NYLON_API TJobID QueueJob(LPFIBER_START_ROUTINE pJob, EJobPriority jobPriority, void* pJobData, TCounterID counterId = 0);
	NYLON_API bool CancelJob(const TJobID jobToCancel);

	typedef void (LoggingCallback)(int, const char*, ...);
	NYLON_API void SetLoggingCallback(LoggingCallback callback);
};

#endif //~__NYLON_DLL_H__