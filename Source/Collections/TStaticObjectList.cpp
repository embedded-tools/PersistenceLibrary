/*
 * Persistence Library / Collections / TStaticObjectList
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
 *
 * https://github.com/embedded-tools/PersistenceLibrary
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef TSTATICOBJECTLIST_INL
#define TSTATICOBJECTLIST_INL

#include "tstaticobjectlist.h"
#include <stddef.h>
#include <stdlib.h>


template<class T, int N>
TStaticObjectList<T, N>::TStaticObjectList()
{
    m_dataPointerCount  = 0;
    m_dataPointerMaxCount    = 0;
    memset(&m_dataPointerArray, sizeof(T)*N, 0);
}

template<class T, int N>
TStaticObjectList<T, N>::~TStaticObjectList()
{
    Clear();
}

template<class T, int N> T* TStaticObjectList<T, N>::Add()
{
    SetCount(m_dataPointerCount+1);
    T* result = new T();
    m_dataPointerArray[m_dataPointerCount-1]=result;
    return result;
};

template<class T, int N>
bool TStaticObjectList<T, N>::Add(T* R)
{    
	int oldCount = m_dataPointerCount;
    int newCount = SetCount(m_dataPointerCount+1);
    if (newCount>0)
    {
        m_dataPointerArray[newCount-1]=R;
    }
	return oldCount!=newCount;
};


template<class T, int N>
bool TStaticObjectList<T, N>::Remove(int id)
{
    if (id<0) return false;
    if (id>=m_dataPointerCount) return false;
    
    for (int j=id;j<m_dataPointerCount-1;j++)
    {
        m_dataPointerArray[j]=m_dataPointerArray[j+1];
    }
    SetCount(m_dataPointerCount-1);
	return true;
};

template<class T, int N>
bool TStaticObjectList<T, N>::Remove(T* R)
{
    for (int i=0;i<m_dataPointerCount;i++)
    {
        if (m_dataPointerArray[i]==R)
        {
            Remove(i);
            return true;
        }
    }
	return false;
};

template<class T, int N>
T* TStaticObjectList<T, N>::Insert (int i)
{
    SetCount(m_dataPointerCount+1);

    if (i<0) i=0;
    if (i<m_dataPointerCount)
    {
        memmove(&m_dataPointerArray[i+1],&m_dataPointerArray[i],(m_dataPointerCount-1-i)*4);
    } else {
       i = m_dataPointerCount-1;
    }
    T* result = new T();
    m_dataPointerArray[i]=result;
    return result;
};

template<class T, int N>
void TStaticObjectList<T, N>::Insert (int i, T* R)
{
    SetCount(m_dataPointerCount+1);

    if (i<0) i=0;
    if (i<m_dataPointerCount)
    {
        memmove(&m_dataPointerArray[i+1],&m_dataPointerArray[i],(m_dataPointerCount-1-i)*4);
    }
    m_dataPointerArray[i]=R;
};

template<class T, int N>
short TStaticObjectList<T, N>::Count()
{
    return m_dataPointerCount;
};

template<class T, int N>
short TStaticObjectList<T, N>::MaxCount()
{
    return N;
};

template<class T, int N>
short TStaticObjectList<T, N>::Capacity()
{
    return m_dataPointerMaxCount;
}

template<class T, int N>
short TStaticObjectList<T, N>::SetCount(short count)
{
    if (count<0) count = 0;   
    m_dataPointerCount = count;
    return m_dataPointerCount;
};

template<class T, int N>
short TStaticObjectList<T, N>::SetCountAndCreate(short count)
{
    SetCount(count);
    for (short i=0;i<m_dataPointerCount;i++) 
    {
        m_dataPointerArray[i]=new T();
    }
    return m_dataPointerCount;
};

template<class T, int N>
void TStaticObjectList<T, N>::Clear()
{
    SetCount(0);    
	memset(m_dataPointerArray, m_dataPointerMaxCount*sizeof(T*), 0);
};

template<class T, int N>
void TStaticObjectList<T, N>::UnallocAndClear()
{
    for(short i=0; i<m_dataPointerCount; i++)
    {
        if (m_dataPointerArray[i]!=NULL)
        {
            delete m_dataPointerArray[i];
            m_dataPointerArray[i] = NULL;
        }
    }
    SetCount(0);    
	memset(m_dataPointerArray, m_dataPointerMaxCount*sizeof(T*), 0);
};

template<class T, int N>
T* TStaticObjectList<T, N>::First()
{
    m_dataIterator = 0;
    if (m_dataPointerCount>0)
    {
        return m_dataPointerArray[0];
    }
    return NULL;    
};

template<class T, int N>
T* TStaticObjectList<T, N>::Next()
{
    m_dataIterator++;
    if (m_dataIterator<m_dataPointerCount)
    {
        return m_dataPointerArray[m_dataIterator];
    }
    return NULL;
};

#ifdef STL_COMPATIBLE

template<class T, int N>
T** TStaticObjectList<T, N>::begin()
{
	return &m_dataPointerArray[0];
};

template<class T, int N>
T** TStaticObjectList<T, N>::end()
{
	return &m_dataPointerArray[m_dataPointerCount];
};

#endif

template<class T, int N>
T* TStaticObjectList<T, N>::operator [] (short id)
{
    if (id<0)
    {
        return NULL;
    }
    if (id>=m_dataPointerCount)
    {
        return NULL;
    }
    return m_dataPointerArray[id];
}

template<class T, int N>
T* TStaticObjectList<T, N>::Item (short id)
{
    if (id<0)
    {
        return NULL;
    }
    if (id>=m_dataPointerCount)
    {
        return NULL;
    }
    return m_dataPointerArray[id];
}



template<class T, int N>
short TStaticObjectList<T, N>::IndexOf(T* R)
{
    for (int i=0; i<m_dataPointerCount; i++)
    {
        if (R == m_dataPointerArray[i])
        {
            return i;
        }
    }
    return -1;
}

#endif
