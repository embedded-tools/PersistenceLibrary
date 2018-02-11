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

#include "TFixedPoint1024.h"

TFixedPoint1024::TFixedPoint1024()
    : m_value(0)
{

}

TFixedPoint1024::TFixedPoint1024(short value)
{
    if (value>1023)
    {
        m_value = FIXEDPOINT_INF;
    }
    if (value<-1023)
    {
        m_value = FIXEDPOINT_NEG_INF;
    }
    m_value = value<<5;
}

TFixedPoint1024::TFixedPoint1024(short frac1, short frac2)
{
    m_value = frac1*32/frac2;
}

TFixedPoint1024& TFixedPoint1024::operator = (short num)
{
    m_value = num << 5;
    return *this;
}

TFixedPoint1024& TFixedPoint1024::operator = (const TFixedPoint1024 value)
{
    m_value = value.m_value;
    return *this;
}

TFixedPoint1024 TFixedPoint1024::operator - ()
{
    TFixedPoint1024 result = *this;
    if (result.m_value==FIXEDPOINT_INF)
    {
        result.m_value=FIXEDPOINT_NEG_INF;
    } else 
    if (result.m_value==FIXEDPOINT_NEG_INF)
    {
        result.m_value=FIXEDPOINT_INF;
    } else {
        result.m_value = -result.m_value;
    }
    return result;
}

