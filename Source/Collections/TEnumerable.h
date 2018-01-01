#ifndef TENUMERABLE___H
#define TENUMERABLE___H

#include "TEnumerator.h"

template <typename T> 
class TEnumerable
{
	public:
		virtual TEnumerator<T> GetEnumerator() = 0;
};

#include "TEnumerable.cpp"

#endif