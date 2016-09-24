/*
 * Persistence Library / FileUtils / TFileStream
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

#include "tfilestream.h"

TFileStream::TFileStream(const char* filename, EFileMode fileMode)
{
	switch(fileMode)
	{
		case efmCreate:			FHandle = fopen(filename,"wb");  FCanRead=false; FCanWrite=true;  FCanSeek=false; break;
		case efmOpenRead:		FHandle = fopen(filename,"rb");  FCanRead=true;  FCanWrite=false; FCanSeek=true;  break;
		case efmOpenReadWrite:  FHandle = fopen(filename,"rb+"); FCanRead=true;  FCanWrite=true;  FCanSeek=true;  break;
		case efmOpenWrite:		FHandle = fopen(filename,"wb");  FCanRead=false; FCanWrite=true;  FCanSeek=false; break;
		default:				FHandle = NULL;					 FCanRead=false; FCanWrite=false; FCanSeek=false; break;

    }
}
