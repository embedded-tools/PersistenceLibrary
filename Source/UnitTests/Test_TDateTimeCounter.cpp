#include "minicppunit.hxx"

#include "TDateTimeCounter.h"

class Test_TDateTimeCounter : public TestFixture<Test_TDateTimeCounter>
{
public:

    TEST_FIXTURE( Test_TDateTimeCounter)
    {
        TEST_CASE( ConstructDestruct );
		TEST_CASE( Setter );
        TEST_CASE( Increment );
    }


    void ConstructDestruct()
    {
        TDateTimeCounter dtc;

    }

	void Setter()
	{
		TDateTimeCounter dtc;
		dtc.SetDate(2016, 10, 1);
		dtc.SetTime(23, 59, 30);
		
		ASSERT_EQUALS(2016, (int)dtc.GetYear());
		ASSERT_EQUALS(10,   (int)dtc.GetMonth());
		ASSERT_EQUALS(1,    (int)dtc.GetDay());

		ASSERT_EQUALS(23,   (int)dtc.GetHour());
		ASSERT_EQUALS(59,   (int)dtc.GetMinute());
		ASSERT_EQUALS(30,   (int)dtc.GetSecond());
	}

    void Increment()
    {
		int i,j;
        TDateTimeCounter dtc;
		dtc.SetDate(2016, 12, 31);
		dtc.SetTime(23, 59, 59);

		ASSERT_EQUALS(2016, (int)dtc.GetYear());
		ASSERT_EQUALS(12,   (int)dtc.GetMonth());
		ASSERT_EQUALS(31,   (int)dtc.GetDay());
		ASSERT_EQUALS(23,   (int)dtc.GetHour());
		ASSERT_EQUALS(59,   (int)dtc.GetMinute());
		ASSERT_EQUALS(59,   (int)dtc.GetSecond());


        for(i = 0; i<61; i++)
        {
			for(j = 0; j<50; j++)
			{
				dtc.OnTimerTickMilliSeconds(20);
			}
        }
		ASSERT_EQUALS(2017, (int)dtc.GetYear());
		ASSERT_EQUALS(1,    (int)dtc.GetMonth());
		ASSERT_EQUALS(1,    (int)dtc.GetDay());
		ASSERT_EQUALS(0,    (int)dtc.GetHour());
		ASSERT_EQUALS(1,    (int)dtc.GetMinute());
		ASSERT_EQUALS(0,    (int)dtc.GetSecond());

		for(i = 0; i<61; i++)
		{
			for(j = 0; j<50; j++)
			{
				dtc.OnTimerTickMilliSeconds(-20);
			}
		}
		ASSERT_EQUALS(2016, (int)dtc.GetYear());
		ASSERT_EQUALS(12,   (int)dtc.GetMonth());
		ASSERT_EQUALS(31,   (int)dtc.GetDay());
		ASSERT_EQUALS(23,   (int)dtc.GetHour());
		ASSERT_EQUALS(59,   (int)dtc.GetMinute());
		ASSERT_EQUALS(59,   (int)dtc.GetSecond());

    }

    
};

REGISTER_FIXTURE( Test_TDateTimeCounter);


