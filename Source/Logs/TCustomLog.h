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

#ifndef TCUSTOMLOG___H
#define TCUSTOMLOG___H

#include "TLogBase.h"

class TCustomLog : public TLogBase
{
    private:
        static void (*m_writeToLog)(const char* pszText, int cbText);
        virtual void WriteToLog(const char* text, unsigned short textLength);
        
    public:
		TCustomLog(bool verbose, void(*GetTimeHandler)(TDateTime &dateTime), void(*WriteToLog)(const char* pszText, int cbText));

};


#endif
