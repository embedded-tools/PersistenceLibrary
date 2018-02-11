/*
 * Persistence Library / Math / TFixedPoint16M
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

#ifndef TFIXEDPOINT16M___H
#define TFIXEDPOINT16M___H

#include "TFixedPointError.h"

//numbers -16777216.0 to 16777215.95, accuracy 0.05
class TFixedPoint16M
{
    private:
        int m_value; //32bit value -16777216 to 16777215.95

    public:
        TFixedPoint16M();
        TFixedPoint16M(long value);
        TFixedPoint16M(long frac1, long frac2);

        TFixedPoint16M& operator = (long num);
        TFixedPoint16M& operator = (const TFixedPoint16M value);

        TFixedPoint16M  operator + (TFixedPoint16M& value);
        TFixedPoint16M& operator +=(TFixedPoint16M& value);
        TFixedPoint16M  operator - (TFixedPoint16M& value);
        TFixedPoint16M& operator -=(TFixedPoint16M& value);
        TFixedPoint16M  operator * (TFixedPoint16M& value);
        TFixedPoint16M& operator *=(TFixedPoint16M& value);        
        TFixedPoint16M  operator / (TFixedPoint16M& value);
        TFixedPoint16M& operator /=(TFixedPoint16M& value);

        TFixedPoint16M  operator - ();
        TFixedPoint16M  operator + (long value);
        TFixedPoint16M& operator +=(long value);
        TFixedPoint16M  operator - (long value);
        TFixedPoint16M& operator -=(long value);
        TFixedPoint16M  operator * (long value);
        TFixedPoint16M& operator *=(long value);        
        TFixedPoint16M  operator / (long value);
        TFixedPoint16M& operator /=(long value);
        TFixedPoint16M  operator % (long value);

        bool operator == (long num);
        bool operator == (TFixedPoint16M& num);
        bool operator != (long num);
        bool operator != (TFixedPoint16M& num);
        bool operator >= (long num);
        bool operator >= (TFixedPoint16M& num);
        bool operator <= (long num);
        bool operator <= (TFixedPoint16M& num);
        bool operator >  (long num);
        bool operator >  (TFixedPoint16M& num);
        bool operator <  (long num);
        bool operator <  (TFixedPoint16M& num);

        signed char    Sgn();        
        long           Round();
        long           Trunc();
        TFixedPoint16M Frac();
        TFixedPoint16M Sqrt();
        TFixedPoint16M Sqrt3();
        bool           IsZero();

        void            Print(char* pbOutputBuffer, unsigned long cbOutputBuffer);
        
};



#endif