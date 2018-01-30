#include "UnitTests.h"
#include "TList.h"

class Test_TList : public TestFixture<Test_TList>
{
  public:

    TEST_FIXTURE( Test_TList)
    {
        TEST_CASE( ConstructorDestructor );
		TEST_CASE( AddItems );
		TEST_CASE( AddManyItems );
		TEST_CASE( DeleteItems );
		TEST_CASE( Assign );
		TEST_CASE( Sort );
		TEST_CASE( SortDescending );
		TEST_CASE( Reverse );
		TEST_CASE( Enumerator );
#ifdef STL_STYLE
		TEST_CASE( STL_Style );
#endif
    }


    void ConstructorDestructor()
    {
		TList<int> list;		
	}

	void AddItems()
	{
		TList<int> list;
		
		ASSERT_EQUALS(0, (int)list.Count());
		list.Add(10);
		ASSERT_EQUALS(1, (int)list.Count());
		list.Add(20);
		ASSERT_EQUALS(2, (int)list.Count());
		list.Add(30);
		ASSERT_EQUALS(3, (int)list.Count());
		list.Add(40);
		ASSERT_EQUALS(4, (int)list.Count());
		list.Add(50);
		ASSERT_EQUALS(5, (int)list.Count());

		list.Clear();

		ASSERT_EQUALS(0, (int)list.Count());
		list.Add(10);
		ASSERT_EQUALS(1, (int)list.Count());
		list.Add(20);
		ASSERT_EQUALS(2, (int)list.Count());
		list.Add(30);
		ASSERT_EQUALS(3, (int)list.Count());
		list.Add(40);
		ASSERT_EQUALS(4, (int)list.Count());
		list.Add(50);
		ASSERT_EQUALS(5, (int)list.Count());
	}

	void AddManyItems()
	{
		TList<int> list;	
        int i;
		for(i = 1; i<=100000; i++)
		{
			list.Add(i);
		}
		ASSERT_EQUALS(32767, (int)list.Count());

		bool match = true;
		for(i = 1; i<=list.Count(); i++)
		{
			if (i!=list[i-1])
			{
				match = false;
			}
		}
		ASSERT(match);
	}

	void DeleteItems()
	{
		TList<int> list;
		list.Add(10);
		list.Add(20);
		list.Add(30);
		list.Add(40);
		list.Add(50);
		ASSERT_EQUALS(5, (int)list.Count());

		ASSERT(list[0]==10);
		ASSERT(list[1]==20);
		ASSERT(list[2]==30);
		ASSERT(list[3]==40);
		ASSERT(list[4]==50);

		list.RemoveAt(-1);
		ASSERT_EQUALS(5, (int)list.Count());

		list.RemoveAt(5);
		ASSERT_EQUALS(5, (int)list.Count());

		ASSERT(list[0]==10);
		ASSERT(list[1]==20);
		ASSERT(list[2]==30);
		ASSERT(list[3]==40);
		ASSERT(list[4]==50);

		list.RemoveAt(1);
		ASSERT_EQUALS(4, (int)list.Count());
		ASSERT(list[0]==10);
		ASSERT(list[1]==30);
		ASSERT(list[2]==40);
		ASSERT(list[3]==50);

		list.RemoveAt(3);
		ASSERT_EQUALS(3, (int)list.Count());
		ASSERT(list[0]==10);
		ASSERT(list[1]==30);
		ASSERT(list[2]==40);

		list.RemoveAt(0);
		ASSERT_EQUALS(2, (int)list.Count());
		ASSERT(list[0]==30);
		ASSERT(list[1]==40);

		list.RemoveAt(1);
		ASSERT_EQUALS(1, (int)list.Count());
		ASSERT(list[0]==30);

		list.RemoveAt(0);
		ASSERT_EQUALS(0, (int)list.Count());

		list.RemoveAt(0);
		ASSERT_EQUALS(0, (int)list.Count());
	}

