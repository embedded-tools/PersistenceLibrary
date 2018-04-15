#ifndef TARRAY_INL
#define TARRAY_INL

#include "TArray.h"

#include <string.h>

template<typename T>
TArray<T>::TArray(T* pData, short itemCount, short itemPitch)
{
	if (itemPitch==0)
	{
		itemPitch = sizeof(T);
	}
	m_itemArray = pData;
	m_itemCount = itemCount;
	m_itemPitch = itemPitch;
}

template<typename T>
TArray<T>::TArray(const TArray& a)
{
	m_itemArray = a.m_itemArray;
	m_itemCount = a.m_itemCount;
	m_itemPitch = a.m_itemPitch;
}

template<typename T>
T& TArray<T>::operator [] (int index)
{	
	static T tmp;
	if ((index<0) || (index>=m_itemCount))
	{
		return tmp;
	}
	return  *((T*)( ((char*)m_itemArray)+index*m_itemPitch ));
}

template<typename T>
TArray<T>::operator T*()
{
	return (T*)(&m_dataArray[0]);
}

template<typename T>
TArray<T>::operator void*()
{
	return (void*)(&m_dataArray[0]);
}

#ifdef STL_STYLE

template<typename T>
T* TArray<T>::begin()
{
	return (&m_itemArray[0]);
}

template<typename T>
T* TArray<T>::end()
{
	return (&m_itemArray[m_itemCount]);
}
#endif

#endif