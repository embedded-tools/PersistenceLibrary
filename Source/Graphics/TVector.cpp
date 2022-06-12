#include "TVector.h"
#include "TCanvas.h"


TVector::TVector()
{
	DirectionX = 0;
	DirectionY = 0;
}

TVector::TVector(short vectorX, short vectorY)
{
	DirectionX = vectorX;
	DirectionY = vectorY;
}

TVector::TVector(const TVector& src)
{
	DirectionX = src.DirectionX;
	DirectionY = src.DirectionY;
}

void TVector::Simplify()
{
	if ((DirectionX==0) && (DirectionY!=0))
	{
		DirectionY = 1;
		return;
	}
	if ((DirectionX!=0) && (DirectionY==0))
	{
		DirectionX = 1;
		return;
	}

	while(true)
	{
		if ((DirectionX%12==0) && (DirectionY%12==0))
		{
			DirectionX /= 12;
			DirectionY /= 12;
		} else
		if ((DirectionX%11==0) && (DirectionY%11==0))
		{
			DirectionX /= 11;
			DirectionY /= 11;
		} else
		if ((DirectionX%10==0) && (DirectionY%10==0))
		{
			DirectionX /= 10;
			DirectionY /= 10;
		} else
		if ((DirectionX%9==0) && (DirectionY%9==0))
		{
			DirectionX /= 9;
			DirectionY /= 9;
		} else
		if ((DirectionX%8==0) && (DirectionY%8==0))
		{
			DirectionX /= 8;
			DirectionY /= 8;
		} else
		if ((DirectionX%7==0) && (DirectionY%7==0))
		{
			DirectionX /= 7;
			DirectionY /= 7;
		} else
		if ((DirectionX%6==0) && (DirectionY%6==0))
		{
			DirectionX /= 6;
			DirectionY /= 6;
		} else
		if ((DirectionX%5==0) && (DirectionY%5==0))
  		{
			DirectionX /= 5;
   			DirectionY /= 5;
		} else
		if ((DirectionX%4==0) && (DirectionY%4==0))
		{
			DirectionX /= 4;
			DirectionY /= 4;
		} else
		if ((DirectionX%3==0) && (DirectionY%3==0))
		{
			DirectionX /= 3;
			DirectionY /= 3;
		} else
		if ((DirectionX%2==0) && (DirectionY%2==0))
		{
			DirectionX /= 2;
			DirectionY /= 2;
		}
		else break;
	}
}

void TVector::Normalize()
{
	int currentVectorSize;

	currentVectorSize = (DirectionX*DirectionX) + (DirectionY*DirectionY);
	currentVectorSize = TCanvas::FastSqrt(currentVectorSize);

	if (currentVectorSize>0)
	{
		DirectionX = DirectionX * UNIFIED_VECTOR_SIZE / currentVectorSize;
		DirectionY = DirectionY * UNIFIED_VECTOR_SIZE / currentVectorSize;
	}
}

TVector TVector::operator * (int x)
{
	TVector result;
	long currentVectorSize;
	currentVectorSize = (DirectionX*DirectionX) + (DirectionY*DirectionY);
	if ((currentVectorSize<16687225) || (currentVectorSize>16851025))
	{
		currentVectorSize = TCanvas::FastSqrt(currentVectorSize);
	} else {
		currentVectorSize = UNIFIED_VECTOR_SIZE;
	}
	result.DirectionX = (DirectionX * x + currentVectorSize / 2 ) / currentVectorSize;
	result.DirectionY = (DirectionY * x + currentVectorSize / 2 ) / currentVectorSize;
	return result;
}

TVector& TVector::operator = (const TVector& vec)
{
	DirectionX = vec.DirectionX;
	DirectionY = vec.DirectionY;
	return *this;
}
