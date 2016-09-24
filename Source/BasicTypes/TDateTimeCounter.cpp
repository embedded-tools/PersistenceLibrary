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


#include "TDateTimeCounter.h"

TDateTimeCounter::TDateTimeCounter()
    : TDateTime(1900, 1, 1, 0, 0, 0),
      m_minutesToAddOneSecond(0),
      m_calibrationCounter(0,0,0)
{

}

void TDateTimeCounter::SetMinuteCalibration(short minutesToCorrection, bool add)
{
	if (!add) minutesToCorrection = -minutesToCorrection;
	m_minutesToAddOneSecond = minutesToCorrection;
}

void TDateTimeCounter::OnTimerTick(short milliseconds)
{
    TDateTime& dt = *this;
    dt = dt.AddMilliSeconds(milliseconds);
	
	m_calibrationCounter.AddMilliSeconds(milliseconds);
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

