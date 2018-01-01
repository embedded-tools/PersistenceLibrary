#ifndef TSAFEARRAY___H
#define TSAFEARRAY___H

template <typename T, int N> class TSafeArray
{
private:
	T 		m_dataArray[N];		   

public:
	TSafeArray();
	T& operator [] (int index);

	operator const char*();
	operator T*();
	operator void*();

	int Length() { return N; };
	T*  Pointer() { return m_dataArray; };
	const char* PChar() { (const char*) return m_dataArray; };
};

#include "TSafeArray.cpp"


#endif