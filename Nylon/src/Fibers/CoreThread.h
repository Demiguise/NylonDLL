#ifndef __CCORETHREAD_H__
#define __CCORETHREAD_H__

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

class CFiber;

struct SThreadInfo
{
	SThreadInfo() 
		: m_threadID(0)
		, m_hHeap(0)
		, m_pStartingFiber(NULL) {}
	DWORD m_threadID;
	HANDLE m_hHeap;
	CFiber* m_pStartingFiber;
};

class CCoreThread
{
public:
	CCoreThread(SThreadInfo* pThreadInfo);
	virtual ~CCoreThread();
	static DWORD WINAPI Run(LPVOID lpThreadParameter);

private:
	DWORD m_tlsIndex;
	CFiber* m_pInitialFiber;
};

#endif //~__CCORETHREAD_H__
