/*
 * Persistence Library / Logs / TLog
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

#ifndef TLOGBASE___H
#define TLOGBASE___H

#include <stdlib.h>
#include "TDateTime.h"
#include "TLog.h"

enum LogType { ltDebug, ltInfo, ltWarning, ltError, ltException };

class TLogBase
{	
	protected:					
		static void(*m_getCurrentTimeHandler)(TDateTime &time);

		bool          m_simpleMutex;
		bool          m_verbose;
		unsigned long m_lastTime;
		unsigned long m_lastDate;
		unsigned long m_lastObjID;
		int           m_lastArg;

		unsigned char* m_buffer;
		unsigned short m_buffer_size;
		unsigned short m_buffer_len;

		TLogBase (bool verbose, void(*GetTimeHandler)(TDateTime &time));

		virtual void LogVerbose(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength);
		virtual void LogMinimal(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength);

		void WriteToCache(const char* text, unsigned short textLength);
		virtual void WriteToLog(const char* text, unsigned short textLength) = 0;		
		
	public:

		void Log(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength=-1);

		void SetOutputCache(unsigned char* memoryBuffer, unsigned short memoryBufferSize);
		void FlushCache();

};


#endif
