#include "UnitTests.h"
#include "TSafeArray.h"
#include <string.h>

class Test_TSafeArray : public TestFixture<Test_TSafeArray>
{
  public:

    TEST_FIXTURE( Test_TSafeArray)
    {
		TEST_CASE( ConstructorDestructor );
        TEST_CASE( ArrayZeroTest );
		TEST_CASE( ModifyCharArray );
		TEST_CASE( ModifyIntArray );
		TEST_CASE( ModifyFloatArray );
		TEST_CASE( IndexOutOfRange );
    }

	void ConstructorDestructor()
	{
		TSafeArray<int,1000> arr;
	}	

    void ArrayZeroTest()
    {
		TSafeArray<int,1000> arr;
		bool zero = true;
		for(int i = 0; i<arr.Length(); i++)
		{
			if (arr[i]!=0) zero=false;
		}		
		ASSERT(zero);
	}

	void ModifyCharArray()
	{
		TSafeArray<unsigned char,10> arr;
		arr[0] = 'T';
		arr[1] = 'e';
		arr[2] = 's';
		arr[3] = 't';

		ASSERT( strcmp(arr, "Test")==0);
	}

	void ModifyIntArray()
	{
		TSafeArray<int,10> arr;
		arr[-1] = 100;
		arr[0]  = 101;
		arr[1]  = 102;
		arr[2]  = 103;
		arr[3]  = 104;
		arr[4]  = 105;
		arr[5]  = 106;
		arr[6]  = 107;
		arr[7]  = 108;
		arr[8]  = 109;
		arr[9]  = 110;
		arr[10] = 111;

		ASSERT_EQUALS(101, arr[0]);
		ASSERT_EQUALS(102, arr[1]);
		ASSERT_EQUALS(103, arr[2]);
		ASSERT_EQUALS(104, arr[3]);
		ASSERT_EQUALS(105, arr[4]);
		ASSERT_EQUALS(106, arr[5]);
		ASSERT_EQUALS(107, arr[6]);
		ASSERT_EQUALS(108, arr[7]);
		ASSERT_EQUALS(109, arr[8]);
		ASSERT_EQUALS(110, arr[9]);
	}

	void ModifyFloatArray()
	{
		TSafeArray<float,10> arr;
		ASSERT(arr[0]==0);
		
		arr[1] = 1.0f;
		arr[5] = 5.0f;
		arr[9] = 9.0f;

		ASSERT(arr[0]==0.0f);
		ASSERT(arr[1]==1.0f);
		ASSERT(arr[2]==0.0f);
		ASSERT(arr[3]==0.0f);
		ASSERT(arr[4]==0.0f);
		ASSERT(arr[5]==5.0f);
		ASSERT(arr[6]==0.0f);
		ASSERT(arr[7]==0.0f);
		ASSERT(arr[8]==0.0f);	
		ASSERT(arr[9]==9.0f);

	}

	void IndexOutOfRange()
	{
		TSafeArray<int,10> arr;
		arr[-1] = 50;

		ASSERT_EQUALS(50, arr[-1000]);
		ASSERT_EQUALS(50, arr[-1]);
		ASSERT_EQUALS(50, arr[50]);
		ASSERT_EQUALS(50, arr[1000]);
	}
};

REGISTER_FIXTURE( Test_TSafeArray);


