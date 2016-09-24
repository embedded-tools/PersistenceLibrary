/*
 * Persistence Library / FileUtils / THandleStream
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

#include "thandlestream.h"

/////////////////////////////
//      THandleStream
/////////////////////////////
THandleStream::THandleStream(FILE* AHandle)
{
    FHandle = AHandle;
	FSize   = 0;
}

THandleStream::THandleStream()
{
	FSize   = 0;
}


THandleStream::~THandleStream()
{
    Close();
}

void THandleStream::Close()
{
    if (FHandle!=NULL)
    {
        fclose(FHandle);
        FHandle=NULL;
    }
}

long THandleStream::ReadBuffer(void* Buffer, long Count)
{
    unsigned short result = (unsigned short)fread(Buffer,1,Count,FHandle);
    return result;
}

long THandleStream::WriteBuffer(void* Buffer, long Count)
{
    unsigned short result = (unsigned short)fwrite(Buffer,1,Count,FHandle);
    return result;
}

long THandleStream::Seek(long Offset, ESeekOrigin origin)
{
    fseek(FHandle, Offset, origin);
	int position = ftell(FHandle);
	return position;
}

long THandleStream::GetPosition()
{
	int position = ftell(FHandle);
	return position;
}

long THandleStream::GetSize()
{
	if ((FSize==0) || (FCanWrite))
	{
		int pos  = ftell(FHandle);
		fseek(FHandle,0,esoFromEnd);
		int size = ftell(FHandle);
		fseek(FHandle,pos, esoFromBeginning);
		FSize = size;
	}	
	return FSize;
}

