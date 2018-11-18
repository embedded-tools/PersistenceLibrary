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
		int n1 = TRandom::GetRandomNumber(49)+1;
        int n2 = TRandom::GetRandomNumber(49)+1;
        int n3 = TRandom::GetRandomNumber(49)+1;
        int n4 = TRandom::GetRandomNumber(49)+1;
        int n5 = TRandom::GetRandomNumber(49)+1;
        int n6 = TRandom::GetRandomNumber(49)+1;
        int n7 = TRandom::GetRandomNumber(49)+1;
        int n8 = TRandom::GetRandomNumber(49)+1;
        int n9 = TRandom::GetRandomNumber(49)+1;
        int n10 = TRandom::GetRandomNumber(49)+1;
	}

    void TestRandom2()
    {
        unsigned char array[256];
		TRandom::GetRandomByteArray(array, sizeof(array));

    }

};

REGISTER_FIXTURE( Test_TRandom);


