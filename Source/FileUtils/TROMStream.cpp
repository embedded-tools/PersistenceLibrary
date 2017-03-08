/*
 * Persistence Library / FileUtils / TROMStream
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
         
#include "TROMStream.h"
         
         
TROMStream::TROMStream(long memoryBlockAddress, long memoryBlockSize)
{
    m_memoryBlockAddress = memoryBlockAddress;
    m_memoryBlockSize = memoryBlockSize;

    FCanRead  = true;
    FCanWrite = false;
    FCanSeek  = true;
}


long TROMStream::ReadBuffer   (void* Buffer, long Count)
{
    if (Count>m_memoryBlockSize-m_currentAddress)
    {
        Count = m_memoryBlockSize-m_currentAddress;
    }
    if (Count>0)
    {
        memcpy(Buffer, (const void*)(m_memoryBlockAddress+m_currentAddress), Count);
    }    
	return Count;
}

long TROMStream::WriteBuffer  (void* Buffer, long Count)
{
	return 0;
}

long TROMStream::Seek (long Offset, ESeekOrigin Origin)
{
	switch (Origin)
	{
		case esoFromBeginning:
			{
				m_currentAddress  = Offset;
			}
			break;

		case esoFromCurrent:
			{
				m_currentAddress += Offset;
			}
			break;


		case esoFromEnd:
			{
				m_currentAddress = m_memoryBlockAddress + m_memoryBlockSize - Offset;
			}
			break;

	}
	if (m_currentAddress < m_memoryBlockAddress)
	{
		m_currentAddress = m_memoryBlockAddress;
	}
	if (m_currentAddress >= (m_memoryBlockAddress + m_memoryBlockSize))
	{
		m_currentAddress = m_memoryBlockAddress + m_memoryBlockSize;
	}
	return m_currentAddress;

}

long TROMStream::GetPosition()
{
	return m_currentAddress-m_memoryBlockAddress;
}

long TROMStream::GetSize()
{
	if (FCanWrite)
	{
		return m_currentAddress-m_memoryBlockAddress;
	}
	if (FCanRead)
	{
		return m_memoryBlockSize;
	}
	return 0;
};

