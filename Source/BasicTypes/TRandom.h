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

#ifndef TRANDOM___H
#define TRANDOM___H

#define TRANDOM_ARG_1 75155
#define TRANDOM_ARG_2 547898677

/************************************************************************/
/* Very simple class for generating random numbers similar to standard
/* function "rand()" in <stdlib>
/*
/* You can use this class if you can't afford to include <stdlib> because 
/* of small ROM
/*
/************************************************************************/
class TRandom
{
    private:
        static unsigned long m_seed;

    public:

        static void Randomize(unsigned long newSeed);
        static unsigned long GetRandomNumber(unsigned long max=4294967295U);   
		static void GetRandomByteArray(unsigned char* pArray, unsigned long arrayLength);
        static bool GetRandomNumberArray(unsigned long maxNumber, unsigned long* pOutputBuffer, short numberCount, bool uniqueNumbers=true, bool zeroIncluded=false);
};

#endif
