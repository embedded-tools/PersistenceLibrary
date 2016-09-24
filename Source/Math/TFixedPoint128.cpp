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

#include "TFixedPoint128.h"

TFixedPoint128::TFixedPoint128()
    : m_value(0)
{

}

TFixedPoint128::TFixedPoint128(short value)
{
    if (value>127)
    {
        m_value = FIXEDPOINT_INF;
    }
    if (value<-127)
    {
        m_value = FIXEDPOINT_NEG_INF;
    }
    m_value = value<<8;
}

TFixedPoint128::TFixedPoint128(short frac1, short frac2)
{
    m_value = (frac1<<8)/frac2;
}

TFixedPoint128& TFixedPoint128::operator = (short num)
{
    m_value = num << 8;
    return *this;
}

TFixedPoint128& TFixedPoint128::operator = (TFixedPoint128& value)
{
    m_value = value.m_value;
    return *this;
}

TFixedPoint128 TFixedPoint128::operator - ()
{
    TFixedPoint128 result = *this;
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

TFixedPoint128  TFixedPoint128::operator + (short value)
{
    TFixedPoint128 result = *this;
    result += value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator +=(short value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    short value5 = value<<8;
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

TFixedPoint128  TFixedPoint128::operator + (TFixedPoint128& value)
{
    TFixedPoint128 result = *this;
    result += value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator +=(TFixedPoint128& value)
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

TFixedPoint128  TFixedPoint128::operator - (short value)
{
    TFixedPoint128 result = *this;
    result -= value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator -=(short value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    short value5 = value<<8;
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

TFixedPoint128  TFixedPoint128::operator - (TFixedPoint128& value)
{
    TFixedPoint128 result = *this;
    result -= value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator -=(TFixedPoint128& value)
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


TFixedPoint128  TFixedPoint128::operator * (short value)
{
    TFixedPoint128 result = *this;
    result *= value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator *=(short value)
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

TFixedPoint128  TFixedPoint128::operator * (TFixedPoint128& value)
{
    TFixedPoint128 result = *this;
    result *= value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator *=(TFixedPoint128& value)
{
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    long result = (m_value * value.m_value)>>8;
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

TFixedPoint128  TFixedPoint128::operator / (short value)
{
    TFixedPoint128 result = *this;
    result /= value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator /=(short value)
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

TFixedPoint128  TFixedPoint128::operator / (TFixedPoint128& value)
{
    TFixedPoint128 result = *this;
    result /= value;
    return result;    
}

TFixedPoint128& TFixedPoint128::operator /=(TFixedPoint128& value)
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
    long result = (((long)m_value<<8) / value.m_value);
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

TFixedPoint128  TFixedPoint128::operator % (short value)
{
    TFixedPoint128 result =  *this;
    if ((m_value==FIXEDPOINT_INF) || (m_value==FIXEDPOINT_NEG_INF))
    {
        return *this;
    }
    if ((value>=128) || (value<=-128))
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF;
        } else {
            m_value = FIXEDPOINT_NEG_INF;
        }
        return *this;
    }
    result.m_value = result.m_value % (value<<8);
    return result;    
}

bool TFixedPoint128::operator == (short num)
{
    if ((num<=-128) || (num>=128)) return false;
    return m_value==(num<<8);
}

bool TFixedPoint128::operator == (TFixedPoint128& num)
{
    return m_value == num.m_value;
}

bool TFixedPoint128::operator != (short num)
{
    if ((num<=-128) || (num>=128)) return true;
    return m_value!=(num<<8);
}

bool TFixedPoint128::operator != (TFixedPoint128& num)
{
    return m_value!=num.m_value;
}

bool TFixedPoint128::operator >= (short num)
{
    if (num>= 128) return false;
    if (num<=-128) return true;
    return m_value>=(num<<8);
}

bool TFixedPoint128::operator >= (TFixedPoint128& num)
{
    return m_value>=num.m_value;
}

bool TFixedPoint128::operator <= (short num)
{
    if (num>=128)  return true;
    if (num<=-128) return false;
    return m_value<=(num<<8);
}

bool TFixedPoint128::operator <= (TFixedPoint128& num)
{
    return m_value<=num.m_value;
}

bool TFixedPoint128::operator > (short num)
{
    if (num>= 128) return false;
    if (num<=-128) return true;
    return m_value>(num<<8);
}

bool TFixedPoint128::operator > (TFixedPoint128& num)
{
    return m_value>num.m_value;
}

bool TFixedPoint128::operator < (short num)
{
    if (num<=-128) return false;
    if (num>= 128) return true;
    return m_value<(num<<8);
}

bool TFixedPoint128::operator < (TFixedPoint128& num)
{
    return m_value<num.m_value;
}

signed char     TFixedPoint128::Sgn()
{
    if (m_value>0) return 1;
    if (m_value==0) return 0;        
    return -1;
}

short TFixedPoint128::Round()
{
    short result = (m_value+128)/256;
    return result;
}

short TFixedPoint128::Trunc()
{
    short result = m_value/256;
    return result;
}

TFixedPoint128 TFixedPoint128::Frac()
{
    TFixedPoint128 result;
    result.m_value = m_value & (0x00FF);
    return result;
}

TFixedPoint128 TFixedPoint128::Sqrt()
{
    TFixedPoint128 res0 = 1;
    TFixedPoint128 res  = 0;
    for(short i = 0; i<20; i++)
    {
        res = ((*this) / res0 + res0) / 2;
        if (res0==res) break;
        res0 = res;
    }
    return res;
}

TFixedPoint128 TFixedPoint128::Sqrt3()
{
    TFixedPoint128 res0 = 1;
    TFixedPoint128 res  = 0;
    for(short i = 0; i<20; i++)
    {
        res = ((*this) / res0 / res0 + res0 + res0) / 3;
        if (res0==res) break;
        res0 = res;
    }
    return res;
}

bool TFixedPoint128::IsZero()
{
    return m_value == 0;
}

void TFixedPoint128::Print(char* pbOutputBuffer, unsigned short cbOutputBuffer)
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
    short exp = 100<<8;

    //divides n by 1000, 100, 10 and 1
    //this way are calculates first four digits
    for(int i = 0; i<3; i++)
    {        
        c = '0';
        if (i==2)
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
    exp=1<<8;
    for(int j = 0; j<2; j++)
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