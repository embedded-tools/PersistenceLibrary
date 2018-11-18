/*
 * Persistence Library / FileUtils / TStringList
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

#ifndef TTEXTFILE___H
#define TTEXTFILE___H

#include <stdlib.h> 
#include <stdio.h>
#include "tstring.h"

/**
 *  Class for reading text files line by line.
 * 
*/
class TTextFile
{
private:	
    //buffer for storing pointers to string beginnings

    const char*     m_fileName;
    FILE*           m_fileHandle;
    unsigned char*  m_buffer;
    long			m_bufferDataCount;
    long			m_bufferDataMaxCount;

    bool AddText(TString& line, long numberOfChars);
public:

    TTextFile ();
    ~TTextFile ();

    bool Open(const char* fileName);
    void Close();    
    
    bool ReadLine(TString& line, long maxLineLength=1024);
};


#endif
