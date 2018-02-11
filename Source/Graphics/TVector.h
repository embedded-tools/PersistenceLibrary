#ifndef TVECTOR___H
#define TVECTOR___H

#define UNIFIED_VECTOR_SIZE 840
#define UNIFIED_VECTOR_SIZE_SQRT 29
//1680 is divisible by 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 15 
//is important that this constant powered by 3 must be still
//less than 2^30 to avoid Int32 overflow

class TVector
{
public:
	int DirectionX;
	int DirectionY;

	TVector();
	TVector(short vectorX, short vectorY);
	TVector(const TVector& src);

	void Normalize();
	void Simplify();

	TVector operator * (int x);

};

#endif