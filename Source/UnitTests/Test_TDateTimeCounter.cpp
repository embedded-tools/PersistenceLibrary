#include "minicppunit.hxx"

#include "TDateTimeCounter.h"

class Test_TDateTimeCounter : public TestFixture<Test_TDateTimeCounter>
{
public:

    TEST_FIXTURE( Test_TDateTimeCounter)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Increment );
    }


    void ConstructDestruct()
    {
        TDateTimeCounter dtc;

    }

    void Increment()
    {
        TDateTimeCounter dtc;

        for(int h = 0; h<183; h++)
        {
            for(int i = 0; i<24; i++)
            {
                for(int j=0; j<60; j++)
                {
                    for (int k=0;k<60;k++)
                    {
                        dtc.OnTimerTick();
                    }
                }
            }
        }
        int x = 0;

        for(int k = 0; k<183; k++)
        {
            for(int l = 0; l<24; l++)
            {
                for(int m=0; m<60; m++)
                {
                    for (int n=0;n<60;n++)
                    {
                        dtc.OnTimerTick();
                    }
                }
            }
        }
        int y = 0;
    }

    
};

//REGISTER_FIXTURE( Test_TDateTimeCounter);


