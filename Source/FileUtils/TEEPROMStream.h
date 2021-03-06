/*
 * Persistence Library / FileUtils / TEEPROMStream
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

#ifndef TEEPROMSTREAM___H
#define TEEPROMSTREAM___H

#include "TStream.h"


/**
 * Eeprom strean can be used for reading / writing data from/to memory 
 * that is not mapped directly to main address space.
 * 
 * Usually you need to call a short method for reading/writing each byte to eeprom
 * one by one.
 */
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
    virtual long WriteBuffer  (const void* Buffer, long Count);
    virtual long Seek (long Offset, ESeekOrigin Origin)=0;
    virtual long GetPosition()=0;
    virtual long GetSize()=0;
};


#endif
