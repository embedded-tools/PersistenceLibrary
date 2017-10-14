/*
 * Persistence Library / Basic types / TTime
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

#include "TTime.h"


TTime::TTime(unsigned char hour, unsigned char minute, unsigned char second, unsigned short milliSecond)
    : m_hour(hour), m_minute(minute), m_milliseconds(second*1000+milliSecond)
{

}

TTime::TTime(const char* time)
    : m_hour(0), m_minute(0), m_milliseconds(0)
{
	//it assumes that <stdio.h> and sscanf function is not available on low end MCUs
	//like Atmel AVR Mega or Atmel AVR Tiny. Always use sscanf if possible!!!
    if (time!=0)
    {
        if (time[0]!=0)
        {
            if ( (time[2]==':') && (time[5]==':') && ((time[8]==0) || (time[8]=='.')) )
            {
                m_hour   = (time[0]-'0')*10 + (time[1]-'0');
                m_minute = (time[3]-'0')*10 + (time[4]-'0');
                m_milliseconds = (time[6]-'0')*10000 + (time[7]-'0')*1000;
                if (time[8]=='.')
                {
                    m_milliseconds+=(time[9]-'0')*100 + (time[10]-'0')*10 + (time[11]-'0');
                }
            }
        }
    }
}

unsigned char TTime::GetHour()
{
    return m_hour;
}

unsigned char TTime::GetMinute()
{
    return m_minute;
}

unsigned char TTime::GetSecond()
{
    return m_milliseconds / 1000;
}

unsigned short TTime::GetMillisecond()
{
    return m_milliseconds % 1000;
}

unsigned long TTime::GetTotalMilliseconds()
{
	unsigned long result = (m_hour * 3600  + m_minute * 60) * 1000 + m_milliseconds;
	return result;
}



void TTime::SetHour(unsigned char hour)
{
	m_hour = hour;
}

void TTime::SetMinute(unsigned char minute)
{
	m_minute = minute;
}

void TTime::SetSecond(unsigned char second)
{
	m_milliseconds = (m_milliseconds % 1000) + (second*1000);
}

void TTime::SetMilliSecond(unsigned short milliSecond)
{
	m_milliseconds = ((m_milliseconds / 1000) * 1000) + milliSecond;
}


TTime TTime::AddHours(short hours)
{
    TTime result = *this;
    result.m_hour += hours;
    if (hours>0)
    {
        if (result.m_hour>=24)
        {
            result.m_hour = result.m_hour % 24;
        }
    } else {
        while(result.m_hour>128)
        {
            result.m_hour += 24;
        }
    }
    return result;
}

TTime TTime::AddMinutes(short minutes)
{
    TTime result = *this;

    short hours = minutes/60;
    minutes = minutes % 60;
    result = result.AddHours(hours);    
    
    result.m_minute += minutes;

    if (minutes>0)
    {
        if (result.m_minute>=60)
        {
            result = result.AddHours(result.m_minute / 60);
            result.m_minute = result.m_minute % 60;
        }
    } else {
        if (result.m_minute>128)
        {
            result.m_minute += 60;
            result = result.AddHours(-1);
        }
    }
    return result;
}

TTime TTime::AddSeconds(short seconds)
{
    TTime result = *this;

    short hours = seconds/3600;
    seconds -= hours*3600;
    result = result.AddHours(hours);

    short minutes = seconds/60;
    seconds -= minutes*60;
    result = result.AddMinutes(minutes);
  
    unsigned short oldValue = result.m_milliseconds;

    if (seconds>0)
    {
        while(seconds>0)
        {
            result.m_milliseconds+=1000;
            if (result.m_milliseconds>=60000)
            {
                result.m_milliseconds-=60000;
                result = result.AddMinutes(1);
            }
            seconds--;
        }
    } else {
        while(seconds<0)
        {
            result.m_milliseconds-=1000;
            if (result.m_milliseconds>=60000)
            {
                result.m_milliseconds+=60000;
                result = result.AddMinutes(-1);
            }
            seconds++;
        }
    }
    return result;
}

TTime TTime::AddMilliSeconds(short milliSeconds)
{
    TTime result = *this;

    short seconds = milliSeconds/1000;
    milliSeconds -= seconds*1000;
    result = result.AddSeconds(seconds);

	if (milliSeconds>0)
	{
		result.m_milliseconds+= milliSeconds;
		if (result.m_milliseconds>=60000)
		{
			result.m_milliseconds-=60000;
			result = result.AddMinutes(1);
		}
	} else {
		if ( ((long)(-milliSeconds))<= ((long)m_milliseconds) )
		{
			result.m_milliseconds+= milliSeconds;
		} else
		{
			result.m_milliseconds+= milliSeconds;
			result.m_milliseconds+=60000;
			result = result.AddMinutes(-1);
		}
	}
    return result;
}

unsigned short TTime::PrintTime(char* pbOutputString, unsigned short cbOutputString)
{    
	if (pbOutputString==0) return 0;
    if (cbOutputString<9) return 0;

	//it assumes that <stdio.h> and sprintf function is not available on low end MCUs
	//like Atmel AVR Mega or Atmel AVR Tiny. Always use sprintf if possible!!!
 
    pbOutputString[0] = m_hour / 10 + '0';
    pbOutputString[1] = m_hour % 10 + '0';
    pbOutputString[2] = ':';
    pbOutputString[3] = m_minute / 10 + '0';
    pbOutputString[4] = m_minute % 10 + '0';
    pbOutputString[5] = ':';
    unsigned short seconds = m_milliseconds/1000;

    pbOutputString[6] = seconds / 10 + '0';
    pbOutputString[7] = seconds % 10 + '0'; 
    pbOutputString[8] = 0;

    return 9;
}

unsigned short TTime::PrintTimeFull(char* pbOutputString, unsigned short cbOutputString)
{
	if (pbOutputString==0) return 0;
    if (cbOutputString<13) return 0;

	//it assumes that <stdio.h> and sprintf function is not available on low end MCUs
	//like Atmel AVR Mega or Atmel AVR Tiny. Always use sprintf if possible!!!

    pbOutputString[0] = m_hour / 10 + '0';
    pbOutputString[1] = m_hour % 10 + '0';
    pbOutputString[2] = ':';
    pbOutputString[3] = m_minute / 10 + '0';
    pbOutputString[4] = m_minute % 10 + '0';
    pbOutputString[5] = ':';
    unsigned short seconds = m_milliseconds/1000;
    unsigned short milliseconds = m_milliseconds-seconds*1000;
    pbOutputString[6] = seconds / 10 + '0';
    pbOutputString[7] = seconds % 10 + '0';    
    pbOutputString[8] = '.';
    pbOutputString[9] = (milliseconds / 100) % 10 + '0';
    pbOutputString[10]= (milliseconds / 10) % 10 + '0';
    pbOutputString[11]= (milliseconds % 10) + '0';
    pbOutputString[12]= 0;

    return 13;
}
