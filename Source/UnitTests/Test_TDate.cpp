#include "minicppunit.hxx"

#include "TDate.h"

class Test_TDate : public TestFixture<Test_TDate>
{
  public:

    TEST_FIXTURE( Test_TDate)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Getter );
        TEST_CASE( Getter2 );
        TEST_CASE( AddDay );
        TEST_CASE( AddMonth );
        TEST_CASE( AddYear );
		TEST_CASE( GetTotalDays1 );
		TEST_CASE( GetTotalDays2 );
		TEST_CASE( LeapYearTest );
		TEST_CASE( DateDifference );
    }


    void ConstructDestruct()
    {
        TDate date(2000, 12, 31);
	}

    void Getter()
    {
        TDate date(2000, 12, 31);
        ASSERT(date.GetYear()==2000);
        ASSERT(date.GetMonth()==12);
        ASSERT(date.GetDay()==31);
    }

    void Getter2()
    {
        TDate date("1984-12-31");
        ASSERT(date.GetYear()==1984);
        ASSERT(date.GetMonth()==12);
        ASSERT(date.GetDay()==31);
    }

    void PrintDate()
    {
        TDate date(1984, 12, 31);
        ASSERT(date.GetYear()==1984);
        ASSERT(date.GetMonth()==12);
        ASSERT(date.GetDay()==31);

        char text[16];
        date.PrintDate(text, 16);

        ASSERT_EQUALS(text, "1984-12-31");
    }

    void AddDay()
    {
        TDate date(2015, 12, 31);
        ASSERT(date.GetYear()==2015);
        ASSERT(date.GetMonth()==12);
        ASSERT(date.GetDay()==31);

        TDate date2 = date.AddDay(60);
        ASSERT(date2.GetYear()==2016);
        ASSERT(date2.GetMonth()==2);
        ASSERT(date2.GetDay()==29);

        char text[16];
        date2.PrintDate(text, 16);

        TDate date3 = date2.AddDay(10167);
        date3.PrintDate(text, 16);
        ASSERT(date3.GetYear()==2043);
        ASSERT(date3.GetMonth()==12);
        ASSERT(date3.GetDay()==31);

        TDate date4 = date3.AddDay(-60);
        date4.PrintDate(text, 16);
        ASSERT(date4.GetYear()==2043);
        ASSERT(date4.GetMonth()==11);
        ASSERT(date4.GetDay()==1);

        TDate date5 = date4.AddDay(-10167);
        date5.PrintDate(text, 16);
        ASSERT(date5.GetYear()==2015);
        ASSERT(date5.GetMonth()==12);
        ASSERT(date5.GetDay()==31);
    }

    void AddMonth()
    {
        TDate date(2015, 12, 31);
        ASSERT(date.GetYear()==2015);
        ASSERT(date.GetMonth()==12);
        ASSERT(date.GetDay()==31);

        TDate date2 = date.AddMonth(2);
        ASSERT(date2.GetYear()==2016);
        ASSERT(date2.GetMonth()==3);
        ASSERT(date2.GetDay()==1);

        TDate date3(2016, 8, 31);
        TDate date4 = date3.AddMonth(1);
        ASSERT(date4.GetYear()==2016);
        ASSERT(date4.GetMonth()==10);
        ASSERT(date4.GetDay()==1);
    }

    void AddYear()
    {
        TDate date(2016, 2, 29);

        ASSERT(date.GetYear()==2016);
        ASSERT(date.GetMonth()==2);
        ASSERT(date.GetDay()==29);

        TDate date2 = date.AddYear();
        ASSERT(date2.GetYear()==2017);
        ASSERT(date2.GetMonth()==3);
        ASSERT(date2.GetDay()==1);

        TDate date3 = date.AddYear(-1);
        ASSERT(date3.GetYear()==2015);
        ASSERT(date3.GetMonth()==3);
        ASSERT(date3.GetDay()==1);
    }

	void GetTotalDays1()
	{
		TDate date1(1970, 1,  1);
		TDate date2(1970, 2,  1);
		TDate date3(1970, 3,  1);
		TDate date4(1970, 4,  1);
		TDate date5(1971, 1,  1);
		int days;

		days = date1.GetTotalDays();
		ASSERT_EQUALS(0, days);

		days = date2.GetTotalDays();
		ASSERT_EQUALS(31, days);

		days = date3.GetTotalDays();
		ASSERT_EQUALS(59, days);

		days = date4.GetTotalDays();
		ASSERT_EQUALS(90, days);

		days = date5.GetTotalDays();
		ASSERT_EQUALS(365, days);
	}

	void GetTotalDays2()
	{
		TDate date1(1972, 1,   1);
		TDate date2(1972, 2,   1);
		TDate date3(1972, 3,   1);
		TDate date4(1972, 4,   1);
		TDate date5(1972, 12, 31);
		TDate date6(1973, 1,   1);
		TDate date7(2149, 6,   5);
		int days;

		days = date1.GetTotalDays();
		ASSERT_EQUALS(730, days);

		days = date2.GetTotalDays();
		ASSERT_EQUALS(761, days);

		days = date3.GetTotalDays();
		ASSERT_EQUALS(790, days);

		days = date4.GetTotalDays();
		ASSERT_EQUALS(821, days);

		days = date5.GetTotalDays();
		ASSERT_EQUALS(1095, days);

		days = date6.GetTotalDays();
 		ASSERT_EQUALS(1096, days);

		days = date7.GetTotalDays();
		ASSERT_EQUALS(1096, days);

	}

	void LeapYearTest()
	{
		TDate date1(1999, 1,  1);
		TDate date2(2000, 1,  1);
		TDate date3(2001, 1,  1);
		TDate date4(2002, 1,  1);
		TDate date5(2003, 1,  1);
		TDate date6(2004, 1,  1);

		int difference;

		difference = date2.GetTotalDays() - date1.GetTotalDays();
		ASSERT_EQUALS(365, difference);

		difference = date3.GetTotalDays() - date2.GetTotalDays();
		ASSERT_EQUALS(366, difference);

		difference = date4.GetTotalDays() - date3.GetTotalDays();
		ASSERT_EQUALS(365, difference);

		difference = date5.GetTotalDays() - date4.GetTotalDays();
		ASSERT_EQUALS(365, difference);

		difference = date6.GetTotalDays() - date5.GetTotalDays();
		ASSERT_EQUALS(365, difference);
	}

	void DateDifference()
	{
		TDate date1(1999, 12,  31);
		TDate date2(2000, 1,  1);

		TDate date3(2001, 7,  31);
		TDate date4(2001, 8,  1);

		int difference;

		difference = date2.GetTotalDays() - date1.GetTotalDays();
		ASSERT_EQUALS(1, difference);

		difference = date4.GetTotalDays() - date3.GetTotalDays();
		ASSERT_EQUALS(1, difference);
	}


};

REGISTER_FIXTURE( Test_TDate);


