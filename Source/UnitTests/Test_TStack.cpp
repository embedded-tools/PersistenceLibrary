#include "minicppunit.hxx"
#include "TStack.h"

class Test_TStack : public TestFixture<Test_TStack>
{
  public:

    TEST_FIXTURE( Test_TStack)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Push_Pop );
        TEST_CASE( FifoCacheTest );
#ifdef STL_STYLE
		TEST_CASE( STL_Style );
#endif
    }

    void ConstructDestruct()
    {
        TStack<long,10> queue;
	}

    void Push_Pop()
    {
        TStack<short,1024> queue;
        ASSERT(queue.IsEmpty());

        bool res;
        
        res = queue.Push(1);        
        ASSERT(res);
        ASSERT(!queue.IsEmpty());        

        res = queue.Push(2);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());        

        res = queue.Push(3);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());
        
        short i1, i2, i3;

        res = queue.Pop(i1);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());        
        ASSERT(i1 == 3);

        res = queue.Pop(i2);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());
        ASSERT(i2 == 2);

        res = queue.Pop(i3);
        ASSERT(res);
        ASSERT(queue.IsEmpty());
        ASSERT(i3 == 1);

        res = queue.Pop(i3);
        ASSERT(!res);
        ASSERT(queue.IsEmpty());

        res = queue.Pop(i3);
        ASSERT(!res);
        ASSERT(queue.IsEmpty());

        res = queue.Pop(i3);
        ASSERT(!res);
        ASSERT(queue.IsEmpty());
    }

    void FifoCacheTest()
    {
        TStack<short,1024> fifo;

        //preparation
        fifo.Push(1);
        fifo.Push(2);
        fifo.Push(3);
        short i1, i2, i3;
        fifo.Pop(i1);
        fifo.Pop(i2);
        fifo.Pop(i3);

        //test
        short i = 0;
        while(true)
        {
            if (fifo.IsFull()) break;            
            i++;
            fifo.Push(i);
        }
        ASSERT(i==1024);

        i = 1024;
        short e = 0;
        while(true)
        {
            if (fifo.IsEmpty()) break;
            fifo.Pop(e);            
            ASSERT_EQUALS(i, e);
			i--;
        }
        ASSERT(i==0);        
    }

#ifdef STL_STYLE
	void STL_Style()
	{
		TStack<int,4> queue;
		ASSERT(queue.empty());
		ASSERT(!queue.full());

		queue.push(11);
		ASSERT(!queue.empty());
		ASSERT(!queue.full());

		queue.push(22);
		ASSERT(!queue.empty());
		ASSERT(!queue.full());

		queue.push(33);
		ASSERT(!queue.empty());
		ASSERT(!queue.full());

		queue.push(44);
		ASSERT(!queue.empty());
		ASSERT(queue.full());

		ASSERT_EQUALS(44, queue.top());

		int valuesCount = 0;
		int values[10];
		while(!queue.empty())
		{
			values[valuesCount++] = queue.top();	
			queue.pop();
		}

		ASSERT_EQUALS(4, valuesCount);
		ASSERT_EQUALS(44, values[0]);
		ASSERT_EQUALS(33, values[1]);
		ASSERT_EQUALS(22, values[2]);
		ASSERT_EQUALS(11, values[3]);
	}
#endif
    

};

REGISTER_FIXTURE( Test_TStack);


