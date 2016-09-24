/*
 * Persistence Library / FileUtils / TMemoryStream
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

#include "tmemorystream.h"

TMemoryStream::TMemoryStream(void* rewriteableBuffer, short rewriteableBufferLength)
{
	FMemory = (char*) rewriteableBuffer;
	FMemoryOwned = false;
	FSize = 0;
	FCapacity = rewriteableBufferLength;
	FPosition = 0;

	FCanRead = true;
	FCanWrite = true;
	FCanSeek = true;	
}

TMemoryStream::TMemoryStream(const char* readOnlyBuffer, short readOnlyBufferLength)
{
	FMemory = (char*) readOnlyBuffer;
	FMemoryOwned = false;
	FSize = readOnlyBufferLength;
	FCapacity = readOnlyBufferLength;
	FPosition = 0;

	FCanRead  = true;
	FCanWrite = false;
	FCanSeek  = true;
}

TMemoryStream::TMemoryStream(short capacity)
{
	FMemory = (char*) malloc(capacity);
	FMemoryOwned = true;
	FSize = 0;
	FCapacity = capacity;
	FPosition = 0;

	FCanRead = true;
	FCanWrite = true;
	FCanSeek = true;
}

TMemoryStream::~TMemoryStream()
{
    Close();
}

void TMemoryStream::Close()
{
	FSize = 0;
	if (FMemoryOwned)
	{
		if (FMemory) free(FMemory);
		FMemory = NULL;
		FSize = 0;
		FCapacity = 0;
	}
}


long TMemoryStream::ReadBuffer(void* Buffer, long Count)
{
    if (Count==0) return 0;
    if (FMemory==NULL) return 0;
    
    int Result = FSize - FPosition;
    if (Result > 0) 
    {
        if (Result > Count)
            Result = Count;

        char* p=FMemory+FPosition;
        memcpy(Buffer, p, Result);      
        FPosition+=Result;
        return Result;
    }
    return 0;
}

long TMemoryStream::WriteBuffer(void* Buffer, long Count)
{    
	if (FPosition<0) return 0;
	if (Count<1) return 0;

    int newSize = FPosition + Count;

	SetSize(newSize);
	if (FSize!=newSize)
	{
		return 0;
	}
    if (FMemory==NULL)
    {
        return 0;
    }

    char* p=FMemory+FPosition;
    memcpy(p,Buffer, Count);
	FPosition = newSize;
    return Count;
}

long TMemoryStream::Seek(long Offset, ESeekOrigin origin)
{
    switch (origin)
    {
		case esoFromBeginning:	FPosition  = Offset;  break;
		case esoFromCurrent:	FPosition += Offset;  break;
		case esoFromEnd:		FPosition  = FSize + Offset; break;
    }     
    return FPosition;
}

long TMemoryStream::SetSize(long newSize)
{
	if (!FMemoryOwned) 
	{
		if (newSize<FCapacity) 
		{
			FSize = newSize;
		} else {
			FSize = FCapacity;
		}
		return FSize;
	}

	if (newSize==0)
    {
        free(FMemory);
        FMemory = NULL;
		FSize = 0;
        FCapacity = 0;
        return 0;
    }

	if (newSize>FCapacity)
	{
		while(newSize>FCapacity)
		{
			if (FCapacity<512)
			{
				FCapacity+=64;
			} else {
				{
					FCapacity+=256;
				}
			}
		}
		if (FMemory!=NULL)
		{
			FMemory = (char*)realloc(FMemory, FCapacity);
		} else {
			FMemory =(char*) malloc(FCapacity);
			memset(FMemory, 0, FCapacity);
		}
		if (FMemory == NULL)
		{
			FSize = 0;
			FCapacity = 0;
		}
	}
	FSize = newSize;

	return FSize;
    
}

long TMemoryStream::GetPosition()
{
	return FPosition;
}

long TMemoryStream::GetSize()
{
	return FSize;
}

void* TMemoryStream::ToPtr()
{
	return FMemory;
}
