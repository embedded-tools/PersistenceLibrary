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

class TEEPROMStream : public TStream
{
private:

	unsigned short (*m_ReadFromEEPROM_Function)(unsigned char* pOutputBuffer, unsigned short outputBufferLength);
	unsigned short (*m_WriteToEEPROM_Function) (unsigned char* pInputBuffer, unsigned short inputBufferLength);

	long m_memoryBlockAddress;
	long m_memoryBlockSize;
	long m_currentAddress;

public:
	
    TEEPROMStream(  long memoryBlockAddress, long memoryBlockSize,
                    unsigned short (*ReadFromEEPROM)(unsigned char* pOutputBuffer, unsigned short outputBufferLength),
                    unsigned short (*WriteToEEPROM) (unsigned char* pInputBuffer, unsigned short inputBufferLength)
                  );

    virtual void Close();
    virtual long ReadBuffer   (void* Buffer, long Count);
    virtual long WriteBuffer  (void* Buffer, long Count);
    virtual long Seek (long Offset, ESeekOrigin Origin)=0;
    virtual long GetPosition()=0;
    virtual long GetSize()=0;
};


#endif
