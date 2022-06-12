#include "UnitTests.h"
#include "TRandom.h"

class Test_TRandom : public TestFixture<Test_TRandom>
{
  public:

    TEST_FIXTURE( Test_TRandom)
    {
        TEST_CASE( TestRandom1 );
        TEST_CASE( TestRandom2 );
		TEST_CASE( TestRandom3 );
    }


    void TestRandom1()
    {      
		int n1 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n2 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n3 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n4 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n5 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n6 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n7 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n8 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n9 = TRandom::GetRandomNumber(49)+1;  // random number 1 - 49
        int n10 = TRandom::GetRandomNumber(49)+1; // random number 1 - 49
	}

    void TestRandom2()
    {
        unsigned char array[256];
		TRandom::GetRandomByteArray(array, sizeof(array)); //random byte array
    }

	void TestRandom3()
	{
		unsigned long lottery1[6];
		unsigned long lottery2[5];
		unsigned long lottery3[10];
		unsigned long lottery4[2];

		TRandom::GetRandomNumberArray(49, lottery1, 6);  //loterry 6  numbers in range 1 - 49
		TRandom::GetRandomNumberArray(50, lottery2, 5);  //loterry 5  numbers in range 1 - 50
		TRandom::GetRandomNumberArray(80, lottery3, 10); //loterry 10 numbers in range 1 - 80
		TRandom::GetRandomNumberArray(10, lottery4, 2);  //loterry 2  numbers in range 1 - 10


	}

};

REGISTER_FIXTURE( Test_TRandom);


