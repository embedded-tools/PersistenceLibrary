#ifndef TENDLESSLINE___H
#define TENDLESSLINE___H

#include "TPosition.h"
#include "TVector.h"

class TEndlessLine
{
public:

	TEndlessLine();
	TEndlessLine(TPosition point1, TPosition point2);
	TEndlessLine(TPosition point,  TVector   vector);

	short A,B;
	long  C;

	TPosition Intersection(TEndlessLine anotherLine);
	int       Distance(TPosition point);
	void      Normalize();

};


#endif
