#ifdef NYLON_EXPORTS
#define NYLON_API _declspec(dllexport)
#else
#define NYLON_API _declspec(dllimport)
#endif

//API Outline
//Memory
//End User handles memory requirements, they pass in a pointer to a block of memory for the library to use.
//In this memory, we create the scheduler and intialise all the number of requested thread objects.
//They also handle the job contexts (aka workspaces) for each of the queued functions.

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
//Fibres are smaller versions of threads, they come with their own stack space and fibre storage. In addition, they are extremely easy to switch between.
//

//Nylon Fibre Implementation
//Nylon implements wrapper objects to handle the instantiation and proper destruction of fibre stack space.
//Whenever a fiber finishes, it grabs an unused Nylon Fibre and binds a new job to it. Then, after it has properly bound it switches over to that new Fibre.
//That new Fibre then releases the previously finished one so everything clears up.

//Fibre state follows this rough procedure.
//1) Inactive - Isn't doing anything.
//2) WaitingForJob - Has no job, currently waiting on one.
//3) Bound - Has a job bound to it, but hasn't started the job.
//4) Active - Has a job bound to it and is currently working through it.
//5) Finished - Has finished the bound job
//6) HasNextFiber - Has taken a job off of the queue and is ready to switch
//7) Releasing - Has passed over to new fibre and is ready to be released
//8) Yielded - Has yielded to another fibre, until a linked timer has been decremented to zero




namespace Nylon
{
}
