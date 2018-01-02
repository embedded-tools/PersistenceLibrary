#include "UnitTests.h"
#include "TObjectList.h"

class TestObject
{
private:
	static int idCounter;

public:	
	int id;
	int positionX;
	int positionY;
	int sizeX;
	int sizeY;

	TestObject()
	{
		idCounter++;
		id        = idCounter;
		positionX = 0;
		positionY = 0;
		sizeX     = 640;
		sizeY     = 480;
	}
};

int TestObject::idCounter = 0;

class Test_ObjectList : public TestFixture<Test_ObjectList>
{
  public:

    TEST_FIXTURE( Test_ObjectList)
    {
		TEST_CASE( ConstructorDestructor );
        TEST_CASE( AddObjects );
		TEST_CASE( RemoveObjects );
		TEST_CASE( Enumerator );
#ifdef STL_STYLE
		TEST_CASE( STL_Style );
#endif
		
    }

	void ConstructorDestructor()
	{
		TObjectList<TestObject> list;
		ASSERT_EQUALS(0, (int)list.Count());
	}		

    void AddObjects()
    {
		TObjectList<TestObject> list;
		ASSERT_EQUALS(0, (int)list.Count());

		TestObject* o1 = list.Add();
		ASSERT_EQUALS(1, o1->id);
		ASSERT_EQUALS(0, o1->positionX);
		ASSERT_EQUALS(0, o1->positionY);
		ASSERT_EQUALS(640, o1->sizeX);
		ASSERT_EQUALS(480, o1->sizeY);
		ASSERT_EQUALS(1, (int)list.Count());

		TestObject* o2 = list.Add();
		ASSERT_EQUALS(2, o2->id);
		ASSERT_EQUALS(2, (int)list.Count());

		TestObject* o3 = list.Add();
		ASSERT_EQUALS(3, o3->id);
		ASSERT_EQUALS(3, (int)list.Count());

		TestObject* o4 = list.Add();
		ASSERT_EQUALS(4, o4->id);
		ASSERT_EQUALS(4, (int)list.Count());

		list.UnallocAndClear();
	}

	void RemoveObjects()
	{
		TObjectList<TestObject> list;

		TestObject* o = list.Add();
		list.Add();		
		list.Add();
		list.Add();
		ASSERT_EQUALS(4, (int)list.Count());

		ASSERT_EQUALS(5, list[0]->id);
		ASSERT_EQUALS(6, list[1]->id);
		ASSERT_EQUALS(7, list[2]->id);
		ASSERT_EQUALS(8, list[3]->id);

		list.Remove(o);
		ASSERT_EQUALS(3, (int)list.Count());
		ASSERT_EQUALS(6, list[0]->id);
		ASSERT_EQUALS(7, list[1]->id);
		ASSERT_EQUALS(8, list[2]->id);
		

		list.RemoveAt(1);
		ASSERT_EQUALS(2, (int)list.Count());
		ASSERT_EQUALS(6, list[0]->id);
		ASSERT_EQUALS(8, list[1]->id);

		list.RemoveAt(10);
		ASSERT_EQUALS(2, (int)list.Count());

		list.RemoveAt(-1);
		ASSERT_EQUALS(2, (int)list.Count());

		list.Remove(NULL);
		ASSERT_EQUALS(2, (int)list.Count());

		list.RemoveAt(1);
		ASSERT_EQUALS(1, (int)list.Count());
		ASSERT_EQUALS(6, list[0]->id);

		list.RemoveAt(0);
		ASSERT_EQUALS(0, (int)list.Count());
		
		list.UnallocAndClear();
	}

	void Enumerator()
	{
		TObjectList<TestObject> list;
		TestObject* o1 = list.Add();
		TestObject* o2 = list.Add();
		TestObject* o3 = list.Add();
		TestObject* o4 = list.Add();

		int         objectsCount = 0;
		TestObject* objects[10];

		TEnumerator<TestObject*> it = list.GetEnumerator();
		while(it.MoveNext())
		{
			objects[objectsCount++] = it.Current();
		}
		ASSERT_EQUALS(4, objectsCount);
		ASSERT(objects[0] == o1);
		ASSERT(objects[1] == o2);
		ASSERT(objects[2] == o3);
		ASSERT(objects[3] == o4);
	}

#ifdef STL_STYLE
	void STL_Style()
	{
		TObjectList<TestObject> list;
		ASSERT_EQUALS(0, list.size());
		ASSERT(list.front()==NULL);
		ASSERT(list.back ()==NULL);


		TestObject* o1 = new TestObject();
		list.push_back(o1);
		ASSERT(list.front()==o1);
		ASSERT(list.back() ==o1);
		ASSERT_EQUALS(9, o1->id);
		ASSERT_EQUALS(0, o1->positionX);
		ASSERT_EQUALS(0, o1->positionY);
		ASSERT_EQUALS(640, o1->sizeX);
		ASSERT_EQUALS(480, o1->sizeY);
		ASSERT_EQUALS(1, list.size());

		TestObject* o2 = new TestObject();
		list.push_back(o2);
		ASSERT_EQUALS(10, o2->id);
		ASSERT_EQUALS(2, list.size());

		TestObject* o3 = new TestObject();
		list.push_back(o3);
		ASSERT_EQUALS(11, o3->id);
		ASSERT_EQUALS(3, list.size());

		TestObject* o4 = new TestObject();
		list.push_back(o4);
		
		ASSERT_EQUALS(12, o4->id);
		ASSERT_EQUALS(4,  list.size());
		ASSERT_EQUALS(9,  list.at(0)->id);
		ASSERT_EQUALS(10, list.at(1)->id);
		ASSERT_EQUALS(11, list.at(2)->id);
		ASSERT_EQUALS(12, list.at(3)->id);
		ASSERT_EQUALS(9,  list.front()->id);
		ASSERT_EQUALS(12, list.back()->id);

		int         objectsCount = 0;
		TestObject* objects[10];

		for(TObjectList<TestObject>::iterator it = list.begin(); it!=list.end(); it++)
		{
			objects[objectsCount++] = *it;
		}
		ASSERT_EQUALS(4,  objectsCount);
		ASSERT_EQUALS(9,  objects[0]->id);
		ASSERT_EQUALS(10, objects[1]->id);
		ASSERT_EQUALS(11, objects[2]->id);
		ASSERT_EQUALS(12, objects[3]->id);

		list.unalloc_clear();
		ASSERT_EQUALS(0, list.size());
		ASSERT(list.empty());
	}
#endif

};

REGISTER_FIXTURE( Test_ObjectList);


