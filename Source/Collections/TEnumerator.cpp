#ifndef TENUMERATOR___INL
#define TENUMERATOR___INL

#include <stdlib.h>
#include "TEnumerator.h"

template <typename T> 
TEnumerator<T>::TEnumerator()
{
	m_current = NULL;
	m_last = NULL;
}

template <typename T> 
TEnumerator<T>::TEnumerator(T* firstItem, T* lastItem)
{
	m_current = firstItem;
	m_last    = lastItem;
	m_current--;
}

template <typename T> 
TEnumerator<T>::TEnumerator(const TEnumerator& enumerator)
{
	m_current = enumerator.m_current;
	m_last    = enumerator.m_last;
}

template <typename T> 
T& TEnumerator<T>::Current()
{
	if (m_current==NULL)
	{
		static T tmp;
		return tmp;
	}
	return *m_current;
}

template <typename T> 
bool TEnumerator<T>::MoveNext()
{
	if (m_current==NULL)
	{
		return false;
	}
	m_current++;
	if (m_current==m_last)
	{
		m_current = NULL;
	}
	return m_current!=NULL;
}


#endif