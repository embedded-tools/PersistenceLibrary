#include "UnitTests.h"
#include "TEndlessLine.h"
#include "TPosition.h"
#include <stdlib.h>

class Test_TEndlessLine: public TestFixture<Test_TEndlessLine>
{
  public:

    TEST_FIXTURE( Test_TEndlessLine)
    {
        TEST_CASE( Create );
		TEST_CASE( Simplify );
		TEST_CASE( Intersection );
    }

    void Create()
    {
		TPosition point1(100,30);
		TPosition point2(-800,10);
		TPosition point3(100,31);
		TEndlessLine line1(point1, point2);
		

		int precision = 16; //precision = 1/16 of pixel size
		int onePixelSize = UNIFIED_VECTOR_SIZE / precision; 

		int dist1 = line1.Distance(point1);
		ASSERT(abs(dist1)<(onePixelSize/2));
		int dist2 = line1.Distance(point2);
		ASSERT(abs(dist2)<(onePixelSize/2)); 

		TPosition point4(110, 10);
		TPosition point5(120, 20);
		TEndlessLine line2(point4, point5);

		int dist4 = line2.Distance(point4);
		ASSERT(abs(dist4)<(onePixelSize/2));
		int dist5 = line2.Distance(point5);
 		ASSERT(abs(dist5)<(onePixelSize/2)); 

		TPosition point6(110, 10);
		TPosition point7(120, 20);
		TEndlessLine line3(point6, point7);

		int dist6 = line3.Distance(point6);
		ASSERT(abs(dist6)<(onePixelSize/2));
		int dist7 = line3.Distance(point7);
		ASSERT(abs(dist7)<(onePixelSize/2)); 
	}

	void Simplify()
	{
		TVector vec(1005, 400);
		ASSERT_EQUALS(1005, vec.DirectionX);
		ASSERT_EQUALS(400,  vec.DirectionY);

		vec.Simplify();
		ASSERT_EQUALS(201, vec.DirectionX);
		ASSERT_EQUALS(80,  vec.DirectionY);


	}

	void Intersection()
	{
		TEndlessLine line1(TPosition(0, 100), TPosition(100, 100));
		TEndlessLine line2(TPosition(30,-40), TPosition(30, 60));
		TEndlessLine line3(TPosition(110,10), TPosition(120,20));
		TEndlessLine line4(TPosition(10000, 100), TPosition(10002, 110));

		TPosition intersection1 = line1.Intersection(line2);
		ASSERT_EQUALS( 30, (int)intersection1.X);
		ASSERT_EQUALS(100, (int)intersection1.Y);

		TPosition intersection2 = line1.Intersection(line3);
		ASSERT_EQUALS(200, (int)intersection2.X);
		ASSERT_EQUALS(100, (int)intersection2.Y);

		TPosition intersection3 = line1.Intersection(line4);
		ASSERT_EQUALS(10000, (int)intersection3.X);
		ASSERT_EQUALS(100, (int)intersection3.Y);


	}

};

REGISTER_FIXTURE( Test_TEndlessLine);


