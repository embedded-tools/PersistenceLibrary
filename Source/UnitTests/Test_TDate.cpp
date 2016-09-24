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




};

REGISTER_FIXTURE( Test_TDate);


