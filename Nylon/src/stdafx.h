// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define FIBER_OUTPUT_ON 1
#define PERF_LOGS_ON 0

#define __NYLON_RELEASE_FIBER if(x) { x->Release(); x = NULL; }

// TODO: reference additional headers your program requires here

static const char* AppendNewlineChar(const char* input)
{
	int inputSize = strlen(input);
	char* output = new char[inputSize + 2];
	memmove(output, input, inputSize);
	output[inputSize] = '\n';
	output[inputSize + 1] = '\0';
	return output;
}

static void PushToLogs(const char* logLine)
{
	OutputDebugStringA(logLine);
}

static void DebugLog(const char* frmt, ...)
{
	char buffer[512];
	const char* newLogLine = AppendNewlineChar(frmt);
	va_list args;
	va_start(args, frmt);
	vsnprintf_s(buffer, 512, newLogLine, args);
	va_end(args);
	PushToLogs((LPCSTR)buffer);
	delete newLogLine;
}

static float Clampf(const float val, const float minVal, const float maxVal)
{
	if (val < minVal)
	{
		return minVal;
	}
	else if (val > maxVal)
	{
		return maxVal;
	}
	else
	{
		return val;
	}
}