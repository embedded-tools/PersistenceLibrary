/*
 * Persistence Library / FileUtils / TLZ77
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

#ifndef TLZ77___H
#define TLZ77___H

enum LZ77Error
{
    lzeSuccess,
    lzeOutputBufferNotBigEnough,
    lzeOffsetTooHigh,
    lzeDataCorrupted,
    lzeUnknownError    
};

LZ77Error LZ77_Compress   (unsigned char* pbDataToCompress, unsigned long cbDataToCompress, unsigned char* pbOutputBuffer, unsigned long& cbOutputBuffer);
LZ77Error LZ77_Decompress (unsigned char* pbDataToDecompress, unsigned long cbDataToDecompress, unsigned char* pbOutputBuffer, unsigned long& cbOutputBuffer);


#endif
