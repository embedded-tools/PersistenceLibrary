#include "minicppunit.hxx"

#include "TStaticDictionary.h"

class Test_TStaticDictionary : public TestFixture<Test_TStaticDictionary>
{
  public:

    TEST_FIXTURE( Test_TStaticDictionary)
    {
        TEST_CASE( ConstructDestruct );        
		TEST_CASE( AddItems1);
		TEST_CASE( AddItems2);
        TEST_CASE( DelItems);
        TEST_CASE( ContainsKey );
        TEST_CASE( ContainsValue );
        TEST_CASE( Clear );        
    }

    
    void ConstructDestruct()
    {
        TStaticDictionary<short, char, 8> dict;
        TStaticDictionary<char, short, 8> dict2;
        TStaticDictionary<long, long,  8>  dict3;
	}   
    
	void AddItems0()
	{
		TStaticDictionary<short, char, 8> dict;
		dict[1] = 10;
	}

	void AddItems1()
	{
		short index1, index2, index3, index4;
		TStaticDictionary<char, long, 8> dict;

        dict[1] = 10000000;
		ASSERT(dict.Key(0)==1);
        ASSERT(dict[1]==10000000);

        dict[2] = 20000000;
		ASSERT(dict.Key(0)==1);		
		ASSERT(dict.Key(1)==2);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);

        dict[3] = 30000000;
        ASSERT(dict.Key(0)==1);		
        ASSERT(dict.Key(1)==2);
        ASSERT(dict.Key(2)==3);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);
        ASSERT(dict[3]==30000000);

		dict[40] = 40000000;
        ASSERT(dict.Key(0)==1);		
        ASSERT(dict.Key(1)==2);
        ASSERT(dict.Key(2)==3);
        ASSERT(dict.Key(3)==40);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);
        ASSERT(dict[3]==30000000);
        ASSERT(dict[40]==40000000);        
	}

    void AddItems2()
    {
        short index1, index2, index3, index4;
        TStaticDictionary<char, long, 4> dict;

        dict.Add(1, 10000000);
        ASSERT(dict.Key(0)==1);
        ASSERT(dict[1]==10000000);

        dict.Add(2,20000000);
        ASSERT(dict.Key(0)==1);		
        ASSERT(dict.Key(1)==2);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);

        dict.Add(3,30000000);
        ASSERT(dict.Key(0)==1);		
        ASSERT(dict.Key(1)==2);
        ASSERT(dict.Key(2)==3);
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);
        ASSERT(dict[3]==30000000);

        dict.Add(4,40000000);
        dict.Add(5,50000000);
        dict.Add(6,60000000);
        ASSERT(dict.Count()==4);
        ASSERT(dict.Key(0)==1);		
        ASSERT(dict.Key(1)==2);
        ASSERT(dict.Key(2)==3);
        ASSERT(dict.Key(3)==4);        
        ASSERT(dict[1]==10000000);
        ASSERT(dict[2]==20000000);
        ASSERT(dict[3]==30000000);
        ASSERT(dict[4]==40000000);        
    }

    void DelItems()
    {
        TStaticDictionary<char, long, 8> dict;
        dict[10] = 10000;
        dict[20] = 20000;
        dict[30] = 30000;
        dict[40] = 40000;
        dict[50] = 50000;
        ASSERT(dict.Count()==5);

        dict.Del(30);

        ASSERT(dict.Count()==4);
        ASSERT(dict.Key(0)==10);
        ASSERT(dict.Key(1)==20);
        ASSERT(dict.Key(2)==40);
        ASSERT(dict.Key(3)==50);

        dict.Del(1);
        ASSERT(dict.Count()==4);
        ASSERT(dict.Key(0)==10);
        ASSERT(dict.Key(1)==20);
        ASSERT(dict.Key(2)==40);
        ASSERT(dict.Key(3)==50);

        dict.Del(10);
        ASSERT(dict.Count()==3);
        ASSERT(dict.Key(0)==20);
        ASSERT(dict.Key(1)==40);
        ASSERT(dict.Key(2)==50);

        dict.Del(50);
        ASSERT(dict.Count()==2);
        ASSERT(dict.Key(0)==20);
        ASSERT(dict.Key(1)==40);

        dict.Del(20);
        ASSERT(dict.Count()==1);
        ASSERT(dict.Key(0)==40);

        dict.Del(0);
        ASSERT(dict.Count()==1);

        dict.Del(40);
        ASSERT(dict.Count()==0);

    }


    void ContainsKey()
    {        
        TStaticDictionary<short, long, 10> dict;
		long i;

        for (i = 1; i<=10; i++)
        {
            dict[i] = i+1000000;
        }
        ASSERT_EQUALS(10, (long)dict.Count());

        int missing = 0;
        for (i = 1; i<=10; i++)
        {
            if (!dict.ContainsKey(i))
            {
                missing = i;
                break;
            }
        }
        ASSERT(missing==0);        
    }

    void ContainsValue()
    {        
        TStaticDictionary<short, long, 10> dict;
        long i;

        for (i = 1; i<=10; i++)
        {
            dict[i] = i+1000000;
        }
        ASSERT_EQUALS(10, (long)dict.Count());

        int missing = 0;
        for (i = 1; i<=10; i++)
        {
            if (!dict.ContainsValue(i+1000000))
            {
                missing = i;
                break;
            }
        }
        ASSERT(missing==0);        
    }


    void Clear()
    {
        TStaticDictionary<char, char, 8> dict;

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

REGISTER_FIXTURE( Test_TStaticDictionary);


