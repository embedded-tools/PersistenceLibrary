#include "minicppunit.hxx"

#include "TDictionary.h"
#include "TString.h"
#include "TArray.h"

struct MyTestStruct1
{
	bool enabled;
	int  a,b;
};

class Test_TDictionary : public TestFixture<Test_TDictionary>
{
  public:

    TEST_FIXTURE( Test_TDictionary)
    {
        TEST_CASE( ConstructDestruct );
		TEST_CASE( DefaultValue );
		TEST_CASE( AddItems1);
		TEST_CASE( AddItems2);
		TEST_CASE( AddManyItems);;
		TEST_CASE( Enumerator );
        TEST_CASE( Clear );
		TEST_CASE( StringTest );
		TEST_CASE( ToArray );
		TEST_CASE( KeysToArray );
		TEST_CASE( ValuesToArray );

#ifdef STL_STYLE
		TEST_CASE( STL_Style );
#endif
    }

    void ConstructDestruct()
    {
        TDictionary<short, char> dict;
        TDictionary<char, short> dict2;
        TDictionary<long, long>  dict3;
	}

	void DefaultValue()
	{
		TDictionary<short, MyTestStruct1> dict;
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


	void AddItems1()
	{
		TDictionary<char, long> dict;

		ASSERT(dict.Count()==0);

		dict.AddKeysAutomatically = false;		
		dict[11] = 10;
		ASSERT(dict.Count()==0);

		dict.Add(11, 10);
		ASSERT_EQUALS(1, (int)dict.Count());
		ASSERT_EQUALS(10, dict[11]);

		dict.Add(TPair<char, long>(12, 20));
		ASSERT_EQUALS(2, (int)dict.Count());
		ASSERT_EQUALS(20, dict[12]);

		dict.Add(13, 30);
		ASSERT_EQUALS(3, (int)dict.Count());
		ASSERT_EQUALS(30, dict[13]);

		dict.Add(TPair<char, long>(14, 40));
		ASSERT_EQUALS(4, (int)dict.Count());
		ASSERT_EQUALS(40, dict[14]);
	}

	void AddItems2()
	{
		TDictionary<short, char> dict;
		dict.AddKeysAutomatically = true;

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

        dict[2] = 13;
        ASSERT_EQUALS(2, (long)dict.Count());
        ASSERT(dict.ContainsKey(2));
        ASSERT_EQUALS(13, (long)dict[2]);
	}

    void AddManyItems()
    {
        TDictionary<int, int> dict;
		dict.AddKeysAutomatically = true;
		int i;

        for (i = 1; i<100000; i++)
        {
            dict[i+1000] = i+2000;
        }
		dict[100000] = 10;

        ASSERT_EQUALS(32767, (long)dict.Count());

		bool match = true;
		int  val;
		for (i = 1; i<=32767; i++)
		{
			val = dict[i+1000];
			if (val!=(i+2000))
			{
				match = false;
			}					
		}
		ASSERT(match);
    }

	void Enumerator()
	{
		TDictionary<int, int> dict;
		dict.AddKeysAutomatically = false;

		dict.Add(11, 10);
		dict.Add(12, 20);
		dict.Add(13, 30);
		dict.Add(14, 40);
		dict.Add(15, 50);

		TEnumerator<TPair<int, int> > enumerator = dict.GetEnumerator();
		

		bool b1 = enumerator.MoveNext(); ASSERT(b1);
		TPair<int,int> val1 = enumerator.Current(); 
		ASSERT_EQUALS(11, val1.first); 
		ASSERT_EQUALS(10, val1.second); 
		

		bool b2 = enumerator.MoveNext(); ASSERT(b2);
		TPair<int,int> val2 = enumerator.Current(); 
		ASSERT_EQUALS(12, val2.first); 
		ASSERT_EQUALS(20, val2.second); 
		

		bool b3 = enumerator.MoveNext(); ASSERT(b3);
		TPair<int,int> val3 = enumerator.Current(); 
		ASSERT_EQUALS(13, val3.first); 
		ASSERT_EQUALS(30, val3.second); 		

		bool b4 = enumerator.MoveNext(); ASSERT(b4);
		TPair<int,int> val4 = enumerator.Current(); 
		ASSERT_EQUALS(14, val4.first); 
		ASSERT_EQUALS(40, val4.second); 
		
		bool b5 = enumerator.MoveNext(); ASSERT(b5);
		TPair<int,int> val5 = enumerator.Current(); 
		ASSERT_EQUALS(15, val5.first); 
		ASSERT_EQUALS(50, val5.second); 		

		bool b6 = enumerator.MoveNext(); ASSERT(!b6);		
	}

    void Clear()
    {
        TDictionary<char, char> dict;

        dict.Clear();
        dict.Clear();
        dict.Clear();

        ASSERT(dict.Count()==0);

        dict.Add(1,10);
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

	void StringTest()
	{
		TDictionary<TString, TString> dict;
		
		dict.Add("VZP",  "Vseobecna");
		dict.Add("VoZP", "Vojenska");
	}

	void ToArray()
	{
		TDictionary<int, int> dict;
		dict.Add(11, 100);
		dict.Add(12, 200);
		dict.Add(13, 300);

		TArray<TPair<int,int> > a = dict.ToArray();
		TArray<TPair<int,int> > b = a;

		ASSERT_EQUALS(3,   b.Length());
		ASSERT_EQUALS(11,  b[0].first);
		ASSERT_EQUALS(100, b[0].second);
		ASSERT_EQUALS(12,  b[1].first);
		ASSERT_EQUALS(200, b[1].second);
		ASSERT_EQUALS(13,  b[2].first);
		ASSERT_EQUALS(300, b[2].second);
	}

	void KeysToArray()
	{
		TDictionary<int, int> dict;
		dict.Add(11, 100);
		dict.Add(12, 200);
		dict.Add(13, 300);

		TArray<int> a = dict.KeysToArray();
		TArray<int> b = a;

		ASSERT_EQUALS(3,   b.Length());
		ASSERT_EQUALS(11,  b[0]);
		ASSERT_EQUALS(12,  b[1]);
		ASSERT_EQUALS(13,  b[2]);

	}

	void ValuesToArray()
	{
		TDictionary<int, int> dict;
		dict.Add(11, 100);
		dict.Add(12, 200);
		dict.Add(13, 300);

		TArray<int> a = dict.ValuesToArray();
		TArray<int> b = a;

		ASSERT_EQUALS(3,   b.Length());
		ASSERT_EQUALS(100, b[0]);
		ASSERT_EQUALS(200, b[1]);
		ASSERT_EQUALS(300, b[2]);
	}

#ifdef STL_STYLE
	void STL_Style()
	{
		TDictionary<int, int> dict;

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
		for(TDictionary<int, int>::iterator it = dict.begin(); it!=dict.end(); it++)
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

REGISTER_FIXTURE( Test_TDictionary);


