/*
 * Persistence Library / Basic types / TDateTimeCounter
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

#ifndef TDATETIMECOUNTER___H
#define TDATETIMECOUNTER___H

#include "TDateTime.h"
#include "Time.h"

/**
 *  TDateTimeCounter increases current date and time every second
 *  (it can be used when you target platform contains no Real Time Clock (RTC))
 */

class TDateTimeCounter : protected TDateTime
{
    private:
		short m_microseconds;
        short m_minutesToAddOneSecond;
		TTime m_calibrationCounter;

        
    public:
        TDateTimeCounter();

		void SetDate(unsigned short year, unsigned char month,  unsigned char day);
		void SetTime(unsigned char  hour, unsigned char minute, unsigned char second);

		unsigned char  GetDay();
		unsigned char  GetMonth();
		unsigned short GetYear();
		unsigned char  GetHour();
		unsigned char  GetMinute();
		unsigned char  GetSecond();
		unsigned short GetMillisecond(); 

		//when time "minutesToCorrection" expires, one second will be added (addSecond=true) or substracted (addSecond=false)
		void SetMinuteCalibration(short minutesToCorrection, bool addSecond);

		//timer tick method add time interval to current time (one millisecond by default)
		void OnTimeTickMicroSeconds(short microseconds=1000);

		//timer tick method add time interval to current time (one second by default)
        void OnTimerTickMilliSeconds(short milliseconds=1000);

};


#endif