/*
 * Persistence Library / Logs / TCustomLog
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

#include "TCustomLog.h"
#include <stdio.h>


void (*TCustomLog::m_writeToLog)(const char* pszText, int cbText) = NULL;


TCustomLog::TCustomLog(bool verbose, void(*GetTimeHandler)(TDateTime &dateTime), void(*WriteToLog)(const char* pszText, int cbText))
: TLogBase(verbose, GetTimeHandler)
{
	m_writeToLog = WriteToLog;
}

void TCustomLog::WriteToLog(const char* text, unsigned short textLength)
{
	if (m_writeToLog)
	{
		m_writeToLog(text, textLength);
	}
}
