#include "TLog.h"
#include <windows.h>
#include <stdio.h>


void WriteToFile(const char* pszText, int cbText)
{
	FILE* hFileLog;

	hFileLog = fopen("file.txt", "ab");
	fwrite(pszText, 1, cbText, hFileLog);
	fclose(hFileLog);	
}

void GetTime(TTime &time)
{
	SYSTEMTIME windowsTime;

	GetSystemTime(&windowsTime);

	time.SetHour(windowsTime.wHour);
	time.SetMinute(windowsTime.wMinute);
	time.SetSecond(windowsTime.wSecond);
	time.SetMilliSecond(windowsTime.wMilliseconds);	
}

int main(int argc, char* argv[])
{
    TLog::Init(WriteToFile, GetTime);

	DEBUG    (NULL, "Text1");
    INFO     (NULL, "Text2");
	WARNING  (NULL, "Text2");
	ERROR    (NULL, "Text3");
	EXCEPTION(NULL, "Text4");
	return 0;	
}
