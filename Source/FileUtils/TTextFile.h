/*
 * Persistence Library / FileUtils / TStringList
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TTEXTFILE___H
#define TTEXTFILE___H

#include <stdlib.h> 
#include <stdio.h>
#include "tstring.h"
#include "tstream.h"
#include "tfilestream.h"
#include "stringutils.h"

class TTextFile
{
private:	
    //buffer for storing pointers to string beginnings

    const char*     m_fileName;
    FILE*           m_fileHandle;
    unsigned char*  m_buffer;
    unsigned short  m_bufferDataCount;
    unsigned short  m_bufferDataMaxCount;

    bool AddText(TString& line, unsigned short numberOfChars);
public:

    TTextFile ();
    ~TTextFile ();

    bool Open(const char* fileName);
    void Close();    
    
    bool ReadLine(TString& line, unsigned short maxLineLength=1024);
};


#endif
