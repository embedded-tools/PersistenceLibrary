#ifndef TWMLOG___H
#define TWMLOG___H

#include "TLogBase.h"
#include "TDateTime.h"
#include <windows.h>

class TWMLog : public TLogBase
{
	private: 
		HWND hWnd;

	protected:
		virtual void LogVerbose(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength);
		virtual void LogMinimal(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength);
		virtual void WriteToLog(const char* text, unsigned short textLength);

	public:
		TWMLog(void(*GetTimeHandler)(TDateTime &time));
	
};

#endif