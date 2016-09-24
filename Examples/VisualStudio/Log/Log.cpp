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
	TLogger logger(WriteToFile, GetTime);

	logger.Log(ltInfo,      1, "Text1");
	logger.Log(ltWarning,   1, "Text2");
	logger.Log(ltError,     1, "Text3");
	logger.Log(ltException, 1, "Text4");

	return 0;	
}
