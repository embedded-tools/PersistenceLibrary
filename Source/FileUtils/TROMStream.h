/*
 * Persistence Library / FileUtils / TEEPROMStream
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

#ifndef TEEPROMSTREAM___H
#define TEEPROMSTREAM___H

#include "tstream.h"

class TROMStream : public TStream
{
private:

	long m_memoryBlockAddress;
	long m_memoryBlockSize;
	long m_currentAddress;

public:
	
    TROMStream(  long memoryBlockAddress, long memoryBlockSize );

    virtual void Close();
    virtual long ReadBuffer   (void* Buffer, long Count);
    virtual long WriteBuffer  (void* Buffer, long Count);
    virtual long Seek (long Offset, ESeekOrigin Origin);
    virtual long GetPosition()=0;
    virtual long GetSize()=0;
};


#endif
