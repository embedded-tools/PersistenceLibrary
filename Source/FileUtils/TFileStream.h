/*
 * Persistence Library / FileUtils / TFileStream
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

#ifndef TFILESTREAM___H
#define TFILESTREAM___H

#include "THandleStream.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum EFileMode
{	
	efmOpenRead=0x0000,
	efmOpenWrite=0x0001,
	efmOpenReadWrite=0x0002,
	efmCreate=0xFFFF
} EFILEMODE;

/**
 * File stream can be used for reading / writing data from file.
 * Filename must be used as a constructor argument.
 *
 * FileStream can be combined with CachedStream if you want to decrease
 * number of readings from file, or with LZ77Streamed if you want
 * to compress/decompress data.
 */
class TFileStream : public THandleStream
{
public:
	
	TFileStream(const char* filename, EFileMode fileMode);

};


#endif