	void Assign()
	{
		TList<int> list;
		list.Add(10);
		list.Add(20);
		list.Add(30);
		list.Add(40);

		ASSERT_EQUALS(4, (int)list.Count());
		ASSERT(list[0]==10);
		ASSERT(list[1]==20);
		ASSERT(list[2]==30);
		ASSERT(list[3]==40);

		TList<int> list2 = list;

		ASSERT_EQUALS(4, (int)list2.Count());
		ASSERT(list[0]==10);
		ASSERT(list[1]==20);
		ASSERT(list[2]==30);
		ASSERT(list[3]==40);

		list2.Add(5);
		list2.Add(10);
		list2.Add(15);

		list2 = list;

		ASSERT_EQUALS(4, (int)list2.Count());
		ASSERT(list[0]==10);
		ASSERT(list[1]==20);
		ASSERT(list[2]==30);
		ASSERT(list[3]==40);
	}

	void Sort()
	{
		TList<float> list;
		list.Add(14.0f);
		list.Add(3.1415926535f);
		list.Add(11.0f);
		list.Add(1.0f);
		list.Add(31.0f);
		list.Add(110.4f);
		list.Add(3.14f);		
		ASSERT_EQUALS(7, (int)list.Count());

		list.Sort();
		ASSERT_EQUALS(1.0f,          list[0]);
		ASSERT_EQUALS(3.14f,         list[1]);
		ASSERT_EQUALS(3.1415926535f, list[2]);
		ASSERT_EQUALS(11.0f,         list[3]);
		ASSERT_EQUALS(14.0f,         list[4]);
		ASSERT_EQUALS(31.0f,         list[5]);
		ASSERT_EQUALS(110.4f,        list[6]); 
	}

	void SortDescending()
	{
		TList<int> list;
		list.Add(3);
		list.Add(-8);
		list.Add(100);
		list.Add(15);
		list.Add(50);
		list.Add(1);
		list.Add(2);
		list.Add(3);
		ASSERT_EQUALS(8, (int)list.Count());

		list.Sort(false, true);
		ASSERT_EQUALS(7, (int)list.Count());

		TEnumerator<int> it = list.GetEnumerator();
		int valuesCount = 0;
		int values[10];
		while(it.MoveNext())
		{
			values[valuesCount++] = it.Current();
		}
		ASSERT_EQUALS(7, valuesCount);

		ASSERT_EQUALS(100, values[0]);
		ASSERT_EQUALS(50,  values[1]);
		ASSERT_EQUALS(15,  values[2]);
		ASSERT_EQUALS(3,   values[3]);
		ASSERT_EQUALS(2,   values[4]);
		ASSERT_EQUALS(1,   values[5]);
		ASSERT_EQUALS(-8 , values[6]);		
	}

	void Reverse()
	{
		TList<int> list;
		list.Add(1);
		list.Add(2);
		list.Add(3);
		ASSERT_EQUALS(3, (int)list.Count());
		ASSERT_EQUALS(1, list[0]);
		ASSERT_EQUALS(2, list[1]);
		ASSERT_EQUALS(3, list[2]);

		list.Reverse();
		ASSERT_EQUALS(3, (int)list.Count());
		ASSERT_EQUALS(3, list[0]);
		ASSERT_EQUALS(2, list[1]);
		ASSERT_EQUALS(1, list[2]);

		list.Reverse();
		ASSERT_EQUALS(3, (int)list.Count());
		ASSERT_EQUALS(1, list[0]);
		ASSERT_EQUALS(2, list[1]);
		ASSERT_EQUALS(3, list[2]);

		list.Add(4);
		ASSERT_EQUALS(4, (int)list.Count());
		ASSERT_EQUALS(1, list[0]);
		ASSERT_EQUALS(2, list[1]);
		ASSERT_EQUALS(3, list[2]);
		ASSERT_EQUALS(4, list[3]);

		list.Reverse();
		ASSERT_EQUALS(4, (int)list.Count());
		ASSERT_EQUALS(4, list[0]);
		ASSERT_EQUALS(3, list[1]);
		ASSERT_EQUALS(2, list[2]);
		ASSERT_EQUALS(1, list[3]);
	}


