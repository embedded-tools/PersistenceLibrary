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

#include "TLogBase.h"
#include <string.h>
#include <stdio.h>

void(*TLogBase::m_getCurrentTimeHandler)(TDateTime &time) = NULL;

TLogBase::TLogBase(bool verbose, void(*GetTimeHandler)(TDateTime &time))
{
	m_verbose = verbose;
	m_getCurrentTimeHandler = GetTimeHandler;

	m_simpleMutex = false;
	m_verbose = verbose;
	m_lastTime = 0;
	m_lastDate = 0;
	m_lastObjID = 0;
	m_lastArg = 0;
	m_buffer = NULL;
	m_buffer_size = 0;
	m_buffer_len = 0;

}

void TLogBase::Log(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength)
{
	if (m_verbose)
	{
		LogVerbose(logType, objectId, arg, message, messageLength);
	}
	else
	{
		LogMinimal(logType, objectId, arg, message, messageLength);
	}

}

void TLogBase::LogVerbose(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength)
{
	char  text[24];
	short textLength = 0;

	if (messageLength==-1)
	{
		messageLength = (short)strlen(message);
	}

	while(m_simpleMutex);
	m_simpleMutex = true;

	if (m_getCurrentTimeHandler)
	{
		TDateTime currentDateTime(0,0,0);
		m_getCurrentTimeHandler(currentDateTime);

		if (currentDateTime.GetYear() == 0)
		{
			textLength = currentDateTime.PrintTimeFull(text, sizeof(text));
		}
		else
		{
			textLength = currentDateTime.PrintDateTime(text, sizeof(text));
		}

		WriteToCache(text, textLength);

		text[0]=',';
		text[1]=' ';
		textLength = 2;
		WriteToCache(text, textLength);
	}
	textLength = sprintf(text, "0x%012lx, ", objectId);
	WriteToCache(text, textLength);

	textLength = sprintf(text, "%*ld, ", 11, arg);
	WriteToCache(text, textLength);

	const char* logTypeString =           "Unknown,   \"";
	switch (logType)
	{
		case ltDebug:	  logTypeString = "Debug,     \""; break;
		case ltInfo:	  logTypeString = "Info,      \""; break;
		case ltWarning:   logTypeString = "Warning,   \""; break;
		case ltError:     logTypeString = "Error,     \""; break;
		case ltException: logTypeString = "Exception, \""; break;
	}
	WriteToCache(logTypeString, 12);
    WriteToCache(message, messageLength);

    char crlf[3];
	crlf[0] = '\"';
	crlf[1] = 13;
	crlf[2] = 10;
	WriteToCache(crlf, 3);

	m_simpleMutex = false;
}

void TLogBase::LogMinimal(LogType logType, unsigned long objectId, long arg, const char* message, short messageLength)
{
	char  text[24];
	short textLength = 0;

	if (messageLength==-1)
	{
		messageLength = (short)strlen(message);
	}

	while(m_simpleMutex);
	m_simpleMutex = true;

	if (m_getCurrentTimeHandler)
	{
		TDateTime currentDateTime(0,0,0);
		m_getCurrentTimeHandler(currentDateTime);

		unsigned long cd = currentDateTime.GetTotalDays();
		if (cd != m_lastDate)
		{
			if (m_lastDate == 0)
			{
				text[0] = 'D';
				textLength = 1;
				WriteToCache(text, textLength);

				textLength = currentDateTime.PrintDate(text, sizeof(text));
				WriteToCache(text, textLength);

				text[0] = '\n';
				textLength = 1;
				WriteToCache(text, textLength);
			}
			else
			{
				unsigned long dd = cd - m_lastDate;
				if (m_lastDate != 0)
				{
					textLength = sprintf(text, "D+%ld\n", dd);
					WriteToCache(text, textLength);
				}
			}
			m_lastDate = cd;
		}

		unsigned long ct = currentDateTime.GetTotalMilliseconds();
		if (ct != m_lastTime)
		{
			if (m_lastTime == 0)
			{
				text[0] = 'T';
				textLength = 1;
				WriteToCache(text, textLength);

				textLength = currentDateTime.PrintTimeFull(text, sizeof(text));
				WriteToCache(text, textLength);

				text[0] = '\n';
				textLength = 1;
				WriteToCache(text, textLength);
			}
			else
			{
				unsigned long dt = ct - m_lastTime;
				if (m_lastTime != 0)
				{
					textLength = sprintf(text, "T+%ld\n", dt);
					WriteToCache(text, textLength);
				}
			}
			m_lastTime = ct;
		}
	}
	if (objectId != m_lastObjID)
	{
		m_lastObjID = objectId;

		textLength = sprintf(text, "O:0x%08lx\n", objectId);
		WriteToCache(text, textLength);
	}
	if (arg != m_lastArg)
	{
		m_lastArg = arg;

		textLength = sprintf(text, "A:%ld\n", arg);
		WriteToCache(text, textLength);
	}

	const char* logTypeString =           "U:";
	switch (logType)
	{
		case ltDebug:	  logTypeString = "D:"; break;
		case ltInfo:	  logTypeString = "I:"; break;
		case ltWarning:   logTypeString = "W:"; break;
		case ltError:     logTypeString = "E:"; break;
		case ltException: logTypeString = "X:"; break;
	}
	WriteToCache(logTypeString, 2);
    WriteToCache(message, messageLength);

    char lf;
	lf = 10;
	WriteToCache(&lf, 1);

	m_simpleMutex = false;
}

void TLogBase::WriteToCache(const char* text, unsigned short textLength)
{
	if (m_buffer)
	{
		while ((m_buffer_len + textLength) > m_buffer_size)
		{
			unsigned short maxLength = m_buffer_size - m_buffer_len;
			if (maxLength)
			{
				memcpy(m_buffer + m_buffer_len, text, maxLength);
				text       += maxLength;
				textLength -= maxLength;
				WriteToLog((const char*) m_buffer, m_buffer_size);
				m_buffer_len = 0;
			}
		}
		memcpy(m_buffer + m_buffer_len, text, textLength);
		m_buffer_len += textLength;
	}
	else
	{
		WriteToLog(text, textLength);
	}
}

void TLogBase::FlushCache()
{
	if (m_buffer)
	{
		WriteToLog((const char*) m_buffer, m_buffer_len);
		m_buffer_len = 0;
	}
}


