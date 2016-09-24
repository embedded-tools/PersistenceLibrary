#include "minicppunit.hxx"
#include "TStaticQueue.h"

class Test_TQueue : public TestFixture<Test_TQueue>
{
  public:

    TEST_FIXTURE( Test_TQueue)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Enqueue_Dequeue );
        TEST_CASE( FifoCacheTest );
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
    

};

REGISTER_FIXTURE( Test_TQueue);


