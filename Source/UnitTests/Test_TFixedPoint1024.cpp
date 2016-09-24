#include "minicppunit.hxx"
#include "TFixedPoint1024.h"

class Test_TFixedPoint1024 : public TestFixture<Test_TFixedPoint1024>
{
  public:

    TEST_FIXTURE( Test_TFixedPoint1024)
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
    }


    void ConstructDestruct()
    {
        TFixedPoint1024 x,y,z;

        ASSERT(x.Trunc()==0);
        ASSERT(y.Round()==0);
        ASSERT(z.Sgn()==0);
	}

    void Assign()
    {
        TFixedPoint1024 x(10);
        ASSERT(x.Trunc()==10);
        ASSERT(x.Round()==10);

        TFixedPoint1024 y(15,8);
        ASSERT(y.Trunc()==1);
        ASSERT(y.Round()==2);

        TFixedPoint1024 z = y;
        ASSERT(z.Trunc()==1);
        ASSERT(z.Round()==2);

        TFixedPoint1024 z2;
        z2 = z;
        ASSERT(z.Trunc()==1);
        ASSERT(z.Round()==2);
    }

    void Add()
    {
        TFixedPoint1024 a, b, c;
        a = 3;
        b = 4;
        c = a + b;
        ASSERT(c.Trunc()==7);
    }

    void Sub()
    {
        TFixedPoint1024 a, b, c;
        a = 3;
        b = 4;
        c = a - b;
        ASSERT(c.Trunc()==-1);
    }

    void Mul()
    {
        TFixedPoint1024 a, b, c;
        a = 3;
        b = 4;
        c = a * b;
        ASSERT(c.Trunc()==12);
    }

    void Div()
    {
        TFixedPoint1024 a, b, c;
        a = 3;
        b = 4;
        c = (a / b) * 20;
        ASSERT(c.Trunc()==15);
    }

    void Neg()
    {
        TFixedPoint1024 x = 20;
        ASSERT(x.Trunc()==20);

        x = -x;
        ASSERT(x.Trunc()==-20);
    }

    void Trunc()
    {
        TFixedPoint1024 x(12,8);
        ASSERT(x.Trunc()==1);

        x = -x;
        ASSERT(x.Trunc()==-1);
    }

    void Round()
    {
        TFixedPoint1024 x(12,8);
        ASSERT(x.Round()==2);

        x = -x;
        ASSERT(x.Round()==-1);
    }

    void Frac()
    {
        TFixedPoint1024 x(7,4);
        TFixedPoint1024 y(3,4);

        ASSERT(x.Frac()==y);
    }

    void Compare()
    {
        TFixedPoint1024 x(33,10);
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

REGISTER_FIXTURE( Test_TFixedPoint1024);


