/*
 * Persistence Library / Basic types / TRandom 
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

#include "TRandom.h"

unsigned long TRandom::m_seed = 0;

void TRandom::Randomize(unsigned long newSeed)
{
    m_seed = newSeed;
}

unsigned long TRandom::GetRandomNumber(unsigned long max)
{
    unsigned long value;

    m_seed = m_seed * TRANDOM_ARG_1 + TRANDOM_ARG_2;  
    if (max < 100000)
    {
        unsigned long divisor = (4294967295U / max) + 1;
        value = m_seed / divisor;
    } else {
        if (max<4294967295U)
        {
            value = m_seed % max;
        } else {
            value = m_seed;
        }        
    }
    return value;
}

void TRandom::GetRandomByteArray(unsigned char* pArray, unsigned long arrayLength)
{
    for(unsigned long i = 0; i<arrayLength; i++)
    {
        pArray[i] = (unsigned char)GetRandomNumber(256);
    }
}
