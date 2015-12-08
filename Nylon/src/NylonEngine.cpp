#include "stdafx.h"
#include "NylonEngine.h"

CNylonEngine::CNylonEngine()
{

}

CNylonEngine::~CNylonEngine()
{

}

void CNylonEngine::Initialise(const int numFibers, const int maxThreads)
{

}

void CNylonEngine::Shutdown()
{

}

void CNylonEngine::Log(Nylon::ELogLevel level, const char* frmt, ...)
{
	if (m_loggingCallback)
	{
		va_list args;
		va_start(args, frmt);
		m_loggingCallback(level, frmt, args);
		va_end(args);
	}
	else
	{
		//Default to outputing debug string to visual studio
		char buffer[s_maxLineLength];

		//Append '\n' to format string
		const int formatSize = strlen(frmt);
		char* newFormat = new char[formatSize + 2];
		memmove(newFormat, frmt, formatSize);
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