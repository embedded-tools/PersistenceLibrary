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

#ifndef TRANDOM___H
#define TRANDOM___H

#define TRANDOM_PRIME_1 696132157
#define TRANDOM_PRIME_2 791941417

class TRandom
{
    private:
        static unsigned long m_seed;

    public:

        static void Randomize(unsigned long newSeed);
        static unsigned long GetRandomNumber(unsigned long max=4294967295);            
        static void GetRandomByteArray(unsigned char* pArray, unsigned long arrayLength);
};

#endif