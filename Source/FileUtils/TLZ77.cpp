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

#include "TLZ77.h"

LZ77Error LZ77_Compress  (unsigned char* pbDataToCompress, unsigned long cbDataToCompress, unsigned char* pbOutputBuffer, unsigned long& cbOutputBuffer)
{
    unsigned char* srcA   = pbDataToCompress;
    unsigned char* srcB   = pbDataToCompress;
    unsigned char* dst    = pbOutputBuffer;
    unsigned long  dstCnt = 0;

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

    unsigned long position = 0;
    while(position<cbDataToCompress)
    {
        bool found = false;

        srcB = srcA;
        char b0 = srcB[0];
        char b1 = srcB[1];
        char b2 = srcB[2];
        char b3 = srcB[3];
         
        int maxOffset = position;
        if (maxOffset>4096) maxOffset = 4096;

        for(short offset = 1; offset <= maxOffset; offset++)
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
                            for(unsigned long search = 4; search<(cbDataToCompress-position); search++)
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
            if ((longestPattern_Offset==1) && (b0==0xBF))
            {
                useEscSeq = true;
            }
            if (useEscSeq)
            {                
                *dst = 0xBF;
                dst++; dstCnt++;
                if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;

                *dst = (unsigned char)longestPattern_Length;
                dst++; dstCnt++;
                if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;

                *dst = (unsigned char)(longestPattern_Length>>8);
                dst++; dstCnt++;
                if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;

                *dst = (unsigned char)(longestPattern_Offset);
                dst++; dstCnt++;
                if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;

                *dst = (unsigned char)(longestPattern_Offset>>8);
                dst++; dstCnt++;
                if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;

                               
            } else {
                srcB = srcA - longestPattern_Offset;
                for(int c = 0; c<longestPattern_Length; c++)
                {                    
                    *dst = *srcB; srcB++;
                    dst++; dstCnt++;
                    if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;
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
            *dst = a0; srcB++;
            dst++; dstCnt++;
            if (dstCnt>=cbOutputBuffer) return lzeOutputBufferNotBigEnough;

            srcA++;
            position++;

            //also part of performance optimization
            a0 = a1;
            a1 = a2;
            a2 = a3;
            a3 = srcA[3];
        }
    }
    cbOutputBuffer = dstCnt;
    return lzeSuccess;
}

LZ77Error LZ77_Decompress(unsigned char* pbDataToDecompress, unsigned long cbDataToDecompress, unsigned char* pbOutputBuffer, unsigned long& cbOutputBuffer)
{
    unsigned char c; 

    unsigned char* srcData = pbDataToDecompress;
    unsigned char* dstData = pbOutputBuffer;
    unsigned long  dstDataSize = 0;

    unsigned long position = 0;
    while(position<cbDataToDecompress)
    {
        c = *srcData;
        if (c==0xBF)
        {
            srcData++;
            short length = *srcData;            
            srcData++;            
            length += (*srcData)<<8;

            short offset = 0;
            if (length==0)
            {
                if (dstDataSize>=cbOutputBuffer)
                {
                    return lzeOutputBufferNotBigEnough;
                }
                *dstData = c;
                dstData++;
                dstDataSize++;
                position+=3;                

            } else {
                if (dstDataSize>=cbOutputBuffer)
                {
                    return lzeOutputBufferNotBigEnough;
                }
                srcData++;
                offset = *srcData;
                srcData++;
                offset += (*srcData)<<8;
                srcData++;     
                if (offset<0)
                {
                    return lzeDataCorrupted;
                }
                if ( offset > 4096)
                {
                    return lzeOffsetTooHigh;
                }
                unsigned char* prevData = dstData - offset;
                if (dstDataSize+length>cbOutputBuffer)
                {
                    return lzeOutputBufferNotBigEnough;
                }
                for(short c = 0; c<length; c++)
                {
                    *dstData = *prevData;
                    dstData++;
                    prevData++;
                    dstDataSize++;
                }
                position+=5;
            }

        } else {
            if (dstDataSize>=cbOutputBuffer)
            {
                return lzeOutputBufferNotBigEnough;
            }
            *dstData = *srcData;
            dstData++;
            srcData++;
            position++;
            dstDataSize++;
        }            
    }    
    cbOutputBuffer = dstDataSize;
    return lzeSuccess;
}

