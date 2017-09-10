/*
 * Persistence Library / Collections / TStaticList
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

#ifndef TSTATICSTATICLIST_INL
#define TSTATICSTATICLIST_INL

#include "tstaticlist.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

template<class T, int N>
TStaticList<T, N>::TStaticList()
{
    m_dataCount  = 0;
    m_dataMaxCount = N;
	memset((void*)&m_dataArray, 0, sizeof(m_dataArray));
}

template<class T, int N>
TStaticList<T, N>::~TStaticList()
{
    Clear();
}

template<class T, int N>
void TStaticList<T, N>::Add(T R)
{
	short oldm_dataCount = m_dataCount;
    SetCount(m_dataCount+1);
	if (oldm_dataCount!=m_dataCount)
	{
		m_dataArray[m_dataCount-1]  = R;
	}
};


template<class T, int N>
void TStaticList<T, N>::Del(short index)
{
    if ( (index<0) || (index>=m_dataCount) )
    {
        //invalid index -> nothing will be deleted
        return;
    }
    for (short j=index;j<m_dataCount-1;j++)
    {
        m_dataArray[j]=m_dataArray[j+1];
    }
    SetCount(m_dataCount-1);
};

template<class T, int N>
void TStaticList<T, N>::Insert (short index, T x)
{
    if (index<0) index = 0;
    if (index>m_dataCount) index = m_dataCount;

    SetCount(m_dataCount+1);
    if (index<m_dataCount-1)
    {
        memmove(&m_dataArray[index+1],&m_dataArray[index],(m_dataCount-1-index)*sizeof(T));
    }
    m_dataArray[index]=x ;
};

template<class T, int N>
bool TStaticList<T, N>::Contains (T x)
{
    bool result=false;
    for(short i = 0; i<m_dataCount; i++)
    {
        if (m_dataArray[i]==x)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<class T, int N>
short TStaticList<T, N>::IndexOf (T x)
{
    bool result=-1;
    for(short i = 0; i<m_dataCount; i++)
    {
        if (m_dataArray[i]==x)
        {
            result = i;
            break;
        }
    }
    return result;
}


template<class T, int N>
short TStaticList<T, N>::Count()
{
    return m_dataCount;
};

template<class T, int N>
short TStaticList<T, N>::MaxCount()
{
    return N;
};


template<class T, int N>
short TStaticList<T, N>::Capacity()
{
    return m_dataMaxCount;
};


template<class T, int N>
short TStaticList<T, N>::SetCount(short count)
{
    if (count<0) count = 0;

	if (count>=m_dataMaxCount)
	{
		 count = m_dataMaxCount;
	}
    m_dataCount = count;
    return m_dataCount;
};

template<class T, int N>
void TStaticList<T, N>::Clear()
{
    SetCount(0);    
};

template<class T, int N>
void* TStaticList<T, N>::First()
{
    m_dataIterator = 0;
    if (m_dataCount>0)
    {
        return m_dataArray[0];
    }
    return NULL;    
};

template<class T, int N>
void* TStaticList<T, N>::Next()
{
    m_dataIterator++;
    if (m_dataIterator<m_dataCount)
    {
        return m_dataArray[m_dataIterator];
    }
    return NULL;
};

template<class T, int N>
T& TStaticList<T, N>::operator [] (short index)
{	
    if ((index>=0) && (index<m_dataCount))
    {
		return m_dataArray[index];
    };

	static T buf;
	memset((void*)&buf, 0, sizeof(T));
    return buf;
}

#endif
