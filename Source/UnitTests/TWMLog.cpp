#include "TWMLog.h"
#include <stdlib.h>

TWMLog::TWMLog(void(*GetTimeHandler)(TDateTime &time))
	: TLogBase(true, GetTimeHandler)
{
	hWnd = NULL;
	m_buffer_size = 320;
	m_buffer = (unsigned char*) malloc(m_buffer_size);	
}

void TWMLog::WriteToLog(const char* text, unsigned short textLength)
{
	if (hWnd == NULL)
	{
		hWnd = FindWindowA(NULL, "Log Viewer");
	}

	//Win32 API manual says:
	//
	//"While this message is being sent, the referenced data must not be 
	//changed by another thread of the sending process."
	//
	//Therefore allocates new data structure for every message
	//(despite it actually is a memory leak)

	if (hWnd != NULL)
	{
		COPYDATASTRUCT* data = (COPYDATASTRUCT*)malloc(sizeof(COPYDATASTRUCT));
		data->dwData = 0;
		data->cbData = textLength;
		data->lpData = (PVOID)text;

		SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)data );
	}
}

void TWMLog::LogVerbose(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength)
{
	TLogBase::LogVerbose(logType, objectId, arg, message, messageLength);
	FlushCache();
}

void TWMLog::LogMinimal(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength)
{
	TLogBase::LogMinimal(logType, objectId, arg, message, messageLength);
	FlushCache();
}