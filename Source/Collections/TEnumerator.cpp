#ifndef TENUMERATOR___INL
#define TENUMERATOR___INL

#include <stdlib.h>
#include "TEnumerator.h"

template <typename T>
TEnumerator<T>::TEnumerator()
{
	m_current = NULL;
	m_last = NULL;
	m_pitch = 0;
}

template <typename T>
TEnumerator<T>::TEnumerator(TArray<T> a)
{
	m_current = a.m_itemArray;
	m_last    = (T*)(((char*)a.m_itemArray) + a.m_itemPitch * a.m_itemCount);
	m_pitch   = a.m_itemPitch;
	m_current = Dec(m_current, 1);
}

template <typename T>
TEnumerator<T>::TEnumerator(T* firstItem, T* lastItem)
{
	m_current = firstItem;
	m_last    = lastItem;
	m_pitch   = sizeof(T);
	m_current = Dec(m_current, 1);
}

template <typename T>
T* TEnumerator<T>::Inc(T* ptr, int n)
{
	T* result = (T*)(((char*)ptr) + m_pitch * n);
	return result;
}

template <typename T>
T* TEnumerator<T>::Dec(T* ptr, int n)
{
	T* result = (T*)(((char*)ptr) - m_pitch * n);
	return result;
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
	m_current = Inc(m_current, 1);
	if (m_current==m_last)
	{
		m_current = NULL;
	}
	return m_current!=NULL;
}

#endif
