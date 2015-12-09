#ifndef __JOBQUEUE_H__
#define __JOBQUEUE_H__

#include "Util/Spinlock.h"
#include "Fibers/Fiber.h"

class CJobQueue
{
public:
	CJobQueue();
	~CJobQueue();

	void Enqueue(SJobRequest& job);
	SJobRequest Dequeue();
	bool Empty() const { return m_queue.empty(); }

private:
	typedef std::queue<SJobRequest> TJobRequests;

	CSpinlock m_dequeueLock;
	CSpinlock m_enqueueLock;
	TJobRequests m_queue;
};

#endif //~__JOBQUEUE_H__
