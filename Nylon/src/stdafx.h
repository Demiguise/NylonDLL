#ifndef __NYLON_STDAFX_H__
#define __NYLON_STDAFX_H__

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#define FIBER_OUTPUT_ON 1
#define PERF_LOGS_ON 0

#define __NYLON_RELEASE_FIBER if(x) { x->Release(); x = NULL; }
#define __NYLON_INFO(frmt, ...) Debug::Log(frmt, __VA_ARGS__)
#define __NYLON_WARN(frmt, ...) Debug::Log(frmt, __VA_ARGS__)
#define __NYLON_ERR(frmt, ...) Debug::Log(frmt, __VA_ARGS__)

namespace Debug
{
	static const int s_maxLineLength = 512;
	static void Log(const char* frmt, ...)
	{
		char buffer[s_maxLineLength];

		//Append '\n' to format string
		const int formatSize = strlen(frmt);
		char* newFormat = new char[formatSize + 2];
		memmove(newFormat, newFormat, formatSize);
		newFormat[formatSize] = '\n';
		newFormat[formatSize + 1] = '\0';
		
		va_list args;
		va_start(args, frmt);
		vsnprintf_s(buffer, s_maxLineLength, newFormat, args);
		va_end(args);

		OutputDebugStringA(buffer);

		delete newFormat;
	}
}

#endif //~__NYLONG_STDAFX_H__