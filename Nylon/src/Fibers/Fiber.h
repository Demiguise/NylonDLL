#ifndef __CFIBER_H__
#define __CFIBER_H__

#include <assert.h>
#include <Windows.h>
#include <atomic>
#include <string>
#include <vector>

#include "Util/Timer.h"
#include "Nylon.h"

#define FIBER_ENABLE_DEBUG 1

class CFiber;
class CNylonEngine;

class CFiberJobData
{
public:
	CFiberJobData() {}
	~CFiberJobData() {}

	void AddData(const int data)
	{
		m_datas.emplace_back(data);
	}
	void AddData(const float data)
	{
		m_datas.emplace_back(data);
	}
	void AddData(const bool data)
	{
		m_datas.emplace_back(data);
	}
	void AddData(void* data)
	{
		m_datas.emplace_back(data);
	}

	int GetInt(const int index) { return m_datas[index].i; }
	float GetFloat(const int index) { return m_datas[index].f; }
	bool GetBool(const int index) { return m_datas[index].b; }
	void* GetPointer(const int index) { return m_datas[index].pV; }

	int GetSize() { return m_datas.size(); }
private:
	union UJobData
	{
		int i;
		float f;
		bool b;
		void* pV;
	};
	std::vector<UJobData> m_datas;
};

typedef std::atomic<int> TCounter;
class CJobCounter
{
public:
	CJobCounter(int initialCount, std::string funcName = "Default")
		: m_funcName(funcName)
	{
		m_counter.store(initialCount);
	}

	~CJobCounter() {}

	bool IsComplete() const { return (m_counter.load() == 0); }
	void Decrement() { m_counter--; }
	const char* GetName() const { return m_funcName.c_str(); }

private:
	CJobCounter& operator= (CJobCounter& rhs); //Forcing no copy operator
	std::string m_funcName;
	TCounter m_counter;
	Nylon::TCounterID m_id;
};

#define CREATEJOB(name, func) SJobRequest name((LPFIBER_START_ROUTINE)func, #func)

struct SJobRequest
{
	SJobRequest()
		: m_pFunc(NULL)
		, m_pCounter(NULL)
		, m_pFiber(NULL) {}

	SJobRequest& operator= (SJobRequest rhs)
	{
		m_jobName = rhs.m_jobName;
		m_pFunc = rhs.m_pFunc;
		m_pJobData = rhs.m_pJobData;
		m_pCounter = rhs.m_pCounter;
		m_pFiber = rhs.m_pFiber;
		return *this;
	}

	std::string m_jobName;
	LPFIBER_START_ROUTINE m_pFunc;
	void* m_pJobData;
	CFiber* m_pFiber;
	Nylon::TJobID m_jobId;
	Nylon::TCounterID m_counterId;
};

class CFiber
{
public:
	enum EFiberState
	{
		eFS_InActive = 0,
		eFS_Active,
		eFS_WaitingForJob,
		eFS_Bound,
		eFS_HasNextFiber,
		eFS_Finished,
		eFS_Releasing,
		eFS_Yielded
	};

	CFiber(size_t stack);
	CFiber();
	~CFiber();

	void Init(UINT16 id, size_t stack, CNylonEngine* pEngine);
	void Bind(SJobRequest& job);
	void Release();

	static void __stdcall Run(LPVOID lpParam);

	//In-Fiber functions
	void ReleasePrevious();
	static void YieldForCounter(CJobCounter* counter);
	void SetNextFiber(CFiber* nextFiber);
	void EndJob();
	static void Log(std::string, ...);

	UINT16 GetID() const { return m_id; }
	SJobRequest* GetJobInfo() { return &m_job; }

	bool inline IsInState(EFiberState state) const;
	bool AtomicStateSwitch(EFiberState oldState, EFiberState newState);
	void inline SetState(EFiberState newState);

	void SetPrevious(CFiber* pPrevFiber) { m_pPrevFiber = pPrevFiber; }

	LPVOID Address() const { return m_pFiber; }
	bool IsActive() const { return m_state == eFS_Active; }

private:
	struct PersonalLogEntry
	{
		PersonalLogEntry(DWORD id, std::string line)
			: threadID(id)
			, entry(line)
		{
			timeStamp = Timer::GetCountNow();
		}
		DWORD threadID;
		std::string entry;
		UINT64 timeStamp;
	};
	typedef std::vector<PersonalLogEntry> TFiberPersonalLog;

	static void SetThreadName(LPCSTR name = "UNNAMED");

	TFiberPersonalLog m_personalLog;

	SJobRequest m_job;
	UINT16 m_id;
	size_t m_stackSize;
	std::atomic<EFiberState> m_state;
	LPVOID m_pFiber;
	CFiber* m_pPrevFiber;
	CFiber* m_pNextFiber;
	CNylonEngine* m_pEngine;
};

#endif //~__CFIBER_H__
