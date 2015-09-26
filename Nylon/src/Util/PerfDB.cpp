#include "stdafx.h"
#include "PerfDB.h"
#include "Common/Spinlock.h"
#include <mutex>

PerformanceDB* g_pPerfDB = NULL;

PerformanceDB::PerformanceDB()
{
	UINT64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	m_secsPerCount = 1 / (float)countsPerSecond;
}

PerformanceDB::~PerformanceDB()
{
}

void PerformanceDB::AddData(std::string name, UINT64 data)
{
	m_scratchpad[GetCurrentThreadId()][name].push_back(data);
}

float PerformanceDB::GetAvgForData(std::string name)
{
	return 0.f;
}

void PerformanceDB::MergeTemp(TPerfMap& tempMap)
{
	TPerfMap::iterator it = tempMap.begin();
	TPerfMap::iterator itEnd = tempMap.end();
	for (; it!= itEnd ; ++it)
	{
		TPerfMap::iterator itPerm = m_perfMap.find(it->first);
		if (itPerm == m_perfMap.end())
		{
			m_perfMap[it->first] = it->second;
		}
		else
		{
			TPerfData& times = it->second;
			itPerm->second.insert(itPerm->second.end(), times.begin(), times.end());
		}
		it->second.clear();
	}
}

void PerformanceDB::ReportAvgs()
{
	TThreadedPerfMap tempMap;
	m_scratchpad.swap(tempMap);

	TPerfMap combinedMap;
	TThreadedPerfMap::iterator threadIt = tempMap.begin();
	TThreadedPerfMap::iterator threadItEnd = tempMap.end();
	for (; threadIt != threadItEnd ; ++threadIt)
	{
		combinedMap.insert(threadIt->second.begin(), threadIt->second.end());
	}

	TPerfMap::iterator it = combinedMap.begin();
	TPerfMap::iterator itEnd = combinedMap.end();
	for (; it != itEnd ; ++it)
	{
		if (!it->second.empty())
		{
			TPerfData& times = it->second;
			UINT64 highest = 0;
			UINT64 lowest = 1000;
			UINT64 avg = 0;
			int numJobs = times.size();
			for (int i = 0 ; i < numJobs ; ++i)
			{
				UINT64& time = times[i];
				avg += time;
				if (time > highest)
				{
					highest = time;
				}
				else if (time < lowest)
				{
					lowest = time;
				}
			}
			avg /= numJobs;

#if USE_RDTSC
			DebugLog("	[%s] CountC: %d | Avg:%u | (%u...%u) | Total:%u", it->first.c_str(), numJobs, avg, lowest, highest, (numJobs * avg));
#else			
			DebugLog("	[%s] Count: %d | Avg:%f | (%f...%f)", it->first.c_str(), numJobs, avg * m_secsPerCount, lowest * m_secsPerCount, highest * m_secsPerCount);
#endif
		}
	}
	MergeTemp(combinedMap);
}