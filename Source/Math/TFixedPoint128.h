/*
 * Persistence Library / Math / TFixedPoint128
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

#ifndef TFIXEDPOINT128___H
#define TFIXEDPOINT128___H

#include "TFixedPointError.h"

//numbers -128.0 to 127.995, accuracy 0.005
class TFixedPoint128
{
private:
    short m_value; //16bit value -32768 to 32767 (represents -128 to 1023.95)

public:
    TFixedPoint128();
    TFixedPoint128(short value);
    TFixedPoint128(short frac1, short frac2);

    TFixedPoint128& operator = (short num);
    TFixedPoint128& operator = (const TFixedPoint128 value);

    TFixedPoint128  operator + (TFixedPoint128& value);
    TFixedPoint128& operator +=(TFixedPoint128& value);
    TFixedPoint128  operator - (TFixedPoint128& value);
    TFixedPoint128& operator -=(TFixedPoint128& value);
    TFixedPoint128  operator * (TFixedPoint128& value);
    TFixedPoint128& operator *=(TFixedPoint128& value);        
    TFixedPoint128  operator / (TFixedPoint128& value);
    TFixedPoint128& operator /=(TFixedPoint128& value);

    TFixedPoint128  operator - ();
    TFixedPoint128  operator + (short value);
    TFixedPoint128& operator +=(short value);
    TFixedPoint128  operator - (short value);
    TFixedPoint128& operator -=(short value);
    TFixedPoint128  operator * (short value);
    TFixedPoint128& operator *=(short value);        
    TFixedPoint128  operator / (short value);
    TFixedPoint128& operator /=(short value);
    TFixedPoint128  operator % (short value);

    bool operator == (short num);
    bool operator == (TFixedPoint128& num);
    bool operator != (short num);
    bool operator != (TFixedPoint128& num);
    bool operator >= (short num);
    bool operator >= (TFixedPoint128& num);
    bool operator <= (short num);
    bool operator <= (TFixedPoint128& num);
    bool operator >  (short num);
    bool operator >  (TFixedPoint128& num);
    bool operator <  (short num);
    bool operator <  (TFixedPoint128& num);

    signed char     Sgn();        
    short           Round();
    short           Trunc();
    TFixedPoint128 Frac();
    TFixedPoint128 Sqrt();
    TFixedPoint128 Sqrt3();
    bool            IsZero();

    void            Print(char* pbOutputBuffer, unsigned short cbOutputBuffer);

};



#endif