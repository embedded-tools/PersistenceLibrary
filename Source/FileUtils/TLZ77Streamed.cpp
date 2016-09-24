/*
 * Persistence Library / FileUtils / TLZ77Streamed
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

#include "TLZ77Streamed.h"

TLZ77Streamed::TLZ77Streamed(TStream* mainStream, void* slidingWindowAndCache, int cacheSize, int slidingWindowSize)
    :TCachedStream(mainStream, 0)
{
    FSlidingWindow = (unsigned char*)slidingWindowAndCache;
    FSlidingWindowSize = 0;
    FSlidingWindowMaxSize = slidingWindowSize;
    FCache = FSlidingWindow + slidingWindowSize;
    FCachePosition = 0;	
    FCacheSize = 0;
    FCacheMaxSize = cacheSize;
    FCacheOwned = false;
    FReadingBlockOffset = 0;
    FReadingBlockLength = 0;
}

TLZ77Streamed::TLZ77Streamed(TStream* mainStream, int cacheSize, int slidingWindowSize)
    :TCachedStream(mainStream, 0)
{
    FSlidingWindow = (unsigned char*)malloc(slidingWindowSize+cacheSize);
    FSlidingWindowSize = 0;
    FSlidingWindowMaxSize = slidingWindowSize;
    FCache = FSlidingWindow + slidingWindowSize;
    FCachePosition = 0;	
    FCacheSize = 0;
    FCacheMaxSize = cacheSize;
    FCacheOwned = true;
    FReadingBlockOffset = 0;
    FReadingBlockLength = 0;
}
 
void TLZ77Streamed::MoveSlidingWindow()
{
    if ((FCacheSize==0) || (FSlidingWindowMaxSize==0))
    {
        return;
    }
    memcpy(FSlidingWindow, FSlidingWindow+FCacheSize, FSlidingWindowMaxSize);

    FSlidingWindowSize+=FCacheSize;
    if (FSlidingWindowSize>FSlidingWindowMaxSize)
    {
        FSlidingWindowSize = FSlidingWindowMaxSize;
    }
    FPosition += FCacheSize;
    FCacheSize = 0;
    FCachePosition = 0;
}

void TLZ77Streamed::StorePattern(short offset, short length)
{
    FParentStream->WriteByte(LZ77_MAGIC_BYTE);
    FParentStream->WriteByte((unsigned char)length);
    FParentStream->WriteByte((unsigned char)(length>>8));
    FParentStream->WriteByte((unsigned char)offset);
    FParentStream->WriteByte((unsigned char)(offset>>8));
}

void TLZ77Streamed::StoreByte(unsigned char b)
{
    FParentStream->WriteByte(b);
    if (b==LZ77_MAGIC_BYTE)
    {        
        WriteByte(0x00);
        WriteByte(0x00);
    }
}


bool TLZ77Streamed::DoWriteOperation()
{
    if (FParentStream==NULL)
    {
        return false;
    }
    unsigned char* srcA = FCache;
    unsigned char* srcB = FCache;    

    unsigned char a0 = srcA[0];
    unsigned char a1 = srcA[1];
    unsigned char a2 = srcA[2];
    unsigned char a3 = srcA[3];    

    unsigned char b0 = srcB[0];
    unsigned char b1 = srcB[1];
    unsigned char b2 = srcB[2];
    unsigned char b3 = srcB[3];

    unsigned short currentPattern_Length = 0;
    unsigned short longestPattern_Offset = 0;
    unsigned short longestPattern_Length = 0;

    short position = 0;
    while(position<FCacheSize)
    {
        bool found = false;

        srcB = srcA;
        char b0 = srcB[0];
        char b1 = srcB[1];
        char b2 = srcB[2];
        char b3 = srcB[3];

        int maxOffSet = FCacheSize + FSlidingWindowSize;
        if (maxOffSet>=FSlidingWindowMaxSize) maxOffSet = FSlidingWindowMaxSize;

        for(short offset = 1; offset <= maxOffSet; offset++)
        {
            srcB--;
            b3 = b2;
            b2 = b1;
            b1 = b0;
            b0 = srcB[0];

//performance optimization - very fast way of comparing first four bytes
            if (a0==b0)
            {
                if (a1==b1)
                {
                    if (a2==b2)
                    {
                        if (a3==b3)
                        {
                            unsigned char* srcAA = srcA + 4;
                            unsigned char* srcBB = srcB + 4;
                            currentPattern_Length = 4;
//end of performance optimization
                            for(int search = 4; search<(FCacheSize-position); search++)
                            {
                                if (*srcAA!=*srcBB)
                                {                    
                                    break;
                                }
                                currentPattern_Length++;
                                srcAA++;
                                srcBB++;
                            }
                            if (currentPattern_Length>longestPattern_Length)
                            {
                                longestPattern_Length = currentPattern_Length;
                                longestPattern_Offset = offset;
                            }
                            if ((offset==1) && (currentPattern_Length>5))
                            {
                                //performance optimization for long sequences of the same byte
                                //(e.g. 32, 32, 32, 32, 32, 32, 32, 32, 32... )
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (longestPattern_Length>0)
        {            
            bool useEscSeq = longestPattern_Length>=6;
            if ((longestPattern_Offset==1) && (b0==LZ77_MAGIC_BYTE))
            {
                useEscSeq = true;
            }
            if (useEscSeq)
            {
                StorePattern(longestPattern_Offset, longestPattern_Length); 
            } else {
                srcB = srcA - longestPattern_Offset;
                for(int c = 0; c<longestPattern_Length; c++)
                {                    
                    StoreByte(*srcB); srcB++;
                }                
            }            

            srcA+=longestPattern_Length;
            position+=longestPattern_Length;

            longestPattern_Length = 0;

            //also part of performance optimization
            a0 = srcA[0];
            a1 = srcA[1];
            a2 = srcA[2];
            a3  = srcA[3];
        } else {
            StoreByte(a0);

            srcA++;
            position++;

            //also part of performance optimization
            a0 = a1;
            a1 = a2;
            a2 = a3;
            a3  = srcA[3];
        }
    }
    MoveSlidingWindow();

    FCachePosition =  0;
    FCacheSize = 0;
    return true;
}

bool TLZ77Streamed::DoReadOperation()
{
    unsigned char c; 

    MoveSlidingWindow();
    
    while(FCacheSize<FCacheMaxSize-5)
    {
        if (FReadingBlockLength!=0)
        {
            FCache[FCacheSize] = FCache[FCacheSize-FReadingBlockOffset];
            FReadingBlockLength--;
            FCacheSize++;
            continue;
        }
        bool res = FParentStream->ReadByte(c);
        if (!res) return false;

        if (c==LZ77_MAGIC_BYTE)
        {
            res = FParentStream->ReadWord(FReadingBlockLength);
            if (!res) 
            {
                return false;
            }

            if (FReadingBlockLength==0)
            {
                FCache[FCacheSize] = c;
                FCacheSize++;
            } else {
                res = FParentStream->ReadWord(FReadingBlockOffset);
                if (!res) 
                {
                    return false;
                }
                if ( (FReadingBlockOffset>FSlidingWindowMaxSize) || (FReadingBlockOffset<0) )                      
                {
                    return false;
                }
            }
        } else {
            FCache[FCacheSize] = c;
            FCacheSize++;
        }            
    }    
    return true;

}
