/*
 * Persistence Library / FileUtils / TLog
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
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

#include "tlog.h"
#include <string.h>
#include <stdio.h>
		
TLog::TLog(void(*WriteToLog)(const char* pszText, int cbText), void(*GetTimeHandler)(TTime &time))
{
	m_writeToLogHandler = WriteToLog;
	m_getCurrentTimeHandler = GetTimeHandler;
}

void TLog::Log(LogType logType, short threadId, const char* message, short messageLength)
{
	char text[25];
	short textLength = 0;

	if (!m_writeToLogHandler)
	{
		return;
	}
	if (messageLength==-1)
	{
		messageLength = strlen(message);
	}
	
	if (m_getCurrentTimeHandler)
	{
		TTime currentTime(0,0,0);
		m_getCurrentTimeHandler(currentTime);
		textLength = currentTime.PrintTimeFull(text, sizeof(text));
	    m_writeToLogHandler(text, textLength);

		text[0] = '\t';
		textLength = 1;
        m_writeToLogHandler(text, textLength);    
	}	

	textLength = sprintf(text, "%05i\t", threadId);
	m_writeToLogHandler(text, textLength);

	const char* logTypeString = "Unknown  \t\"";
	switch (logType)
	{
		case ltDebug:	  logTypeString = "Debug    \t\""; break;
		case ltInfo:	  logTypeString = "Info     \t\""; break;
		case ltWarning:   logTypeString = "Warning  \t\""; break;
		case ltError:     logTypeString = "Error    \t\""; break;
		case ltException: logTypeString = "Exception\t\""; break;
	}
    char quota = '\"';
	m_writeToLogHandler(logTypeString, strlen(logTypeString));
    m_writeToLogHandler(message, messageLength);

    char crlf[3];
    crlf[0] = '\"';
    crlf[1] = 13;
    crlf[2] = 10;
    m_writeToLogHandler(crlf, 3);

}