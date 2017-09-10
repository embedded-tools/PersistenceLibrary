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

#ifndef TMEMORYSTREAM___H
#define TMEMORYSTREAM___H

#include "tstream.h"

class TMemoryStream:public TStream
{
private:
	char* m_memory;
	bool  m_memoryOwner;

	long  m_size, m_capacity;
	long  m_position;    

	long  SetSize(long size);

public:

	TMemoryStream(void* rewriteableBuffer, short rewriteableBufferLength);
	TMemoryStream(const char* readOnlyBuffer, short readOnlyBufferLength);
    TMemoryStream(short capacity=64);
    virtual ~TMemoryStream();

    virtual void Close();
	virtual long ReadBuffer  (void* Buffer, long Count);
    virtual long WriteBuffer (void* Buffer, long Count);
    virtual long Seek (long Offset, ESeekOrigin Origin);
	virtual long GetPosition();
	virtual long GetSize();
 
	void* ToPtr();
};


#endif
