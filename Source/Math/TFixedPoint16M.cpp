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

#include "TFixedPoint16M.h"

#ifndef WIN32
#include <stdint.h>
#endif


TFixedPoint16M::TFixedPoint16M()
    : m_value(0)
{

}

TFixedPoint16M::TFixedPoint16M(long value)
{
    if (value>16777215)
    {
        m_value = FIXEDPOINT_INF_32;
    }
    if (value<-1677215)
    {
        m_value = FIXEDPOINT_NEG_INF_32;
    }
    m_value = value<<5;
}

TFixedPoint16M::TFixedPoint16M(long frac1, long frac2)
{
    m_value = frac1*32/frac2;
}

TFixedPoint16M& TFixedPoint16M::operator = (long num)
{
    m_value = num << 5;
    return *this;
}

TFixedPoint16M& TFixedPoint16M::operator = (const TFixedPoint16M value)
{
    m_value = value.m_value;
    return *this;
}

TFixedPoint16M TFixedPoint16M::operator - ()
{
    TFixedPoint16M result = *this;
    if (result.m_value==FIXEDPOINT_INF_32)
    {
        result.m_value=FIXEDPOINT_NEG_INF_32;
    } else 
    if (result.m_value==FIXEDPOINT_NEG_INF_32)
    {
        result.m_value=FIXEDPOINT_INF_32;
    } else {
        result.m_value = -result.m_value;
    }
    return result;
}

