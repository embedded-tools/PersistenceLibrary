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
    unsigned long value = 0;

    m_seed = m_seed * TRANDOM_ARG_1 + TRANDOM_ARG_2;  
    if (max < 100000)
    {
		if (max != 0)
		{
          unsigned long divisor = (4294967295U / max) + 1;
          value = m_seed / divisor;
		}
    } else {
        if (max<4294967295U)
        {
            value = m_seed % max;
        } 
		else 
		{
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

bool TRandom::GetRandomNumberArray(unsigned long maxNumber, unsigned long* pOutputBuffer, short numberCount, bool uniqueNumbers, bool zeroIncluded)
{
	bool result = true;

	//all required numbers are generated
    for (short i = 0; i < numberCount; i++)
    {
        unsigned long n = 0;

		bool uniqueNumberFound = false;

		//repeatedly generates random number 
		//until it is unique
        for(unsigned short max = 0; max<65535; max++)
        {                    
            if (zeroIncluded)
            {
                n = GetRandomNumber(maxNumber + 1);
            }
            else
            {
                n = GetRandomNumber(maxNumber) + 1;
            }
			pOutputBuffer[i] = n;

            if (uniqueNumbers)
            {
                bool isUnique = true;
                for (short j = 0; j < i; j++)
                {
					if (n == pOutputBuffer[j])
                    {
                        isUnique = false;
                    }
                }
                if (isUnique) 
				{
					//new number is unique
					uniqueNumberFound = true;
					break;
				}
            }
            else
            {
				//unique numbers are not required
				//therefore jumps out of for max loop
				uniqueNumberFound = true;
                break;
            }
        } //for max
		if (!uniqueNumberFound)
		{
			result = false;
		}
    }//for i
	return result;
}
