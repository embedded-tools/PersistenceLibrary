#include "TEndlessLine.h"
#include "TCanvas.h"

TEndlessLine::TEndlessLine()
:
  A(0),
  B(0),
  C(0)
{
	
}

TEndlessLine::TEndlessLine(TPosition point1, TPosition point2)
{
	TVector vector = point2 - point1;

	vector.Simplify();
	A = -vector.DirectionY;
	B =  vector.DirectionX;
	C = - A * point1.X - B * point1.Y;
}

TEndlessLine::TEndlessLine(TPosition point,  TVector vector)
{
	vector.Simplify();
	A = -vector.DirectionY;
	B =  vector.DirectionX;
	C = - A * point.X - B * point.Y;
}

TPosition TEndlessLine::Intersection(TEndlessLine anotherLine)
{
	TPosition result;
	result.X = (short)
		       ( ( (this->B * anotherLine.C) - (anotherLine.B * this->C) ) /
		       ( (this->A * anotherLine.B) - (anotherLine.A * this->B) ) );

	result.Y = (short)
		       ( ( (this->A * anotherLine.C) - (anotherLine.A * this->C) ) /
	  	       (-(this->A * anotherLine.B) + (anotherLine.A * this->B) ) );

	return result;
}

int  TEndlessLine::Distance(TPosition point)
{
	return A * point.X + B * point.Y + C;

	//float  dist = (A * point.X + B * point.Y + C) / ((float) UNIFIED_VECTOR_SIZE);
}

void TEndlessLine::Normalize()
{
	int currentVectorSize;

	currentVectorSize = (A*A) + (B*B);
	currentVectorSize = TCanvas::FastSqrt(currentVectorSize);

	if (currentVectorSize>0)
	{
		A = A * UNIFIED_VECTOR_SIZE / currentVectorSize;
		B = B * UNIFIED_VECTOR_SIZE / currentVectorSize;
		C = C * UNIFIED_VECTOR_SIZE / currentVectorSize;
	}
}