TFixedPoint1024  TFixedPoint1024::operator + (short value)
{
    TFixedPoint1024 result = *this;
    result += value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator +=(short value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    short value5 = value<<5;
    short result = m_value + value5;
    if (value5>0)
    {
        if (result<m_value) result=FIXEDPOINT_INF;
    } else {
        if (result>m_value) result=FIXEDPOINT_NEG_INF;
    }
    m_value=result;
    return *this;
}

TFixedPoint1024  TFixedPoint1024::operator + (TFixedPoint1024& value)
{
    TFixedPoint1024 result = *this;
    result += value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator +=(TFixedPoint1024& value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    short result = m_value + value.m_value;
    if (value.Round()>=0)
    {
        if (result<m_value) result=FIXEDPOINT_INF;
    } else {
        if (result>m_value) result=FIXEDPOINT_NEG_INF;
    }
    m_value = result;
    return *this;
}

TFixedPoint1024  TFixedPoint1024::operator - (short value)
{
    TFixedPoint1024 result = *this;
    result -= value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator -=(short value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    short value5 = value<<5;
    short result = m_value - value5;
    if (value5>0)
    {
        if (result>m_value) result=FIXEDPOINT_INF;
    } else {
        if (result<m_value) result=FIXEDPOINT_NEG_INF;
    }
    m_value=result;
    return *this;
}

TFixedPoint1024  TFixedPoint1024::operator - (TFixedPoint1024& value)
{
    TFixedPoint1024 result = *this;
    result -= value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator -=(TFixedPoint1024& value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    m_value-=value.m_value;
    if (value.Round()>=0)
    {
        if (m_value>value.m_value) m_value=FIXEDPOINT_NEG_INF;
    } else {
        if (m_value<value.m_value) m_value=FIXEDPOINT_INF;
    }
    return *this;
}


TFixedPoint1024  TFixedPoint1024::operator * (short value)
{
    TFixedPoint1024 result = *this;
    result *= value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator *=(short value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    long result = (m_value * value);
    if (result>=32767)
    {
        result = 32767;
    } else
    if (result<-32768)
    {
        result = -32768;
    }
    if (value>=1) 
    {
        if (((short)result)<m_value)
        {
            m_value = FIXEDPOINT_INF;
        }
        m_value = (short)result;
    } else {
        if (((short)result)>m_value)
        {
            m_value = FIXEDPOINT_NEG_INF;
        }
        m_value = (short)result;
    }
    return *this;
}

TFixedPoint1024  TFixedPoint1024::operator * (TFixedPoint1024& value)
{
    TFixedPoint1024 result = *this;
    result *= value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator *=(TFixedPoint1024& value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    long result = (m_value * value.m_value)>>5;
    if (result>=32767)
    {
        result = 32767;
    } else
    if (result<-32768)
    {
        result = -32768;
    }
    if (value.Round()>=1) //value>1.0
    {
        if (((short)result)<m_value)
        {
            m_value = FIXEDPOINT_INF;
        }
        m_value = (short)result;
    } else {
        if (((short)result)>m_value)
        {
            m_value = FIXEDPOINT_NEG_INF;
        }
        m_value = (short)result;
    }
    return *this;
}

TFixedPoint1024  TFixedPoint1024::operator / (short value)
{
    TFixedPoint1024 result = *this;
    result /= value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator /=(short value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    if (value==0)
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF;
        } else {
            m_value = FIXEDPOINT_NEG_INF;
        }
        return *this;        
    }
    //result = 32bit value
    long result = (m_value / value);
    if (result>=32767)
    {
        result = 32767;
    } else
        if (result<-32768)
        {
            result = -32768;
        }
        if (value>=1) //value>1.0
        {
            if (((short)result)<m_value)
            {
                m_value = FIXEDPOINT_INF;
            }
            m_value = (short)result;
        } else {
            if (((short)result)>m_value)
            {
                m_value = FIXEDPOINT_NEG_INF;
            }
            m_value = (short)result;
        }
        return *this;

}

TFixedPoint1024  TFixedPoint1024::operator / (TFixedPoint1024& value)
{
    TFixedPoint1024 result = *this;
    result /= value;
    return result;    
}

TFixedPoint1024& TFixedPoint1024::operator /=(TFixedPoint1024& value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    if (value.IsZero())
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF;
        } else {
            m_value = FIXEDPOINT_NEG_INF;
        }
        return *this;        
    }
    //result = 32bit value
    long result = (((long)m_value<<5) / value.m_value);
    if (result>=32767)
    {
        result = 32767;
    } else
    if (result<-32768)
    {
        result = -32768;
    }
    if (value.m_value>=32) //value>1.0
    {
        if (((short)result)<m_value)
        {
            m_value = FIXEDPOINT_INF;
        }
        m_value = (short)result;
    } else {
        if (((short)result)>m_value)
        {
            m_value = FIXEDPOINT_NEG_INF;
        }
        m_value = (short)result;
    }
    return *this;
}

TFixedPoint1024  TFixedPoint1024::operator % (short value)
{
    TFixedPoint1024 result =  *this;
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    if ((value>=1024) || (value<=-1024))
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF;
        } else {
            m_value = FIXEDPOINT_NEG_INF;
        }
        return *this;
    }
    result.m_value = result.m_value % (value<<5);
    return result;    
}

bool TFixedPoint1024::operator == (short num)
{
    if ((num<=-1024) || (num>=1024)) return false;
    return m_value == (num<<5);
}

bool TFixedPoint1024::operator == (TFixedPoint1024& num)
{
    return m_value == num.m_value;
}

bool TFixedPoint1024::operator != (short num)
{
    if ((num<=-1024) || (num>=1024)) return true;
    return m_value != (num<<5);
}

bool TFixedPoint1024::operator != (TFixedPoint1024& num)
{
    return m_value!=num.m_value;
}

bool TFixedPoint1024::operator >= (short num)
{
    if (num>=1024) return false;
    if (num<=-1024) return true;
    return m_value>=(num<<5);
}

bool TFixedPoint1024::operator >= (TFixedPoint1024& num)
{
    return m_value>=num.m_value;
}

bool TFixedPoint1024::operator <= (short num)
{
    if (num<=-1024) return false;
    if (num>= 1024) return true;
    return m_value<=(num<<5);
}

bool TFixedPoint1024::operator <= (TFixedPoint1024& num)
{
    return m_value<=num.m_value;
}

bool TFixedPoint1024::operator > (short num)
{
    if (num>= 1024) return false;
    if (num<=-1024) return true;
    return m_value>(num<<5);
}

bool TFixedPoint1024::operator > (TFixedPoint1024& num)
{
    return m_value>num.m_value;
}

bool TFixedPoint1024::operator < (short num)
{
    if (num<=-1024) return false;
    if (num>= 1024) return true;
    return m_value<(num<<5);
}

bool TFixedPoint1024::operator < (TFixedPoint1024& num)
{
    return m_value<num.m_value;
}

signed char     TFixedPoint1024::Sgn()
{
    if (m_value>0) return 1;
    if (m_value==0) return 0;        
    return -1;
}

short TFixedPoint1024::Round()
{
    short result = (m_value+16)/32;
    return result;
}

short TFixedPoint1024::Trunc()
{
    short result = m_value/32;
    return result;
}

TFixedPoint1024 TFixedPoint1024::Frac()
{
    TFixedPoint1024 result;
    result.m_value = m_value & (0x001F);
    return result;
}

TFixedPoint1024 TFixedPoint1024::Sqrt()
{
    TFixedPoint1024 res0 = 1;
    TFixedPoint1024 res  = 0;
    for(short i = 0; i<20; i++)
    {
        res = ((*this) / res0 + res0) / 2;
        if (res0==res) break;
        res0 = res;
    }
    return res;
}

TFixedPoint1024 TFixedPoint1024::Sqrt3()
{
    TFixedPoint1024 res0 = 1;
    TFixedPoint1024 res  = 0;
    for(short i = 0; i<20; i++)
    {
        res = ((*this) / res0 / res0 + res0 + res0) / 3;
        if (res0==res) break;
        res0 = res;
    }
    return res;
}

bool TFixedPoint1024::IsZero()
{
    return m_value==0;
}

void TFixedPoint1024::Print(char* pbOutputBuffer, unsigned short cbOutputBuffer)
{
    *pbOutputBuffer = 0;
    if (cbOutputBuffer<7)
    {
        return;
    }

    if (m_value==FIXEDPOINT_INF)
    {
        *pbOutputBuffer='+'; pbOutputBuffer++;
        *pbOutputBuffer='I'; pbOutputBuffer++;
        *pbOutputBuffer='N'; pbOutputBuffer++;
        *pbOutputBuffer='F'; pbOutputBuffer++;
        *pbOutputBuffer=0;
        return;
    }
    if (m_value==FIXEDPOINT_NEG_INF)
    {
        *pbOutputBuffer='-'; pbOutputBuffer++;
        *pbOutputBuffer='I'; pbOutputBuffer++;
        *pbOutputBuffer='N'; pbOutputBuffer++;
        *pbOutputBuffer='F'; pbOutputBuffer++;
        *pbOutputBuffer=0;
        return;
    }

    cbOutputBuffer=0;
    unsigned short n = 0;
    if (m_value>0)
    {
        n = m_value;
        n += 1;
    }  else 
    {
        *pbOutputBuffer='-';
        pbOutputBuffer++;
        cbOutputBuffer++;
        n  = -m_value;
        n += 1;
    }
    
    char c = 0;
    char zeroChar = 0;
    short exp = 1000<<5;

    //divides n by 1000, 100, 10 and 1
    //this way are calculates first four digits
    for(int i = 0; i<4; i++)
    {        
        c = '0';
        if (i==3)
        {
            zeroChar = '0';
        }
        while(n>=exp)
        {
            c++;
            n-=exp;
        }
        if (c=='0')
        {
            if (zeroChar!=0)
            {
                *pbOutputBuffer=zeroChar;
                pbOutputBuffer++;
                cbOutputBuffer++;
            }
        } else {
            *pbOutputBuffer=c;
            zeroChar = '0';
            pbOutputBuffer++;
            cbOutputBuffer++;
        }        
        exp/=10;
    }
    //inserts comma
    *pbOutputBuffer='.';
    pbOutputBuffer++;
    cbOutputBuffer++;

    //prints fraction digits
    exp=1<<5;
    for(int j = 0; j<1; j++)
    {
        c = '0';
        n*=10;        
        while(n>=exp)
        {
            c++;
            n-=exp;
        }
        *pbOutputBuffer=c;
        pbOutputBuffer++;
        cbOutputBuffer++;        
    }
    *pbOutputBuffer=0;
    pbOutputBuffer++;
    cbOutputBuffer++;        
}