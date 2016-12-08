/*
 * Persistence Library / Basic types / TRandom 
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

#include "TRandom.h"

unsigned long TRandom::m_seed = 0;

void TRandom::Randomize(unsigned long newSeed)
{
    m_seed = newSeed;
}

unsigned long TRandom::GetRandomNumber(unsigned long max)
{
    unsigned long value;

    m_seed = m_seed * TRANDOM_PRIME_1 + TRANDOM_PRIME_2;
    
    if (max<=0x100)
    {
        value = m_seed>>23;
    } else 
    if (max <= 0x10000)
    {
        value = m_seed>>15;
    } else 
    if (max <= 0x1000000)
    {
        value = m_seed>>7;
    } else {
        value = m_seed;
    }
    return (m_seed % max);
}

void TRandom::GetRandomByteArray(unsigned char* pArray, unsigned long arrayLength)
{
    for(unsigned long i = 0; i<arrayLength; i++)
    {
        pArray[i] = (unsigned char)GetRandomNumber(256);
    }
}
