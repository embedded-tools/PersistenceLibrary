/*
 * Persistence Library / Basic types / TDateTime
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

#ifndef DATETIME___H
#define DATETIME___H

#include "TDate.h"
#include "TTime.h"

/**
 *  TDateTime is very simple class for storing date and time a adding or substracting intervals (seconds, minutes, hours, days, months, years) to this date and time.
 *
 */

class TDateTime : public TDate, public TTime
{
    public:
        TDateTime(unsigned short year, unsigned char month, unsigned char day);
        TDateTime(unsigned short year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second, unsigned short millisecond=0);
        TDateTime(const char* dateTime);

        unsigned short PrintDateTime (char* pbOutputString, unsigned short cbOutputString);

        TDateTime AddYear(short year = 1);
        TDateTime AddMonth(short months = 1);
        TDateTime AddDay(short days = 1);   
        TDateTime AddHours(short hours=1);
        TDateTime AddMinutes(short hours=1);
        TDateTime AddSeconds(short seconds=1);
        TDateTime AddMilliSeconds(short seconds=1);


                        
};

#endif

