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
#ifndef TFILELOG___H
#define TFILELOG___H

#include "TLogBase.h"

class TFileLog : public TLogBase
{
    private:
        const char* m_filename;
        virtual void WriteToLog(const char* text, unsigned short textLength);
        
    public:
		TFileLog(const char* filename, void(*GetTimeHandler)(TDateTime &time));

};


#endif
