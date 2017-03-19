#include "minicppunit.hxx"
#include "TFixedPoint16M.h"

class Test_TFixedPoint16M : public TestFixture<Test_TFixedPoint16M>
{
  public:

    TEST_FIXTURE( Test_TFixedPoint16M)
    {
        TEST_CASE( ConstructDestruct );
        TEST_CASE( Assign );
        TEST_CASE( Add );
        TEST_CASE( Sub );
        TEST_CASE( Mul );
        TEST_CASE( Div );
        TEST_CASE( Neg );
        TEST_CASE( Trunc );
        TEST_CASE( Round );
        TEST_CASE( Frac );
        TEST_CASE( Compare );
        TEST_CASE( Sqrt );
        TEST_CASE( Sqrt3 );
    }


    void ConstructDestruct()
    {
        TFixedPoint16M x,y,z;

        ASSERT(x.Trunc()==0);
        ASSERT(y.Round()==0);
        ASSERT(z.Sgn()==0);
	}

    void Assign()
    {
        TFixedPoint16M x(10);
        ASSERT(x.Trunc()==10);
        ASSERT(x.Round()==10);

        TFixedPoint16M y(15,8);
        ASSERT(y.Trunc()==1);
        ASSERT(y.Round()==2);

        TFixedPoint16M z = y;
        ASSERT(z.Trunc()==1);
        ASSERT(z.Round()==2);

        TFixedPoint16M z2;
        z2 = z;
        ASSERT(z.Trunc()==1);
        ASSERT(z.Round()==2);
    }

    void Add()
    {
        TFixedPoint16M a, b, c;
        a = 3;
        b = 4;
        c = a + b;
        ASSERT(c.Trunc()==7);
    }

    void Sub()
    {
        TFixedPoint16M a, b, c;
        a = 3;
        b = 4;
        c = a - b;
        ASSERT(c.Trunc()==-1);
    }

    void Mul()
    {
        TFixedPoint16M a, b, c;
        a = 3;
        b = 4;
        c = a * b;
        ASSERT(c.Trunc()==12);
    }

    void Div()
    {
        TFixedPoint16M a, b, c;
        a = 3;
        b = 4;
        c = (a / b) * 20;
        ASSERT(c.Trunc()==15);
    }

    void Neg()
    {
        TFixedPoint16M x = 20;
        ASSERT(x.Trunc()==20);

        x = -x;
        ASSERT(x.Trunc()==-20);
    }

    void Trunc()
    {
        TFixedPoint16M x(12,8);
        ASSERT(x.Trunc()==1);

        x = -x;
        ASSERT(x.Trunc()==-1);
    }

    void Round()
    {
        TFixedPoint16M x(12,8);
        ASSERT(x.Round()==2);

        x = -x;
        ASSERT(x.Round()==-1);
    }

    void Frac()
    {
        TFixedPoint16M x(7,4);
        TFixedPoint16M y(3,4);

        ASSERT(x.Frac()==y);
    }

    void Sqrt()
    {
        TFixedPoint16M x(121);
        TFixedPoint16M y;
        y = x.Sqrt();
        ASSERT(y.Round()==11);
    }

    void Sqrt3()
    {
        TFixedPoint16M x(121);
        TFixedPoint16M y;
        y = x.Sqrt3();
        ASSERT(y.Round()==5);
    }

    void Compare()
    {
        TFixedPoint16M x(33,10);
        ASSERT(x>0);
        ASSERT(x>3);
        ASSERT(x>=3);
        ASSERT(x!=3);        
        ASSERT(x<4);
        ASSERT(x<=4);
        ASSERT(x!=4);
        ASSERT(x<5000);
        ASSERT(x<=5000);
        ASSERT(x>=-5000);
        ASSERT(x>-5000);

        x = 3;
        ASSERT(x>0);
        ASSERT(!(x>3));
        ASSERT(x>=3);
        ASSERT(x==3);
        ASSERT(x<4);
        ASSERT(x<=4);
        ASSERT(x!=4);
        
        x = 4;
        ASSERT(x>0);
        ASSERT(x>=3);
        ASSERT(x!=3);
        ASSERT(!(x<4));
        ASSERT(x<=4);
        ASSERT(x==4);

    }
    
};

REGISTER_FIXTURE( Test_TFixedPoint16M);


