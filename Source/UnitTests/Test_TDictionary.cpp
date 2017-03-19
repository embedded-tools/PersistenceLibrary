#include "minicppunit.hxx"

#include "TSortedDictionary.h"

class Test_TSortedDictionary : public TestFixture<Test_TSortedDictionary>
{
  public:

    TEST_FIXTURE( Test_TSortedDictionary)
    {
        TEST_CASE( ConstructDestruct );
		TEST_CASE( AddItems0);
		TEST_CASE( AddItems1);
		TEST_CASE( AddItems2);
		TEST_CASE( AddItems3);
        TEST_CASE( AddItems4);
        TEST_CASE( ContainsKey );
        TEST_CASE( Clear );
    }

    void ConstructDestruct()
    {
        TSortedDictionary<short, char> dict;
        TSortedDictionary<char, short> dict2;
        TSortedDictionary<long, long>  dict3;
	}

	void AddItems0()
	{
		TSortedDictionary<short, char> dict;
		dict[1] = 10;
	}

	void AddItems1()
	{
		short index1, index2, index3, index4;
		TSortedDictionary<char, long> dict;

        dict[1] = 10000000;
		index1 = dict.FindKeyIndex(1);
		ASSERT(index1==0);
        ASSERT(dict[1]==10000000);

        dict[2] = 20000000;
		index1 = dict.FindKeyIndex(1);
		index2 = dict.FindKeyIndex(2);
		ASSERT(index1==0);		
		ASSERT(index2==1);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);

        dict[3] = 30000000;
		index1 = dict.FindKeyIndex(1);
		index2 = dict.FindKeyIndex(2);
		index3 = dict.FindKeyIndex(3);
		ASSERT(index1==0);		
		ASSERT(index2==1);
		ASSERT(index3==2);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);
        ASSERT(dict[3]==30000000);

		dict[4] = 40000000;
		index1 = dict.FindKeyIndex(1);
		index2 = dict.FindKeyIndex(2);
		index3 = dict.FindKeyIndex(3);
		index4 = dict.FindKeyIndex(4);
		ASSERT(index1==0);		
		ASSERT(index2==1);
		ASSERT(index3==2);
		ASSERT(index4==3);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);
        ASSERT(dict[3]==30000000);
        ASSERT(dict[4]==40000000);
	}

	void AddItems2()
	{
		short index1, index2, index3, index4;
		TSortedDictionary<short, char> dict;

		dict[4] = 40;
		index4 = dict.FindKeyIndex(4);
		ASSERT(index4==0);

		dict[3] = 30;
		index3 = dict.FindKeyIndex(3);
		index4 = dict.FindKeyIndex(4);		
		ASSERT(index3==0);
		ASSERT(index4==1);

		dict[2] = 20;
		index2 = dict.FindKeyIndex(2);
		index3 = dict.FindKeyIndex(3);
		index4 = dict.FindKeyIndex(4);		
		ASSERT(index2==0);
		ASSERT(index3==1);
		ASSERT(index4==2);

		dict[1] = 10;
		index1 = dict.FindKeyIndex(1);
		index2 = dict.FindKeyIndex(2);
		index3 = dict.FindKeyIndex(3);
		index4 = dict.FindKeyIndex(4);		
		ASSERT(index1==0);
		ASSERT(index2==1);
		ASSERT(index3==2);
		ASSERT(index4==3);

	}

	void AddItems3()
	{
		TSortedDictionary<short, char> dict;

        ASSERT_EQUALS(0, (long)dict.Count());
        ASSERT(!dict.ContainsKey(1));

        dict[1] = 11;
        ASSERT_EQUALS(1, (long)dict.Count());
        ASSERT(dict.ContainsKey(1));
        ASSERT_EQUALS(11, (long)dict[1]);

        dict[1] = 12;
        ASSERT_EQUALS(1, (long)dict.Count());
        ASSERT(dict.ContainsKey(1));
        ASSERT_EQUALS(12, (long)dict[1]);

        dict[1] = 13;
        ASSERT_EQUALS(1, (long)dict.Count());
        ASSERT(dict.ContainsKey(1));
        ASSERT_EQUALS(13, (long)dict[1]);
	}

    void AddItems4()
    {
        TSortedDictionary<short, long> dict;
		short i;

        for (i = 10; i<200; i++)
        {
            dict[i+1000] = i+2000;
        }
        for (i = 0; i<10; i++)
        {
            dict[i+1000] = i+2000;
        }                                                                                                         
        ASSERT_EQUALS(200, (long)dict.Count());

        for (i = 10; i<200; i++)
        {
            dict[i+1000] = i+2000;
        }
        ASSERT_EQUALS(200, (long)dict.Count());
    }

    void ContainsKey()
    {
        TSortedDictionary<short, long> dict;
		short i;

        for (i = 0; i<1000; i+=2)
        {
            dict[i] = i+1000000;
        }
        for (i = 1; i<1000; i+=2)
        {
            dict[i] = i+1000000;
        }
        ASSERT_EQUALS(1000, (long)dict.Count());

        int missing = 0;
        for (i = 1; i<1000; i++)
        {
            if (!dict.ContainsKey(i))
            {
                missing = i;
                break;
            }
        }
        ASSERT(missing==0);
    }

    void Clear()
    {
        TSortedDictionary<char, char> dict;

        dict.Clear();
        dict.Clear();
        dict.Clear();

        ASSERT(dict.Count()==0);

        dict[1] = 10;
        ASSERT(dict.Count()==1);
        ASSERT(dict[1] == 10);
        dict.Clear();

        ASSERT(dict.Count()==0);

        dict.Clear();
        dict.Clear();
        dict.Clear();
        dict.Clear();
        ASSERT(dict.Count()==0);
    }



};

REGISTER_FIXTURE( Test_TSortedDictionary);


