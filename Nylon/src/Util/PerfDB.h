#ifndef PERF_DB_H
#define PERF_DB_H

#include <map>
#include <string>
#include <vector>

#define USE_RDTSC 0

class PerformanceDB
{
public:
	PerformanceDB();
	~PerformanceDB();

	void AddData(std::string name, UINT64 data);
	float GetAvgForData(std::string name);
	void ReportAvgs();

private:
	typedef std::vector<UINT64> TPerfData;
	//TPerfMap layout is [ThreadID][FunctionName]
	typedef std::map<DWORD, std::map<std::string, TPerfData>> TThreadedPerfMap;
	typedef std::map<std::string, TPerfData> TPerfMap;
	void MergeTemp(TPerfMap& tempMap);
	TPerfMap m_perfMap;
	TThreadedPerfMap m_scratchpad;

	float m_secsPerCount;
};
extern PerformanceDB* g_pPerfDB;

#endif //~PERF_DB_H
