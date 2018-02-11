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

#include "ttime.h"

enum LogType { ltDebug, ltInfo, ltWarning, ltError, ltException };


class TLog
{
	protected:

		void(*m_writeToLogHandler)(const char* pszText, int cbText);
		void(*m_getCurrentTimeHandler)(TTime &time);				

	public:

		TLog(void(*WriteToLog)(const char* pszText, int cbText), void(*GetTimeHandler)(TTime &time));

		void Log(LogType logType, short threadId, const char* message, short messageLength=-1);

};




#endif