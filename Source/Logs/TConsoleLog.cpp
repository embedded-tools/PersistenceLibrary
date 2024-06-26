/*
 * Persistence Library / Logs / TConsoleLog
 *
 * Copyright (c) 2016-2018 Ondrej Sterba <osterba@atlas.cz>
 *
 * https://github.com/embedded-tools/PersistenceLibrary
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include "TConsoleLog.h"
#include <stdio.h>

TConsoleLog::TConsoleLog(void(*GetTimeHandler)(TDateTime &time))
: TLogBase(true, GetTimeHandler)
{

}

void TConsoleLog::WriteToLog(const char* text, unsigned short textLength)
{
	char buf[16];
	sprintf(buf, "%%.%ds", textLength);	
	printf(buf, text);
}
