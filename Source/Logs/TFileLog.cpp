/*
 * Persistence Library / Logs / TFileLog
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

#include "TFileLog.h"
#include <stdio.h>

TFileLog::TFileLog(const char* filename, void(*GetTimeHandler)(TDateTime &time))
: TLogBase(true, GetTimeHandler)
{
    m_filename = filename;
}

void TFileLog::WriteToLog(const char* text, unsigned short textLength)
{
    FILE* file = fopen(m_filename, "ab");
    if (file!=NULL)
    {
        fwrite(text, 1, textLength, file);
        fclose(file);    
    }
}
