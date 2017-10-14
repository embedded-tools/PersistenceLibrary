#include "minicppunit.hxx"

#include "TTime.h"

class Test_TTime : public TestFixture<Test_TTime>
{
  public:

    TEST_FIXTURE( Test_TTime)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Getters );
        TEST_CASE( PrintTime );
        TEST_CASE( ParseTime );
        TEST_CASE( ParseTimeLong );
        TEST_CASE( AddSecond );
        TEST_CASE( AddMinute );
        TEST_CASE( AddHour );
		TEST_CASE( GetTotalMilliseconds );
    }

    void ConstructDestruct()
    {
        TTime time(12, 30, 59);

	}

    void Getters()
    {
        TTime time(12, 30, 59);
        ASSERT(time.GetHour()==12);
        ASSERT(time.GetMinute()==30);
        ASSERT(time.GetSecond()==59);
        ASSERT(time.GetMillisecond()==0);
    }

    void PrintTime()
    {
        char text[32];

        TTime time(12, 30, 59);
        time.PrintTime(text, 32);
        ASSERT_EQUALS(text, "12:30:59");

        time.PrintTimeFull(text, 32);
        ASSERT_EQUALS(text, "12:30:59.000");
        
    }

    void ParseTime()
    {
        TTime time("12:34:56");
        ASSERT(time.GetHour()==12);
        ASSERT(time.GetMinute()==34);
        ASSERT(time.GetSecond()==56);
        ASSERT(time.GetMillisecond()==0);
    }

    void ParseTimeLong()
    {
        TTime time("12:34:56.123");
        ASSERT(time.GetHour()==12);
        ASSERT(time.GetMinute()==34);
        ASSERT(time.GetSecond()==56);
        ASSERT(time.GetMillisecond()==123);
    }

    void AddSecond()
    {
        char text[32];

        TTime time(12,30,15);
        ASSERT(time.GetHour()==12);
        ASSERT(time.GetMinute()==30);
        ASSERT(time.GetSecond()==15);

        TTime time2 = time.AddSeconds(10000);
        time2.PrintTimeFull(text,32);
        ASSERT(time2.GetHour()==15);
        ASSERT(time2.GetMinute()==16);
        ASSERT(time2.GetSecond()==55);

        TTime time3 = time2.AddSeconds(-10000);
        time3.PrintTimeFull(text,32);        
        ASSERT(time3.GetHour()==12);
        ASSERT(time3.GetMinute()==30);
        ASSERT(time3.GetSecond()==15);
    }

    void AddMinute()
    {
        char text[32];

        TTime time(23,50,45);
        ASSERT(time.GetHour()==23);
        ASSERT(time.GetMinute()==50);
        ASSERT(time.GetSecond()==45);

        TTime time2 = time.AddMinutes(230);
        time2.PrintTimeFull(text,32);
        ASSERT(time2.GetHour()==3);
        ASSERT(time2.GetMinute()==40);
        ASSERT(time2.GetSecond()==45);

        TTime time3 = time2.AddMinutes(-175);
        time3.PrintTimeFull(text,32);        
        ASSERT(time3.GetHour()==0);
        ASSERT(time3.GetMinute()==45);
        ASSERT(time3.GetSecond()==45);

        TTime time4 = time3.AddMinutes(-65);
        time4.PrintTimeFull(text,32);        
        ASSERT(time4.GetHour()==23);
        ASSERT(time4.GetMinute()==40);
        ASSERT(time4.GetSecond()==45);
    }

    void AddHour()
    {
        char text[32];

        TTime time(12,50,45);
        ASSERT(time.GetHour()==12);
        ASSERT(time.GetMinute()==50);
        ASSERT(time.GetSecond()==45);

        TTime time2 = time.AddHours(16);
        time2.PrintTimeFull(text,32);
        ASSERT(time2.GetHour()==4);
        ASSERT(time2.GetMinute()==50);
        ASSERT(time2.GetSecond()==45);

        TTime time3 = time2.AddHours(-16);
        time3.PrintTimeFull(text,32);        
        ASSERT(time3.GetHour()==12);
        ASSERT(time3.GetMinute()==50);
        ASSERT(time3.GetSecond()==45);

        TTime time4 = time3.AddHours(-16);
        time4.PrintTimeFull(text,32);        
        ASSERT(time4.GetHour()==20);
        ASSERT(time4.GetMinute()==50);
        ASSERT(time4.GetSecond()==45);
    }

	void GetTotalMilliseconds()
	{
		TTime time1(12,0,0);
		TTime time2(23,59,59,999);

		int ms1 = time1.GetTotalMilliseconds();
		int ms2 = time2.GetTotalMilliseconds();
		int dif = ms2 - ms1;

		ASSERT_EQUALS(43199999, dif);

	}
};

REGISTER_FIXTURE( Test_TTime);


