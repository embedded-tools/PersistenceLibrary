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

class TDateTimeCounter : public TDateTime
{
    private:
        short m_minutesToAddOneSecond;
		TTime m_calibrationCounter;

        
    public:
        TDateTimeCounter();

		//when time "minutesToCorrection" expires, one second will be added (addSecond=true) or substracted (addSecond=false)
		void SetMinuteCalibration(short minutesToCorrection, bool addSecond);

		//timer tick method add time interval to current time (one second by default)
        void OnTimerTick(short milliseconds=1000);

};


#endif