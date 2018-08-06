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

#ifndef TLOG___H
#define TLOG___H

#include <stdlib.h>
#include "TTime.h"

enum LogType { ltDebug, ltInfo, ltWarning, ltError, ltException };

class TLog;

extern TLog* _internal_logger;

class TLog
{	
	protected:					

        TLog(void(*WriteToLog)(const char* pszText, int cbText), void(*GetTimeHandler)(TTime &time));		

		void(*m_writeToLogHandler)(const char* pszText, int cbText);
		void(*m_getCurrentTimeHandler)(TTime &time);				
		
	public:
        
        static void Init(void(*WriteToLog)(const char* pszText, int cbText), void(*GetTimeHandler)(TTime &time));		

		void Log(LogType logType, void* objectId, const char* message, short messageLength=-1);		
    
};


void DEBUG(void* OBJ, const char* A);
void INFO(void* OBJ, const char* A);
void WARNING(void* OBJ, const char* A);
void CRITICAL(void* OBJ, const char* A);
void EXCEPTION(void* OBJ, const char* A);

void DEBUG(void* OBJ, const char* A, int i1);
void INFO(void* OBJ, const char* A, int i1);
void WARNING(void* OBJ, const char* A, int i1);
void CRITICAL(void* OBJ, const char* A, int i1);
void EXCEPTION(void* OBJ, const char* A, int i1);

void DEBUG(void* OBJ, const char* A, const char* s1, int i1);
void INFO(void* OBJ, const char* A, const char* s1, int i1);
void WARNING(void* OBJ, const char* A, const char* s1, int i1);
void CRITICAL(void* OBJ, const char* A, const char* s1, int i1);
void EXCEPTION(void* OBJ, const char* A, const char* s1, int i1);

void DEBUG(void* OBJ, const char* A, int i1, int i2);
void INFO(void* OBJ, const char* A, int i1, int i2);
void WARNING(void* OBJ, const char* A, int i1, int i2);
void CRITICAL(void* OBJ, const char* A, int i1, int i2);
void EXCEPTION(void* OBJ, const char* A, int i1, int i2);

#endif