/*
 * Persistence Library / FileUtils / TCachedStream
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

#ifndef TCACHEDSTREAM___H
#define TCACHEDSTREAM___H

#include "tstream.h"

class TCachedStream: public TStream
{
protected:
	unsigned char* FCache;
	bool           FCacheOwned;
	int            FCachePosition;	
	int            FCacheSize;
	int            FCacheMaxSize;
	int            FPosition;

	virtual bool DoWriteOperation();
	virtual bool DoReadOperation();

public:

	void FlushCache();

	TCachedStream(TStream* mainStream, void* cache, int cacheSize);
	TCachedStream(TStream* mainStream, int cacheSize=512);
	virtual ~TCachedStream();

    virtual long ReadBuffer   (void* Buffer, long Count);
    virtual long WriteBuffer  (void* Buffer, long Count);
    virtual long Seek (long Offset, ESeekOrigin Origin);
	virtual long GetPosition();
	virtual long GetSize();
	virtual void Close();
};

#endif
