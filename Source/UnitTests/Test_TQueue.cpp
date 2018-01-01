#include "minicppunit.hxx"
#include "TQueue.h"

class Test_TQueue : public TestFixture<Test_TQueue>
{
  public:

    TEST_FIXTURE( Test_TQueue)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Enqueue_Dequeue );
        TEST_CASE( FifoCacheTest );
		TEST_CASE( STL_Style );
    }

    void ConstructDestruct()
    {
        TQueue<long,10> queue;
	}

    void Enqueue_Dequeue()
    {
        TQueue<short,1024> queue;
        ASSERT(queue.IsEmpty());

        bool res;
        
        res = queue.Enqueue(1);        
        ASSERT(res);
        ASSERT(!queue.IsEmpty());        

        res = queue.Enqueue(2);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());        

        res = queue.Enqueue(3);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());
        
        short i1, i2, i3;

        res = queue.Dequeue(i1);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());        
        ASSERT(i1 == 1);

        res = queue.Dequeue(i2);
        ASSERT(res);
        ASSERT(!queue.IsEmpty());
        ASSERT(i2 == 2);

        res = queue.Dequeue(i3);
        ASSERT(res);
        ASSERT(queue.IsEmpty());
        ASSERT(i3 == 3);

        res = queue.Dequeue(i3);
        ASSERT(!res);
        ASSERT(queue.IsEmpty());

        res = queue.Dequeue(i3);
        ASSERT(!res);
        ASSERT(queue.IsEmpty());

        res = queue.Dequeue(i3);
        ASSERT(!res);
        ASSERT(queue.IsEmpty());
    }

    void FifoCacheTest()
    {
        TQueue<short,1024> fifo;

        //preparation
        fifo.Enqueue(1);
        fifo.Enqueue(2);
        fifo.Enqueue(3);
        short i1, i2, i3;
        fifo.Dequeue(i1);
        fifo.Dequeue(i2);
        fifo.Dequeue(i3);

        //test
        short i = 0;
        while(true)
        {
            if (fifo.IsFull()) break;            
            i++;
            fifo.Enqueue(i);
        }
        ASSERT(i==1024);

        i = 0;
        short e = 0;
        while(true)
        {
            if (fifo.IsEmpty()) break;
            fifo.Dequeue(e);
            i++;
            ASSERT_EQUALS(i, e);
        }
        ASSERT(i==1024);        
    }

	void STL_Style()
	{
		TQueue<int,4> queue;
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

		ASSERT_EQUALS(11, queue.front());
		ASSERT_EQUALS(44, queue.back());

		int valuesCount = 0;
		int values[10];
		while(!queue.empty())
		{
			values[valuesCount++] = queue.front();	
			queue.pop();
		}

		ASSERT_EQUALS(4, valuesCount);
		ASSERT_EQUALS(11, values[0]);
		ASSERT_EQUALS(22, values[1]);
		ASSERT_EQUALS(33, values[2]);
		ASSERT_EQUALS(44, values[3]);

	}
    

};

REGISTER_FIXTURE( Test_TQueue);


