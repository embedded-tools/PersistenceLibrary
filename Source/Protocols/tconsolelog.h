#ifndef TCONSOLELOG___H
#define TCONSOLELOG___H

#include "tlog.h"
#include "TTime.h"

class TConsoleLog : public TLog
{
    protected:
        static void WriteToConsole(const char* pszText, int cbText);       
        TConsoleLog(void(*GetTimeHandler)(TTime &time));
        
public:
		        
        static void Init(void(*GetTimeHandler)(TTime &time));

};


#endif