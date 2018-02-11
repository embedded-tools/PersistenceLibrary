/*
 * Persistence Library / FileUtils / TMemoryStream
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

#include "tmemorystream.h"

TMemoryStream::TMemoryStream(void* rewriteableBuffer, short rewriteableBufferLength)
{
	m_memory = (char*) rewriteableBuffer;
	m_memoryOwner = false;
	m_size = 0;
	m_capacity = rewriteableBufferLength;
	m_position = 0;

	m_canRead = true;
	m_canWrite = true;
	m_canSeek = true;	
}

TMemoryStream::TMemoryStream(const char* readOnlyBuffer, short readOnlyBufferLength)
{
	m_memory = (char*) readOnlyBuffer;
	m_memoryOwner = false;
	m_size = readOnlyBufferLength;
	m_capacity = readOnlyBufferLength;
	m_position = 0;

	m_canRead  = true;
	m_canWrite = false;
	m_canSeek  = true;
}

TMemoryStream::TMemoryStream(short capacity)
{
	m_memory = (char*) malloc(capacity);
	m_memoryOwner = true;
	m_size = 0;
	m_capacity = capacity;
	m_position = 0;

	m_canRead = true;
	m_canWrite = true;
	m_canSeek = true;
}

TMemoryStream::~TMemoryStream()
{
    Close();
}

void TMemoryStream::Close()
{
	m_size = 0;
	if (m_memoryOwner)
	{
		if (m_memory) free(m_memory);
		m_memory = NULL;
		m_size = 0;
		m_capacity = 0;
	}
}


long TMemoryStream::ReadBuffer(void* Buffer, long Count)
{
    if (Count==0) return 0;
    if (m_memory==NULL) return 0;
    
    int Result = m_size - m_position;
    if (Result > 0) 
    {
        if (Result > Count)
            Result = Count;

        char* p=m_memory+m_position;
        memcpy(Buffer, p, Result);      
        m_position+=Result;
        return Result;
    }
    return 0;
}

long TMemoryStream::WriteBuffer(void* Buffer, long Count)
{    
	if (m_position<0) return 0;
	if (Count<1) return 0;

    int newSize = m_position + Count;

	SetSize(newSize);
	if (m_size!=newSize)
	{
		return 0;
	}
    if (m_memory==NULL)
    {
        return 0;
    }

    char* p=m_memory+m_position;
    memcpy(p,Buffer, Count);
	m_position = newSize;
    return Count;
}

long TMemoryStream::Seek(long Offset, ESeekOrigin origin)
{
    switch (origin)
    {
		case esoFromBeginning:	m_position  = Offset;  break;
		case esoFromCurrent:	m_position += Offset;  break;
		case esoFromEnd:		m_position  = m_size + Offset; break;
    }     
    return m_position;
}

long TMemoryStream::SetSize(long newSize)
{
	if (!m_memoryOwner) 
	{
		if (newSize<m_capacity) 
		{
			m_size = newSize;
		} else {
			m_size = m_capacity;
		}
		return m_size;
	}

	if (newSize==0)
    {
        free(m_memory);
        m_memory = NULL;
		m_size = 0;
        m_capacity = 0;
        return 0;
    }

	if (newSize>m_capacity)
	{
		while(newSize>m_capacity)
		{
			if (m_capacity<512)
			{
				m_capacity+=64;
			} else {
				{
					m_capacity+=256;
				}
			}
		}
		if (m_memory!=NULL)
		{
			m_memory = (char*)realloc(m_memory, m_capacity);
		} else {
			m_memory =(char*) malloc(m_capacity);
			memset(m_memory, 0, m_capacity);
		}
		if (m_memory == NULL)
		{
			m_size = 0;
			m_capacity = 0;
		}
	}
	m_size = newSize;

	return m_size;
    
}

long TMemoryStream::GetPosition()
{
	return m_position;
}

long TMemoryStream::GetSize()
{
	return m_size;
}

void* TMemoryStream::ToPtr()
{
	return m_memory;
}
