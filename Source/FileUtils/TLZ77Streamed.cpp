/*
 * Persistence Library / FileUtils / TLZ77Streamed
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

#include "TLZ77Streamed.h"

TLZ77Streamed::TLZ77Streamed(TStream* mainStream, void* slidingWindowAndCache, int cacheSize, int slidingWindowSize)
    :TCachedStream(mainStream, 0)
{
    m_slidingWindow = (unsigned char*)slidingWindowAndCache;
    m_slidingWindowSize = 0;
    m_slidingWindowMaxSize = slidingWindowSize;
    m_cache = m_slidingWindow + slidingWindowSize;
    m_cachePosition = 0;	
    m_cacheSize = 0;
    m_cacheMaxSize = cacheSize;
    m_cacheOwned = false;
    m_readingBlockOffset = 0;
    m_readingBlockLength = 0;
}

TLZ77Streamed::TLZ77Streamed(TStream* mainStream, int cacheSize, int slidingWindowSize)
    :TCachedStream(mainStream, 0)
{
    m_slidingWindow = (unsigned char*)malloc(slidingWindowSize+cacheSize);
    m_slidingWindowSize = 0;
    m_slidingWindowMaxSize = slidingWindowSize;
    m_cache = m_slidingWindow + slidingWindowSize;
    m_cachePosition = 0;	
    m_cacheSize = 0;
    m_cacheMaxSize = cacheSize;
    m_cacheOwned = true;
    m_readingBlockOffset = 0;
    m_readingBlockLength = 0;
}

TLZ77Streamed::~TLZ77Streamed()
{
    Close();
    if (m_cacheOwned)
    {
        if (m_slidingWindow)
        {
            free(m_slidingWindow);
        }        
    }
    m_slidingWindow = NULL;
    m_cache=NULL;
    m_cacheOwned = false;
    m_cachePosition = 0;
    m_cacheSize = 0;
    m_cacheMaxSize = 0;
}

long TLZ77Streamed::GetSize()
{
    if (m_cachePosition==m_cacheSize)
    {
        DoReadOperation();
    }    
    return TCachedStream::GetSize();
}
 
void TLZ77Streamed::MoveSlidingWindow()
{
    if ((m_cacheSize==0) || (m_slidingWindowMaxSize==0))
    {
        return;
    }
    memcpy(m_slidingWindow, m_slidingWindow+m_cacheSize, m_slidingWindowMaxSize);

    m_slidingWindowSize+=m_cacheSize;
    if (m_slidingWindowSize>m_slidingWindowMaxSize)
    {
        m_slidingWindowSize = m_slidingWindowMaxSize;
    }
    m_position += m_cacheSize;
    m_cacheSize = 0;
    m_cachePosition = 0;
}

void TLZ77Streamed::StorePattern(short offset, short length)
{
    m_parentStream->WriteByte(LZ77_MAGIC_BYTE);
    m_parentStream->WriteByte((unsigned char)length);
    m_parentStream->WriteByte((unsigned char)(length>>8));
    m_parentStream->WriteByte((unsigned char)offset);
    m_parentStream->WriteByte((unsigned char)(offset>>8));
}

void TLZ77Streamed::StoreByte(unsigned char b)
{
    m_parentStream->WriteByte(b);
    if (b==LZ77_MAGIC_BYTE)
    {        
        m_parentStream->WriteByte(0x00);
        m_parentStream->WriteByte(0x00);
    }
}


bool TLZ77Streamed::DoWriteOperation()
{
    if (m_parentStream==NULL)
    {
        return false;
    }
    unsigned char* srcA = m_cache;
    unsigned char* srcB = m_cache;    

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
    while(position<m_cacheSize)
    {
        bool found = false;

        srcB = srcA;
        unsigned char b0 = srcB[0];
        unsigned char b1 = srcB[1];
        unsigned char b2 = srcB[2];
        unsigned char b3 = srcB[3];

        int maxOffset = position + m_slidingWindowSize;
        if (maxOffset>m_slidingWindowMaxSize) maxOffset = m_slidingWindowMaxSize;
        for(short offset = 1; offset<=maxOffset; offset++)
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
                            for(int search = 4; search<(m_cacheSize-position); search++)
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
                            if ((offset==1) && (currentPattern_Length>=8))
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
            if (b0==LZ77_MAGIC_BYTE)
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
            a3 = srcA[3];
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

    m_cachePosition =  0;
    m_cacheSize = 0;
    return true;
}

bool TLZ77Streamed::DoReadOperation()
{
    unsigned char c; 

    MoveSlidingWindow();
    
    while(m_cacheSize<m_cacheMaxSize-5)
    {
        if (m_readingBlockLength!=0)
        {
            m_cache[m_cacheSize] = m_cache[m_cacheSize-m_readingBlockOffset];
            m_readingBlockLength--;
            m_cacheSize++;
            continue;
        }
        bool res = m_parentStream->ReadByte(c);
        if (!res) return false;

        if (c==LZ77_MAGIC_BYTE)
        {
            res = m_parentStream->ReadWord(m_readingBlockLength);
            if (!res) 
            {
                return false;
            }

            if (m_readingBlockLength==0)
            {
                m_cache[m_cacheSize] = c;
                m_cacheSize++;
            } else {
                res = m_parentStream->ReadWord(m_readingBlockOffset);
                if (!res) 
                {
                    return false;
                }
                if ( (m_readingBlockOffset>m_slidingWindowMaxSize) || (m_readingBlockOffset<0) )                      
                {
                    return false;
                }
            }
        } else {
            m_cache[m_cacheSize] = c;
            m_cacheSize++;
        }            
    }    
    return true;

}
