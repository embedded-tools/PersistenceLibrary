#include "minicppunit.hxx"

#include "TDateTime.h"

class Test_TDateTime : public TestFixture<Test_TDateTime>
{
  public:

    TEST_FIXTURE( Test_TDateTime)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Getter );
        TEST_CASE( Getter2 );
        TEST_CASE( Print );
        TEST_CASE( Parse1 );
        TEST_CASE( Parse2 );
        TEST_CASE( Parse3 );

        TEST_CASE( AddDay );
		TEST_CASE( AddMonth );
		TEST_CASE( AddYear );
		TEST_CASE( AddSecond );
		TEST_CASE( AddMinute );
		TEST_CASE( AddHour )
    }

    void ConstructDestruct()
    {
        TDateTime dt(2000,12,31);
	}

    void Getter()
    {
        TDateTime dt (2000,12,31);
        ASSERT(dt.GetYear()==2000);
        ASSERT(dt.GetMonth()==12);
        ASSERT(dt.GetDay()==31);
        ASSERT(dt.GetHour()==0);
        ASSERT(dt.GetMinute()==0);
        ASSERT(dt.GetSecond()==0);
    }

    void Getter2()
    {
        TDateTime dt (2000,12,31, 1, 50, 59);
        ASSERT(dt.GetYear()==2000);
        ASSERT(dt.GetMonth()==12);
        ASSERT(dt.GetDay()==31);
        ASSERT(dt.GetHour()==1);
        ASSERT(dt.GetMinute()==50);
        ASSERT(dt.GetSecond()==59);
    }

    void Print()
    {
        TDateTime dt(2000,12,31,1,50,59);

        char text[32];
        dt.PrintDateTime(text, 32);       
    }

    void Parse1()
    {
        TDateTime dt("2000-12-31");
        ASSERT(dt.GetYear()==2000);
        ASSERT(dt.GetMonth()==12);
        ASSERT(dt.GetDay()==31);
        ASSERT(dt.GetHour()==0);
        ASSERT(dt.GetMinute()==0);
        ASSERT(dt.GetSecond()==0);
    }

    void Parse2()
    {
        TDateTime dt("2000-12-31T12:34:56");
        ASSERT(dt.GetYear()==2000);
        ASSERT(dt.GetMonth()==12);
        ASSERT(dt.GetDay()==31);
        ASSERT(dt.GetHour()==12);
        ASSERT(dt.GetMinute()==34);
        ASSERT(dt.GetSecond()==56);
    }

    void Parse3()
    {
        TDateTime dt("2000-12-31T12:34:56.123");
        ASSERT(dt.GetYear()==2000);
        ASSERT(dt.GetMonth()==12);
        ASSERT(dt.GetDay()==31);
        ASSERT(dt.GetHour()==12);
        ASSERT(dt.GetMinute()==34);
        ASSERT(dt.GetSecond()==56);
        ASSERT(dt.GetMillisecond()==123);
    }

	void AddDay()
	{
		TDateTime date(2015, 12, 31);
		ASSERT(date.GetYear()==2015);
		ASSERT(date.GetMonth()==12);
		ASSERT(date.GetDay()==31);

		TDateTime date2 = date.AddDay(60);
		ASSERT(date2.GetYear()==2016);
		ASSERT(date2.GetMonth()==2);
		ASSERT(date2.GetDay()==29);

		char text[16];
		date2.PrintDate(text, 16);

		TDateTime date3 = date2.AddDay(10167);
		date3.PrintDate(text, 16);
		ASSERT(date3.GetYear()==2043);
		ASSERT(date3.GetMonth()==12);
		ASSERT(date3.GetDay()==31);

		TDateTime date4 = date3.AddDay(-60);
		date4.PrintDate(text, 16);
		ASSERT(date4.GetYear()==2043);
		ASSERT(date4.GetMonth()==11);
		ASSERT(date4.GetDay()==1);

		TDateTime date5 = date4.AddDay(-10167);
		date5.PrintDate(text, 16);
		ASSERT(date5.GetYear()==2015);
		ASSERT(date5.GetMonth()==12);
		ASSERT(date5.GetDay()==31);
	}

	void AddMonth()
	{
		TDateTime date(2015, 12, 31);
		ASSERT(date.GetYear()==2015);
		ASSERT(date.GetMonth()==12);
		ASSERT(date.GetDay()==31);

		TDateTime date2 = date.AddMonth(2);
		ASSERT(date2.GetYear()==2016);
		ASSERT(date2.GetMonth()==3);
		ASSERT(date2.GetDay()==1);

		TDateTime date3(2016, 8, 31);
		TDateTime date4 = date3.AddMonth(1);
		ASSERT(date4.GetYear()==2016);
		ASSERT(date4.GetMonth()==10);
		ASSERT(date4.GetDay()==1);
	}

	void AddYear()
	{
		TDateTime date(2016, 2, 29);

		ASSERT(date.GetYear()==2016);
		ASSERT(date.GetMonth()==2);
		ASSERT(date.GetDay()==29);

		TDateTime date2 = date.AddYear();
		ASSERT(date2.GetYear()==2017);
		ASSERT(date2.GetMonth()==3);
		ASSERT(date2.GetDay()==1);

		TDateTime date3 = date.AddYear(-1);
		ASSERT(date3.GetYear()==2015);
		ASSERT(date3.GetMonth()==3);
		ASSERT(date3.GetDay()==1);
	}

	void AddSecond()
	{
		char text[32];

		TDateTime time(1900,1,1, 12,30,15);
		ASSERT(time.GetHour()==12);
		ASSERT(time.GetMinute()==30);
		ASSERT(time.GetSecond()==15);

		TDateTime time2 = time.AddSeconds(10000);
		time2.PrintTimeFull(text,32);
		ASSERT(time2.GetHour()==15);
		ASSERT(time2.GetMinute()==16);
		ASSERT(time2.GetSecond()==55);

		TDateTime time3 = time2.AddSeconds(-10000);
		time3.PrintTimeFull(text,32);        
		ASSERT(time3.GetHour()==12);
		ASSERT(time3.GetMinute()==30);
		ASSERT(time3.GetSecond()==15);
	}

	void AddMinute()
	{
		char text[32];

		TDateTime time(1900,1,1, 23,50,45);
		ASSERT(time.GetHour()==23);
		ASSERT(time.GetMinute()==50);
		ASSERT(time.GetSecond()==45);

		TDateTime time2 = time.AddMinutes(230);
		time2.PrintTimeFull(text,32);
		ASSERT(time2.GetHour()==3);
		ASSERT(time2.GetMinute()==40);
		ASSERT(time2.GetSecond()==45);

		TDateTime time3 = time2.AddMinutes(-175);
		time3.PrintTimeFull(text,32);        
		ASSERT(time3.GetHour()==0);
		ASSERT(time3.GetMinute()==45);
		ASSERT(time3.GetSecond()==45);

		TDateTime time4 = time3.AddMinutes(-65);
		time4.PrintTimeFull(text,32);        
		ASSERT(time4.GetHour()==23);
		ASSERT(time4.GetMinute()==40);
		ASSERT(time4.GetSecond()==45);

		TDateTime time5 = time.AddMinutes(3000);
		ASSERT(time5.GetYear()==1900);
		ASSERT(time5.GetMonth()==1);
		ASSERT(time5.GetDay()==3);
		

	}

	void AddHour()
	{
		char text[32];

		TDateTime time(1900,1,1, 12,50,45);
		ASSERT(time.GetHour()==12);
		ASSERT(time.GetMinute()==50);
		ASSERT(time.GetSecond()==45);

		TDateTime time2 = time.AddHours(16);
		time2.PrintTimeFull(text,32);
		ASSERT(time2.GetHour()==4);
		ASSERT(time2.GetMinute()==50);
		ASSERT(time2.GetSecond()==45);

		TDateTime time3 = time2.AddHours(-16);
		time3.PrintTimeFull(text,32);        
		ASSERT(time3.GetHour()==12);
		ASSERT(time3.GetMinute()==50);
		ASSERT(time3.GetSecond()==45);

		TDateTime time4 = time3.AddHours(-16);
		time4.PrintTimeFull(text,32);        
		ASSERT(time4.GetHour()==20);
		ASSERT(time4.GetMinute()==50);
		ASSERT(time4.GetSecond()==45);
	}



};

REGISTER_FIXTURE( Test_TDateTime);