	void Enumerator()
	{
		TList<int> list;
		list.Add(10);
		list.Add(20);
		list.Add(30);
		list.Add(40);
		list.Add(50);

		TEnumerator<int> it = list.GetEnumerator();

		bool b1 = it.MoveNext(); ASSERT(b1);
		int val1 = it.Current(); ASSERT_EQUALS(10, val1); 		

		bool b2 = it.MoveNext(); ASSERT(b2);
		int val2 = it.Current(); ASSERT_EQUALS(20, val2); 

		bool b3 = it.MoveNext(); ASSERT(b3);
		int val3 = it.Current(); ASSERT_EQUALS(30, val3); 

		bool b4 = it.MoveNext(); ASSERT(b4);
		int val4 = it.Current(); ASSERT_EQUALS(40, val4); 		

		bool b5 = it.MoveNext(); ASSERT(b5);
		int val5 = it.Current(); ASSERT_EQUALS(50, val5);		

		bool b6 = it.MoveNext(); ASSERT(!b6);
	}

#ifdef STL_STYLE
	void STL_Style()
	{
		TList<int> list;
		ASSERT(list.empty());

		list.push_back(20);
		ASSERT(!list.empty());
		ASSERT_EQUALS(1, (int)list.size());
		ASSERT_EQUALS(20, list.front());
		ASSERT_EQUALS(20, list.back());

		list.push_front(10);
		ASSERT(!list.empty());
		ASSERT_EQUALS(2, (int)list.size());
		ASSERT_EQUALS(10, list.front());
		ASSERT_EQUALS(20, list.back());

		list.push_back(30);
		ASSERT(!list.empty());
		ASSERT_EQUALS(3, (int)list.size());
		ASSERT_EQUALS(10, list.front());
		ASSERT_EQUALS(30, list.back());		

		list.push_front(1);
		list.push_back(40);
		ASSERT(!list.empty());
		ASSERT_EQUALS(5, (int)list.size());
		ASSERT_EQUALS(1,  list[0]);
		ASSERT_EQUALS(10, list[1]);
		ASSERT_EQUALS(20, list[2]);
		ASSERT_EQUALS(30, list[3]);
		ASSERT_EQUALS(40, list[4]);

		int values[10];
		int valuesCount = 0;
		for(TList<int>::iterator it = list.begin(); it!=list.end(); ++it)
		{
			values[valuesCount++] = *it;	
		}
		ASSERT_EQUALS(5,  valuesCount);
		ASSERT_EQUALS(1,  values[0]);
		ASSERT_EQUALS(10, values[1]);
		ASSERT_EQUALS(20, values[2]);
		ASSERT_EQUALS(30, values[3]);
		ASSERT_EQUALS(40, values[4]);

		list.reverse();
		ASSERT_EQUALS(5, (int)list.size());
		ASSERT_EQUALS(40, list[0]);
		ASSERT_EQUALS(30, list[1]);
		ASSERT_EQUALS(20, list[2]);
		ASSERT_EQUALS(10, list[3]);
		ASSERT_EQUALS(1,  list[4]);

		list.pop_front();
		ASSERT_EQUALS(4, (int)list.size());
		ASSERT_EQUALS(30, list[0]);
		ASSERT_EQUALS(20, list[1]);
		ASSERT_EQUALS(10, list[2]);
		ASSERT_EQUALS(1,  list[3]);

		list.pop_back();
		ASSERT_EQUALS(3,  (int)list.size());
		ASSERT_EQUALS(30, list[0]);
		ASSERT_EQUALS(20, list[1]);
		ASSERT_EQUALS(10, list[2]);

		list.sort();
		ASSERT_EQUALS(3,  (int)list.size());
		ASSERT_EQUALS(10, list[0]);
		ASSERT_EQUALS(20, list[1]);
		ASSERT_EQUALS(30, list[2]);

		ASSERT(!list.empty());
		list.clear();
		ASSERT(list.empty());			
	}
#endif

};

REGISTER_FIXTURE( Test_TList);


