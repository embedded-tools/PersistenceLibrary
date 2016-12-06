#include "UnitTests.h"
#include "TRandom.h"

class Test_TRandom : public TestFixture<Test_TRandom>
{
  public:

    TEST_FIXTURE( Test_TRandom)
    {
        TEST_CASE( TestRandom1 );
        TEST_CASE( TestRandom2 );
    }


    void TestRandom1()
    {
        TRandom rnd;
        int n1 = rnd.GetRandomNumber(49)+1;
        int n2 = rnd.GetRandomNumber(49)+1;
        int n3 = rnd.GetRandomNumber(49)+1;
        int n4 = rnd.GetRandomNumber(49)+1;
        int n5 = rnd.GetRandomNumber(49)+1;
        int n6 = rnd.GetRandomNumber(49)+1;
        int n7 = rnd.GetRandomNumber(49)+1;
        int n8 = rnd.GetRandomNumber(49)+1;
        int n9 = rnd.GetRandomNumber(49)+1;
        int n10 = rnd.GetRandomNumber(49)+1;
	}

    void TestRandom2()
    {
        unsigned char array[256];
        TRandom rnd;
        rnd.GetRandomByteArray(array, sizeof(array));

    }

};

REGISTER_FIXTURE( Test_TRandom);


