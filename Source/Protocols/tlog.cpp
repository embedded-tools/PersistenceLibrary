/*
 * Persistence Library / FileUtils / TLog
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

#include "tlog.h"
#include <string.h>
#include <stdio.h>

TLog* _internal_logger = NULL;
		
static bool simpleMutex = false;        
        
TLog::TLog(void(*WriteToLog)(const char* pszText, int cbText), void(*GetTimeHandler)(TTime &time))
{
	m_writeToLogHandler = WriteToLog;
	m_getCurrentTimeHandler = GetTimeHandler;
}

void TLog::Init(void(*WriteToLog)(const char* pszText, int cbText), void(*GetTimeHandler)(TTime &time))
{
    if (_internal_logger)
    {
        delete _internal_logger;
    }
    if ((WriteToLog==NULL) || (GetTimeHandler==NULL))
    {
        _internal_logger = NULL;
    } else {
        _internal_logger = new TLog(WriteToLog, GetTimeHandler);
    }
}

void TLog::Log(LogType logType, void* objectId, const char* message, short messageLength)
{
	char  text[40];
	short textLength = 0;

	if (!m_writeToLogHandler)
	{
		return;
	}
	if (messageLength==-1)
	{
		messageLength = strlen(message);
	}
    
    while(simpleMutex);    
    simpleMutex = true;
    	    
	if (m_getCurrentTimeHandler)
	{
		TTime currentTime(0,0,0);
		m_getCurrentTimeHandler(currentTime);
		textLength = currentTime.PrintTimeFull(text, sizeof(text));
	    m_writeToLogHandler(text, textLength);
		
		text[0]=' ';
        text[1]=' ';
		textLength = 2;
		m_writeToLogHandler(text, textLength);
	}	

	textLength = sprintf(text, "%010u  ", objectId);
	m_writeToLogHandler(text, textLength);

	const char* logTypeString =           "Unknown  \t\"";
	switch (logType)
	{
		case ltDebug:	  logTypeString = "Debug  \t\""; break;
		case ltInfo:	  logTypeString = "Info   \t\""; break;
		case ltWarning:   logTypeString = "Warning\t\""; break;
		case ltError:     logTypeString = "Error  \t\""; break;
		case ltException: logTypeString = "Exceptn\t\""; break;
	}
	m_writeToLogHandler(logTypeString, strlen(logTypeString));
    m_writeToLogHandler(message, messageLength);

    char crlf[3];
    crlf[0] = '\"';
    crlf[1] = 13;
    crlf[2] = 10;
    m_writeToLogHandler(crlf, 3);
    
    simpleMutex = false;
}

void DEBUG(void* OBJ, const char* A)
{
    if (_internal_logger) 
    {
        _internal_logger->Log(ltDebug, OBJ, A);
    }	
}

void INFO(void* OBJ, const char* A)
{
    if (_internal_logger) 
    {
        _internal_logger->Log(ltInfo, OBJ, A);
    }	
}

void WARNING(void* OBJ, const char* A)
{
    if (_internal_logger) 
    {
        _internal_logger->Log(ltWarning, OBJ, A);
    }
}

void ERROR(void* OBJ, const char* A)
{
    if (_internal_logger) 
    {
        _internal_logger->Log(ltError, OBJ, A);
    }
}
  
void EXCEPTION(void* OBJ, const char* A)
{
    if (_internal_logger) 
    {
        _internal_logger->Log(ltException, OBJ, A);
    }
} 


void DEBUG(void* OBJ, const char* A, int i1)
{	
    if (_internal_logger) 
    {		
		char buf[128];
#ifdef WIN32
		_snprintf(buf, sizeof(buf), A, i1);		
#else
        snprintf(buf, sizeof(buf), A, i1);		
#endif
        _internal_logger->Log(ltDebug, OBJ, buf);
    }
}

void INFO(void* OBJ, const char* A, int i1)
{	
    if (_internal_logger) 
    {		
        char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, i1);		
#else 
        snprintf(buf, sizeof(buf), A, i1);		
#endif
        _internal_logger->Log(ltInfo, OBJ, buf);
    }
}


void WARNING(void* OBJ, const char* A, int i1)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
		_snprintf(buf, sizeof(buf), A, i1);		
#else
        snprintf(buf, sizeof(buf), A, i1);		
#endif
        _internal_logger->Log(ltWarning, OBJ, buf);
    }
}

void ERROR(void* OBJ, const char* A, int i1)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
		_snprintf(buf, sizeof(buf), A, i1);		
#else
        snprintf(buf, sizeof(buf), A, i1);		
#endif
        _internal_logger->Log(ltError, OBJ, buf);
    }
}

void EXCEPTION(void* OBJ, const char* A, int i1)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, i1);		
#else
		snprintf(buf, sizeof(buf), A, i1);		
#endif
        _internal_logger->Log(ltException, OBJ, buf);
    }
}

void DEBUG(void* OBJ, const char* A, int i1, int i2)
{	
    if (_internal_logger) 
    {		
		char buf[128];
#ifdef WIN32
		_snprintf(buf, sizeof(buf), A, i1, i2);		
#else
        snprintf(buf, sizeof(buf), A, i1, i2);		
#endif
        _internal_logger->Log(ltDebug, OBJ, buf);
    }
}

void INFO(void* OBJ, const char* A, int i1, int i2)
{	
    if (_internal_logger) 
    {		
        char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, i1, i2);		
#else
        snprintf(buf, sizeof(buf), A, i1, i2);		
#endif
        _internal_logger->Log(ltInfo, OBJ, buf);
    }
}


void WARNING(void* OBJ, const char* A, int i1, int i2)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, i1, i2);		
#else
		snprintf(buf, sizeof(buf), A, i1, i2);		
#endif
        _internal_logger->Log(ltWarning, OBJ, buf);
    }
}

void ERROR(void* OBJ, const char* A, int i1, int i2)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, i1, i2);		
#else
		snprintf(buf, sizeof(buf), A, i1, i2);		
#endif
        _internal_logger->Log(ltError, OBJ, buf);
    }
}

void EXCEPTION(void* OBJ, const char* A, int i1, int i2)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, i1, i2);		
#else
		snprintf(buf, sizeof(buf), A, i1, i2);		
#endif
        _internal_logger->Log(ltException, OBJ, buf);
    }
}

void DEBUG(void* OBJ, const char* A, const char* s1, int i2)
{	
    if (_internal_logger) 
    {		
		char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, s1, i2);		
#else 
		snprintf(buf, sizeof(buf), A, s1, i2);		
#endif
        _internal_logger->Log(ltDebug, OBJ, buf);
    }
}

void INFO(void* OBJ, const char* A, const char* s1, int i2)
{	
    if (_internal_logger) 
    {		
        char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, s1, i2);		
#else
        snprintf(buf, sizeof(buf), A, s1, i2);		
#endif
        _internal_logger->Log(ltInfo, OBJ, buf);
    }
}

void WARNING(void* OBJ, const char* A, const char* s1, int i2)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
		_snprintf(buf, sizeof(buf), A, s1, i2);		
#else
        snprintf(buf, sizeof(buf), A, s1, i2);		
#endif
        _internal_logger->Log(ltWarning, OBJ, buf);
    }
}

void ERROR(void* OBJ, const char* A, const char* s1, int i2)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
		_snprintf(buf, sizeof(buf), A, s1, i2);		
#else
        snprintf(buf, sizeof(buf), A, s1, i2);		
#endif
        _internal_logger->Log(ltError, OBJ, buf);
    }
}

void EXCEPTION(void* OBJ, const char* A, const char* s1, int i2)
{
    if (_internal_logger) 
    {
		char buf[128];
#ifdef WIN32
        _snprintf(buf, sizeof(buf), A, s1, i2);		
#else
		snprintf(buf, sizeof(buf), A, s1, i2);		
#endif
        _internal_logger->Log(ltException, OBJ, buf);
    }
}
