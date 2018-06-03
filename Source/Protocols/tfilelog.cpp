/*
 * Persistence Library / FileUtils / TFileLog
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

#include "tfilelog.h"
#include <stdio.h>

const char* TFileLog::m_filename = NULL;

TFileLog::TFileLog(const char* filename, void(*GetTimeHandler)(TTime &time))
: TLog(WriteToFile, GetTimeHandler)
{
    //const char* m_filename;
    m_filename = filename;
}

void TFileLog::Init(const char* filename, void(*GetTimeHandler)(TTime &time))
{
    _internal_logger = new TFileLog(filename, GetTimeHandler);
}

void TFileLog::WriteToFile(const char* pszText, int cbText)
{
    FILE* file = fopen(m_filename, "ab");
    if (file!=NULL)
    {
        fwrite(pszText, 1, cbText, file);
        fclose(file);    
    }
}

    
        