TFixedPoint16M  TFixedPoint16M::operator + (long value)
{
    TFixedPoint16M result = *this;
    result += value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator +=(long value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    long value5 = value<<5;
    long result = m_value + value5;
    if (value5>0)
    {
        if (result<m_value) result=FIXEDPOINT_INF_32;
    } else {
        if (result>m_value) result=FIXEDPOINT_NEG_INF_32;
    }
    m_value=result;
    return *this;
}

TFixedPoint16M  TFixedPoint16M::operator + (TFixedPoint16M& value)
{
    TFixedPoint16M result = *this;
    result += value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator +=(TFixedPoint16M& value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    long result = m_value + value.m_value;
    if (value.Round()>=0)
    {
        if (result<m_value) result=FIXEDPOINT_INF_32;
    } else {
        if (result>m_value) result=FIXEDPOINT_NEG_INF_32;
    }
    m_value = result;
    return *this;
}

TFixedPoint16M  TFixedPoint16M::operator - (long value)
{
    TFixedPoint16M result = *this;
    result -= value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator -=(long value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    long value5 = value<<5;
    long result = m_value - value5;
    if (value5>0)
    {
        if (result>m_value) result=FIXEDPOINT_INF_32;
    } else {
        if (result<m_value) result=FIXEDPOINT_NEG_INF_32;
    }
    m_value=result;
    return *this;
}

TFixedPoint16M  TFixedPoint16M::operator - (TFixedPoint16M& value)
{
    TFixedPoint16M result = *this;
    result -= value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator -=(TFixedPoint16M& value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    m_value-=value.m_value;
    if (value.Round()>=0)
    {
        if (m_value>value.m_value) m_value=FIXEDPOINT_NEG_INF_32;
    } else {
        if (m_value<value.m_value) m_value=FIXEDPOINT_INF_32;
    }
    return *this;
}


TFixedPoint16M  TFixedPoint16M::operator * (long value)
{
    TFixedPoint16M result = *this;
    result *= value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator *=(long value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
#ifdef WIN32

#if (_MSC_VER>=1400)
    long long result = (long long)m_value * value;
#else                                                                   
   __int64 result = ((__int64)m_value) * value;
#endif

#else

	int64_t result = ((int64_t)m_value * value);
#endif
    if (result>=16777215)
    {
        result = 16777215;
    } else
    if (result<-16777215)
    {
        result = -16777215;
    }
    if (value>=1) 
    {
        if ((long)result<m_value)
        {
            m_value = FIXEDPOINT_INF_32;
        }
        m_value = (int)result;
    } else {
        if ((long)result>m_value)
        {
            m_value = FIXEDPOINT_NEG_INF_32;
        }
        m_value = (int)result;
    }
    return *this;
}

TFixedPoint16M  TFixedPoint16M::operator * (TFixedPoint16M& value)
{
    TFixedPoint16M result = *this;
    result *= value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator *=(TFixedPoint16M& value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
#ifdef WIN32

#if (_MSC_VER>=1400)
    long long result = (long long)m_value * value.m_value>>5;
#else
    __int64 result = ((__int64)m_value) * value.m_value>>5;
#endif
    
#else
	int64_t result = (int64_t)(m_value * value.m_value)>>5;;
#endif

    if (result>=16777215)
    {
        result = 16777215;
    } else
    if (result<-16777215)
    {
        result = -16777215;
    }
    if (value.Round()>=1) //value>1.0
    {
        if (((long)result)<m_value)
        {
            m_value = FIXEDPOINT_INF_32;
        }
        m_value = (long)result;
    } else {
        if (((long)result)>m_value)
        {
            m_value = FIXEDPOINT_NEG_INF_32;
        }
        m_value = (long)result;
    }
    return *this;
}

TFixedPoint16M  TFixedPoint16M::operator / (long value)
{
    TFixedPoint16M result = *this;
    result /= value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator /=(long value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    if (value==0)
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF_32;
        } else {
            m_value = FIXEDPOINT_NEG_INF_32;
        }
        return *this;        
    }
    //result = 32bit value
    long result = (m_value / value);
    if (result>=16777215)
    {
        result = 16777215;
    } else
        if (result<-16777215)
        {
            result = -16777215;
        }
        if (value>=1) //value>1.0
        {
            if (((long)result)<m_value)
            {
                m_value = FIXEDPOINT_INF_32;
            }
            m_value = (long)result;
        } else {
            if (((long)result)>m_value)
            {
                m_value = FIXEDPOINT_NEG_INF_32;
            }
            m_value = (long)result;
        }
        return *this;

}

TFixedPoint16M  TFixedPoint16M::operator / (TFixedPoint16M& value)
{
    TFixedPoint16M result = *this;
    result /= value;
    return result;    
}

TFixedPoint16M& TFixedPoint16M::operator /=(TFixedPoint16M& value)
{
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    if (value.IsZero())
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF_32;
        } else {
            m_value = FIXEDPOINT_NEG_INF_32;
        }
        return *this;        
    }
    //result = 32bit value
    long result = (((long)m_value<<5) / value.m_value);
    if (result>=16777215)
    {
        result = 16777215;
    } else
    if (result<-16777215)
    {
        result = -16777215;
    }
    if (value.m_value>=32) //value>1.0
    {
        if (((long)result)<m_value)
        {
            m_value = FIXEDPOINT_INF_32;
        }
        m_value = (long)result;
    } else {
        if (((long)result)>m_value)
        {
            m_value = FIXEDPOINT_NEG_INF_32;
        }
        m_value = (long)result;
    }
    return *this;
}

TFixedPoint16M  TFixedPoint16M::operator % (long value)
{
    TFixedPoint16M result =  *this;
    if ((m_value==FIXEDPOINT_INF_32) || (m_value==FIXEDPOINT_NEG_INF_32))
    {
        return *this;
    }
    if ((value>=16777215) || (value<=-16777215))
    {
        if (Sgn()>=0)
        {
            m_value = FIXEDPOINT_INF_32;
        } else {
            m_value = FIXEDPOINT_NEG_INF_32;
        }
        return *this;
    }
    result.m_value = result.m_value % (value<<5);
    return result;    
}

bool TFixedPoint16M::operator == (long num)
{
    if ((num<=-16777215) || (num>=16777215)) return false;
    return m_value == (num<<5);
}

bool TFixedPoint16M::operator == (TFixedPoint16M& num)
{
    return m_value == num.m_value;
}

bool TFixedPoint16M::operator != (long num)
{
    if ((num<=-16777215) || (num>=16777215)) return true;
    return m_value != (num<<5);
}

bool TFixedPoint16M::operator != (TFixedPoint16M& num)
{
    return m_value!=num.m_value;
}

bool TFixedPoint16M::operator >= (long num)
{
    if (num>=16777215) return false;
    if (num<=-16777215) return true;
    return m_value>=(num<<5);
}

bool TFixedPoint16M::operator >= (TFixedPoint16M& num)
{
    return m_value>=num.m_value;
}

bool TFixedPoint16M::operator <= (long num)
{
    if (num<=-16777215) return false;
    if (num>= 16777215) return true;
    return m_value<=(num<<5);
}

bool TFixedPoint16M::operator <= (TFixedPoint16M& num)
{
    return m_value<=num.m_value;
}

bool TFixedPoint16M::operator > (long num)
{
    if (num>= 16777215) return false;
    if (num<=-16777215) return true;
    return m_value>(num<<5);
}

bool TFixedPoint16M::operator > (TFixedPoint16M& num)
{
    return m_value>num.m_value;
}

bool TFixedPoint16M::operator < (long num)
{
    if (num<=-16777215) return false;
    if (num>= 16777215) return true;
    return m_value<(num<<5);
}

bool TFixedPoint16M::operator < (TFixedPoint16M& num)
{
    return m_value<num.m_value;
}

signed char     TFixedPoint16M::Sgn()
{
    if (m_value>0) return 1;
    if (m_value==0) return 0;        
    return -1;
}

long TFixedPoint16M::Round()
{
    long result = (m_value+16)/32;
    return result;
}

long TFixedPoint16M::Trunc()
{
    long result = m_value/32;
    return result;
}

TFixedPoint16M TFixedPoint16M::Frac()
{
    TFixedPoint16M result;
    result.m_value = m_value & (0x001F);
    return result;
}

TFixedPoint16M TFixedPoint16M::Sqrt()
{
    TFixedPoint16M res0 = 1;
    TFixedPoint16M res  = 0;
    for(long i = 0; i<20; i++)
    {
        res = ((*this) / res0 + res0) / 2;
        if (res0==res) break;
        res0 = res;
    }
    return res;
}

TFixedPoint16M TFixedPoint16M::Sqrt3()
{
    TFixedPoint16M res0 = 1;
    TFixedPoint16M res  = 0;
    for(long i = 0; i<20; i++)
    {
        res = ((*this) / res0 / res0 + res0 + res0) / 3;
        if (res0==res) break;
        res0 = res;
    }
    return res;
}

bool TFixedPoint16M::IsZero()
{
    return m_value==0;
}

void TFixedPoint16M::Print(char* pbOutputBuffer, unsigned long cbOutputBuffer)
{
    *pbOutputBuffer = 0;
    if (cbOutputBuffer<7)
    {
        return;
    }

    if (m_value==FIXEDPOINT_INF_32)
    {
        *pbOutputBuffer='+'; pbOutputBuffer++;
        *pbOutputBuffer='I'; pbOutputBuffer++;
        *pbOutputBuffer='N'; pbOutputBuffer++;
        *pbOutputBuffer='F'; pbOutputBuffer++;
        *pbOutputBuffer=0;
        return;
    }
    if (m_value==FIXEDPOINT_NEG_INF_32)
    {
        *pbOutputBuffer='-'; pbOutputBuffer++;
        *pbOutputBuffer='I'; pbOutputBuffer++;
        *pbOutputBuffer='N'; pbOutputBuffer++;
        *pbOutputBuffer='F'; pbOutputBuffer++;
        *pbOutputBuffer=0;
        return;
    }

    cbOutputBuffer=0;
    unsigned long n = 0;
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
    unsigned long exp = 1000<<5;

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
