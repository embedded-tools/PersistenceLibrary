/*
 * Persistence Library / Basic types / TTime 
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

#ifndef TTIME___H
#define TTIME___H

/**
 *  TDate is very simple class for storing time a adding or substracting intervals (seconds, minutes, hours) to this date.
 *
 */

class TTime
{
protected:
    unsigned char  m_hour;
    unsigned char  m_minute;
    unsigned short m_milliseconds;

public:

    TTime(unsigned char hour, unsigned char minute, unsigned char second, unsigned short milliSecond=0);
    TTime(const char* time);

    unsigned char GetHour();
    unsigned char GetMinute();
    unsigned char GetSecond();
    unsigned short GetMillisecond(); 
	unsigned long  GetTotalMilliseconds();

	void SetHour(unsigned char hour);
	void SetMinute(unsigned char minute);
	void SetSecond(unsigned char second);
	void SetMilliSecond(unsigned short milliSecond);

    TTime AddHours(short hours=1);
    TTime AddMinutes(short hours=1);
    TTime AddSeconds(short seconds=1);
    TTime AddMilliSeconds(short seconds=1);

    unsigned short PrintTime    (char* pbOutputString, unsigned short cbOutputString);
    unsigned short PrintTimeFull(char* pbOutputString, unsigned short cbOutputString);
};

#endif


