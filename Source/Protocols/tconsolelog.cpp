#include "tconsolelog.h"
#include <stdio.h>

TConsoleLog::TConsoleLog(void(*GetTimeHandler)(TTime &time))
: TLog(WriteToConsole, GetTimeHandler)
{

}

void TConsoleLog::Init(void(*GetTimeHandler)(TTime &time))
{
    _internal_logger = new TConsoleLog(GetTimeHandler);
}

void TConsoleLog::WriteToConsole(const char* pszText, int cbText)
{
	char buf[16];
	sprintf(buf, "%%.%ds", cbText);	
	printf(buf, pszText);
}