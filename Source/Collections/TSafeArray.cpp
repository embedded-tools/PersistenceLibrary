#ifndef TSAFEARRAY_INL
#define TSAFEARRAY_INL

#include "TSafeArray.h"

#include <string.h>

template<typename T, int N>
TSafeArray<T,N>::TSafeArray()
{
	memset(m_dataArray, 0, sizeof(m_dataArray));
}

template<typename T, int N>
T& TSafeArray<T,N>::operator [] (int index)
{	
	static T tmp;
	if ((index<0) || (index>=N))
	{
		return tmp;
	}
	return m_dataArray[index];
}

template<typename T, int N>
TSafeArray<T,N>::operator const char*()
{
	return (const char*)(&m_dataArray[0]);
}

template<typename T, int N>
TSafeArray<T,N>::operator T*()
{
	return (T*)(&m_dataArray[0]);
}

template<typename T, int N>
TSafeArray<T,N>::operator void*()
{
	return (void*)(&m_dataArray[0]);
}

#endif