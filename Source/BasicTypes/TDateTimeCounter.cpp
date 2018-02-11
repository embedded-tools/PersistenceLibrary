/*
 * Persistence Library / Basic types / TDateTimeCounter
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


#include "TDateTimeCounter.h"

TDateTimeCounter::TDateTimeCounter()
    : TDateTime(1900, 1, 1, 0, 0, 0),
	  m_microseconds(0),
      m_minutesToAddOneSecond(0),
      m_calibrationCounter(0,0,0)
{

}

void TDateTimeCounter::SetDate(unsigned short year, unsigned char month,  unsigned char day)
{
	m_year = year;
	m_month = month;
	m_day = day;
}

void TDateTimeCounter::SetTime(unsigned char  hour, unsigned char minute, unsigned char second)
{	
	m_hour = hour;
	m_minute = minute;
	m_milliseconds = (unsigned short)(second * 1000);
}

unsigned char  TDateTimeCounter::GetDay()
{
	return m_day;
}

unsigned char  TDateTimeCounter::GetMonth()
{
	return m_month;
}

unsigned short TDateTimeCounter::GetYear()
{
	return m_year;
}

unsigned char  TDateTimeCounter::GetHour()
{
	return m_hour;
}

unsigned char  TDateTimeCounter::GetMinute()
{
	return m_minute;
}

unsigned char  TDateTimeCounter::GetSecond()
{
	return m_milliseconds/1000;
}

unsigned short TDateTimeCounter::GetMillisecond()
{	
	return m_milliseconds % 1000;
}


void TDateTimeCounter::SetMinuteCalibration(short minutesToCorrection, bool add)
{
	if (!add) minutesToCorrection = -minutesToCorrection;
	m_minutesToAddOneSecond = minutesToCorrection;
}

void TDateTimeCounter::OnTimeTickMicroSeconds(short microseconds)
{
	m_microseconds += microseconds;
	short milliseconds = m_microseconds/1000;
	if (milliseconds>0)
	{
		OnTimerTickMilliSeconds(milliseconds);
		m_microseconds-=microseconds*1000;
	}
}

void TDateTimeCounter::OnTimerTickMilliSeconds(short milliseconds)
{
    TDateTime& dt = *this;
    dt = dt.AddMilliSeconds(milliseconds);
	
	m_calibrationCounter.AddMilliSeconds(milliseconds);
	if (m_minutesToAddOneSecond!=0)
	{
		if (m_calibrationCounter.GetMinute()==m_minutesToAddOneSecond)
		{
			if (m_minutesToAddOneSecond>0)
			{
				dt = dt.AddSeconds();
			} else {
				dt = dt.AddSeconds(-1);
			}
		}
	}
}

