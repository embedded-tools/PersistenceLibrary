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
         
#include "TEEPROMStream.h"
         
         
TEEPROMStream::TEEPROMStream(long memoryBlockAddress, long memoryBlockSize,
                             unsigned short (*ReadFromEEPROM)(unsigned char* pOutputBuffer, unsigned short outputBufferLength),
							 unsigned short (*WriteToEEPROM) (unsigned char* pInputBuffer, unsigned short inputBufferLength)
							)
{
    m_memoryBlockAddress = memoryBlockAddress;
    m_memoryBlockSize = memoryBlockSize;
	m_ReadFromEEPROM_Function = ReadFromEEPROM;
	m_WriteToEEPROM_Function  = WriteToEEPROM;

    m_canRead  = true;
    m_canWrite = true;
    m_canSeek  = true;
}


long TEEPROMStream::ReadBuffer   (void* Buffer, long Count)
{
	if (m_ReadFromEEPROM_Function)
	{
		return m_ReadFromEEPROM_Function((unsigned char*)Buffer, (unsigned short)Count);
	}
	return 0;
}

long TEEPROMStream::WriteBuffer  (const void* Buffer, long Count)
{
	if (m_WriteToEEPROM_Function)
	{
		return m_WriteToEEPROM_Function((unsigned char*)Buffer, (unsigned short)Count);
	}
	return 0;
}

long TEEPROMStream::Seek (long Offset, ESeekOrigin Origin)
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

long TEEPROMStream::GetPosition()
{
	return m_currentAddress-m_memoryBlockAddress;
}

long TEEPROMStream::GetSize()
{
	if (m_canWrite)
	{
		return m_currentAddress-m_memoryBlockAddress;
	}
	if (m_canRead)
	{
		return m_memoryBlockSize;
	}
	return 0;
};

