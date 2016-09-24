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
    m_seed = m_seed * TRANDOM_PRIME_1 + TRANDOM_PRIME_2;

    if (max<2147483647)
    {
        return (m_seed % max);
    }
    return m_seed;
}
