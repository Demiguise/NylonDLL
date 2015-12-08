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

#endif //~__NYLONG_STDAFX_H__