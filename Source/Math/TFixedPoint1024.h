/*
 * Persistence Library / Math / TFixedPoint1024
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

#ifndef TFIXEDPOINT1024___H
#define TFIXEDPOINT1024___H

#include "TFixedPointError.h"

//numbers -1024.0 to 1023.95, accuracy 0.05
class TFixedPoint1024
{
    private:
        short m_value; //16bit value -32768 to 32767 (represents -1024 to 1023.95)

    public:
        TFixedPoint1024();
        TFixedPoint1024(short value);
        TFixedPoint1024(short frac1, short frac2);

        TFixedPoint1024& operator = (short num);
        TFixedPoint1024& operator = (const TFixedPoint1024 value);

        TFixedPoint1024  operator + (TFixedPoint1024& value);
        TFixedPoint1024& operator +=(TFixedPoint1024& value);
        TFixedPoint1024  operator - (TFixedPoint1024& value);
        TFixedPoint1024& operator -=(TFixedPoint1024& value);
        TFixedPoint1024  operator * (TFixedPoint1024& value);
        TFixedPoint1024& operator *=(TFixedPoint1024& value);        
        TFixedPoint1024  operator / (TFixedPoint1024& value);
        TFixedPoint1024& operator /=(TFixedPoint1024& value);

        TFixedPoint1024  operator - ();
        TFixedPoint1024  operator + (short value);
        TFixedPoint1024& operator +=(short value);
        TFixedPoint1024  operator - (short value);
        TFixedPoint1024& operator -=(short value);
        TFixedPoint1024  operator * (short value);
        TFixedPoint1024& operator *=(short value);        
        TFixedPoint1024  operator / (short value);
        TFixedPoint1024& operator /=(short value);
        TFixedPoint1024  operator % (short value);

        bool operator == (short num);
        bool operator == (TFixedPoint1024& num);
        bool operator != (short num);
        bool operator != (TFixedPoint1024& num);
        bool operator >= (short num);
        bool operator >= (TFixedPoint1024& num);
        bool operator <= (short num);
        bool operator <= (TFixedPoint1024& num);
        bool operator >  (short num);
        bool operator >  (TFixedPoint1024& num);
        bool operator <  (short num);
        bool operator <  (TFixedPoint1024& num);

        signed char     Sgn();        
        short           Round();
        short           Trunc();
        TFixedPoint1024 Frac();
        TFixedPoint1024 Sqrt();
        TFixedPoint1024 Sqrt3();
        bool            IsZero();

        void            Print(char* pbOutputBuffer, unsigned short cbOutputBuffer);
        
};



#endif