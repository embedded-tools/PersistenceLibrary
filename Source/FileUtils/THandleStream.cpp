/*
 * Persistence Library / FileUtils / THandleStream
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

#include "THandleStream.h"

/////////////////////////////
//      THandleStream
/////////////////////////////
THandleStream::THandleStream(FILE* AHandle)
{
    m_fileHandle = AHandle;
	m_fileSize   = 0;
}

THandleStream::THandleStream()
{
	m_fileSize   = 0;
}


THandleStream::~THandleStream()
{
    Close();
}

void THandleStream::Close()
{
    if (m_fileHandle!=NULL)
    {
        fclose(m_fileHandle);
        m_fileHandle=NULL;
    }
}

long THandleStream::ReadBuffer(void* Buffer, long Count)
{
    if (m_fileHandle==NULL)
    {
        return 0;
    }        
    unsigned short result = (unsigned short)fread(Buffer,1,Count,m_fileHandle);
    return result;
}

long THandleStream::WriteBuffer(const void* Buffer, long Count)
{
    if (m_fileHandle==NULL)
    {
        return 0;
    }
    unsigned short result = (unsigned short)fwrite(Buffer,1,Count,m_fileHandle);
    return result;
}

long THandleStream::Seek(long Offset, ESeekOrigin origin)
{    
	int position = 0;
    if (m_fileHandle)
    {
        fseek(m_fileHandle, Offset, origin);
        position = ftell(m_fileHandle);
    }
	return position;
}

long THandleStream::GetPosition()
{    
	int position = 0;
    if (m_fileHandle)
    {
        position = ftell(m_fileHandle);
    }
	return position;
}

long THandleStream::GetSize()
{
    if (m_fileHandle==NULL)
    {
        return 0;
    }
	if ((m_fileSize==0) || (m_canWrite))
	{
		int pos  = ftell(m_fileHandle);
		fseek(m_fileHandle,0,esoFromEnd);
		int size = ftell(m_fileHandle);
		fseek(m_fileHandle,pos, esoFromBeginning);
		m_fileSize = size;
	}	
	return m_fileSize;
}

