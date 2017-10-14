/*
 * Persistence Library / Basic types / TDate
 *
 * Copyright (c) 2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TDATE___H
#define TDATE___H

/**
 *  TDate is very simple class for storing date a adding or substracting intervals (days, months, years) to this date.
 *
 */

class TDate
{
    protected:
		unsigned short m_year;
		unsigned char  m_month;
        unsigned char  m_day;                

        unsigned char  DaysInMonth(unsigned short month, unsigned short year);

    public:

        TDate(unsigned short year, unsigned char month, unsigned char day);
        TDate(const char* date);

        unsigned char  GetDay();
        unsigned char  GetMonth();
        unsigned short GetYear();
		unsigned short GetTotalDays();
        
        TDate AddYear(short seconds = 1);
        TDate AddMonth(short months = 1);
        TDate AddDay(short days = 1);        

        unsigned short PrintDate (char* pbOutputString, unsigned short cbOutputString);

		bool operator >  (const TDate &date);
		bool operator >= (const TDate &date);
		bool operator == (const TDate &date);
		bool operator != (const TDate &date);
		bool operator <= (const TDate &date);
		bool operator < (const TDate &date);
        
};

#endif