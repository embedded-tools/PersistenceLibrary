#include "minicppunit.hxx"

#include "TSortedDictionary.h"

struct MyTestStruct1
{
	bool enabled;
	int  a,b;
};

class Test_TSortedDictionary : public TestFixture<Test_TSortedDictionary>
{
  public:

    TEST_FIXTURE( Test_TSortedDictionary)
    {
        TEST_CASE( ConstructDestruct );
		TEST_CASE( DefaultValue );
		TEST_CASE( AddItems0);
		TEST_CASE( AddItems1);
		TEST_CASE( AddItems2);
		TEST_CASE( AddItems3);
        TEST_CASE( AddItems4);
        TEST_CASE( DeleteItems);
        TEST_CASE( ContainsKey);
        TEST_CASE( Clear );
#ifdef STL_STYLE
		TEST_CASE( STL_Style );
#endif
    }

    void ConstructDestruct()
    {
        TSortedDictionary<short, char> dict;
        TSortedDictionary<char, short> dict2;
        TSortedDictionary<long, long>  dict3;
	}

	void DefaultValue()
	{
		TSortedDictionary<short, MyTestStruct1> dict;
		dict.AddKeysAutomatically = true;

		MyTestStruct1* def = dict.GetDefaultValue();
		def->a = 5;
		def->b = 10;
		def->enabled = false;
		
		dict[51].enabled = true;
		ASSERT_EQUALS(5,  dict[51].a);
		ASSERT_EQUALS(10, dict[51].b);

		dict[100].a = 0;
		ASSERT_EQUALS(10, dict[100].b);
		ASSERT(!dict[100].enabled);
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
		dict.AddKeysAutomatically = true;

        dict[1] = 10000000;
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
		dict.AddKeysAutomatically = true;

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

        dict.Add(1, 11);
        ASSERT_EQUALS(1, (long)dict.Count());
        ASSERT(dict.ContainsKey(1));
        ASSERT_EQUALS(11, (long)dict[1]);

        dict[1] = 12;
        ASSERT_EQUALS(1, (long)dict.Count());
        ASSERT(dict.ContainsKey(1));
        ASSERT_EQUALS(12, (long)dict[1]);

        dict.Add(2, 13);
        ASSERT_EQUALS(2, (long)dict.Count());
        ASSERT(dict.ContainsKey(2));
        ASSERT_EQUALS(13, (long)dict[2]);
	}

    void AddItems4()
    {
        TSortedDictionary<short, long> dict;
		dict.AddKeysAutomatically = true;
		long i;

        for (i = 10; i<200; i++)
        {
            dict[(short)(i+1000)] = i+2000;
        }
        for (i = 0; i<10; i++)
        {
            dict[(short)(i+1000)] = i+2000;
        }                                                                                                         
        ASSERT_EQUALS(200, (long)dict.Count());

        for (i = 10; i<200; i++)
        {
            dict[(short)(i+1000)] = i+2000;
        }
        ASSERT_EQUALS(200, (long)dict.Count());
    }

    void DeleteItems()
    {
        TSortedDictionary<short, char> dict;
        dict.AddKeysAutomatically = true;
        dict[10] = 11;
        dict[20] = 21;
        dict[30] = 31;
        dict[40] = 41;
        dict[50] = 51;
        ASSERT_EQUALS(5, (int)dict.Count());   

        ASSERT(dict.ContainsKey(30));
        dict.Remove(30);        
        ASSERT(!dict.ContainsKey(30));
        ASSERT_EQUALS(4, (int)dict.Count());   

        ASSERT(dict.ContainsKey(20));
        dict.Remove(20);        
        ASSERT(!dict.ContainsKey(20));
        ASSERT_EQUALS(3, (int)dict.Count());   

        ASSERT(dict.ContainsKey(10));
        dict.Remove(10);        
        ASSERT(!dict.ContainsKey(10));
        ASSERT_EQUALS(2, (int)dict.Count());   

        ASSERT(dict.ContainsKey(50));
        dict.Remove(50);        
        ASSERT(!dict.ContainsKey(50));
        ASSERT_EQUALS(1, (int)dict.Count());   

        ASSERT(dict.ContainsKey(40));
        dict.Remove(40);        
        ASSERT(!dict.ContainsKey(40));
        ASSERT_EQUALS(0, (int)dict.Count());   
    }


    void ContainsKey()
    {
        TSortedDictionary<short, long> dict;
		dict.AddKeysAutomatically = true;
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

        dict.Add(1, 10);
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

#ifdef STL_STYLE
	void STL_Style()
	{
		TSortedDictionary<int, int> dict;

		ASSERT(dict.data()==dict.begin());
		ASSERT(dict.empty());
		ASSERT_EQUALS(0, dict.size());

		dict.insert(TPair<int, int>(11, 101));
		dict.insert(TPair<int, int>(12, 102));
		dict.insert(TPair<int, int>(13, 103));
		dict.insert(TPair<int, int>(14, 104));
		dict.insert(15, 105);

		ASSERT_EQUALS(5, dict.size());

		dict.at(11) = 11;
		dict.at(13) = 13;
		dict.at(15) = 15;

		int valuesCount = 0;
		int values[10];
		int keys[10];
		for(TSortedDictionary<int, int>::iterator it = dict.begin(); it!=dict.end(); it++)
		{
			keys[valuesCount] = it->first;
			values[valuesCount++] = it->second;
		}
		ASSERT_EQUALS(11 , keys[0]);
		ASSERT_EQUALS(12,  keys[1]);
		ASSERT_EQUALS(13,  keys[2]);
		ASSERT_EQUALS(14,  keys[3]);
		ASSERT_EQUALS(15,  keys[4]);

		ASSERT_EQUALS(11 , values[0]);
		ASSERT_EQUALS(102, values[1]);
		ASSERT_EQUALS(13,  values[2]);
		ASSERT_EQUALS(104, values[3]);
		ASSERT_EQUALS(15,  values[4]);

		dict.clear();
		ASSERT(dict.empty());
		ASSERT_EQUALS(0, dict.size());
	}
#endif

};

REGISTER_FIXTURE( Test_TSortedDictionary);


