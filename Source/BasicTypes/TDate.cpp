/*
 * Persistence Library / Basic types / TDate
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

#include "TDate.h"


TDate::TDate(unsigned short year, unsigned char month, unsigned char day)
    : m_year(year), m_month(month), m_day(day)
{
    unsigned char maxDays = DaysInMonth(m_month, m_year);
    if (m_day>maxDays)
    {
        m_day = maxDays;
    }
    if (m_month>12)
    {
        m_month = 12;
    }
}

TDate::TDate(const char* date)
    : m_year(1900), m_month(1), m_day(1)
{
    if (date)
    {
		//it assumes that <stdio.h> and sscanf function is not available on low end MCUs
		//like Atmel AVR Mega or Atmel AVR Tiny. Always use sscanf if possible!!!
        if (date[0]!=0)
        {
            if ( (date[4]=='-') && (date[7]=='-') && ((date[10]==0) || (date[10]=='T')) )
            {
                m_year = (date[0]-'0')*1000+(date[1]-'0')*100+(date[2]-'0')*10+(date[3]-'0');
                m_month = (date[5]-'0')*10+(date[6]-'0');
                m_day = (date[8]-'0')*10+(date[9]-'0');
            }
        }
    }
}

unsigned char TDate::GetDay()
{
    return m_day;
}

unsigned char TDate::GetMonth()
{
    return m_month;
}

unsigned short TDate::GetYear()
{
    return m_year;
}

unsigned short TDate::GetTotalDays()
{
	unsigned short result = (m_year-1970)*365 + (m_year-1969)/4;
	if (m_month>11) result+=30;
	if (m_month>10) result+=31;
	if (m_month>9)  result+=30;
	if (m_month>8)  result+=31;
	if (m_month>7)  result+=31;
	if (m_month>6)  result+=30;
	if (m_month>5)  result+=31;
	if (m_month>4)  result+=30;
	if (m_month>3)  result+=31;
	if (m_month>2)  result+=(m_year%4)?28:29;
	if (m_month>1)  result+=31;
	result+=m_day - 1;

	//max. range is 1.1.1970 to 5.6.2149 (0 to 65535)

	return result;
}

unsigned char TDate::DaysInMonth(unsigned short month, unsigned short year)
{
    unsigned char result = 1;
    switch(month)
    {
        case 1:  result = 31; break;
        case 2:  result = ((year%4)==0)?29:28; break;
        case 3:  result = 31; break;
        case 4:  result = 30; break;
        case 5:  result = 31; break;
        case 6:  result = 30; break;
        case 7:  result = 31; break;
        case 8:  result = 31; break;
        case 9:  result = 30; break;
        case 10: result = 31; break;
        case 11: result = 30; break;
        case 12: result = 31; break;
    }
    return result;
}

void TDate::SetYear(short year)
{
	m_year = year;
}

void TDate::SetMonth(short month)
{
	m_month = (unsigned char)month;
}

void TDate::SetDay(short day)
{
	m_day = (unsigned char)day;
}

TDate TDate::AddYear(short years)
{
    TDate result(*this);
    result.m_year += years;

    unsigned char maxDays = DaysInMonth(result.m_month, result.m_year);
    if (result.m_day>maxDays)
    {
        result.m_day = 1;
        result.m_month++;
    }
    return result;
}

TDate TDate::AddMonth(short months)
{
    short y = months / 12;
    short m = months % 12;

    TDate result = *this;

    result = result.AddYear(y);
    result.m_month += m;
    if (months>=0)
    {
        if (result.m_month>12)
        {
            result.m_year++;
            result.m_month-=12;
        }
    } else {
        if ((result.m_month<1) || (result.m_month>128))
        {
            result.m_month += 12;
        }
    }       

    unsigned char maxDays = DaysInMonth(result.m_month, result.m_year);
    if (result.m_day>maxDays)
    {
        result.m_day = 1;
        result.m_month++;
    }
    return result;
}


TDate TDate::AddDay(short days)
{
    TDate result(*this);

    while(days!=0)
    {
        if (days>0)
        {
            if (days>16)
            {
                result.m_day += 16;
                days-=16;
            } else {
                result.m_day++;
                days--;
            }
            unsigned short maxDays = DaysInMonth(result.m_month, result.m_year);
            if (result.m_day>maxDays)
            {
                result.m_day-=maxDays;
                result.m_month++;
                if (result.m_month>12)
                {
                    result.m_month = 1;
                    result.m_year++;
                }                
            }
        } else {
            if (days<-16)
            {
                result.m_day -= 16;
                days+=16;
            } else {
                result.m_day--;
                days++;
            }
            if ((result.m_day<1) || (result.m_day>128))
            {                
                result.m_month--;
                if (result.m_month<1)
                {
                    result.m_month = 12;
                    result.m_year--;
                }
                unsigned short maxDays = DaysInMonth(result.m_month, result.m_year);
                result.m_day+= maxDays;
            }
        }
    }
    return result;
}


unsigned short TDate::PrintDate(char* pbOutputString, unsigned short cbOutputString)
{    
	if (pbOutputString==0) return false;
    if (cbOutputString<11) return false;

	//it assumes that <stdio.h> and sprintf function is not available on low end MCUs
	//like Atmel AVR Mega or Atmel AVR Tiny. Always use sprintf if possible!!!

    pbOutputString[0] = (m_year  / 1000) % 10 + '0';
    pbOutputString[1] = (m_year / 100)  % 10 + '0';     
    pbOutputString[2] = (m_year / 10)  % 10 + '0';
    pbOutputString[3] = (m_year)      % 10 + '0';
    pbOutputString[4] = '-';
    pbOutputString[5] = m_month / 10 + '0';
    pbOutputString[6] = m_month % 10 + '0';
    pbOutputString[7] = '-';
    pbOutputString[8] = m_day / 10 + '0';
    pbOutputString[9] = m_day % 10 + '0';    
    pbOutputString[10] = 0;
    return 10;
}

