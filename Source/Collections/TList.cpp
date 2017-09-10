/*
 * Persistence Library / Collections / TList
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

#ifndef TLIST_INL
#define TLIST_INL

#include "tlist.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

template<class T>
TList<T>::TList()
{
	m_dataStatic = false;
    m_dataCount  = 0;
    m_dataMaxCount    = 0;
	m_dataStatic = false;
    m_dataArray       = NULL;	
}

template<class T>
TList<T>::~TList()
{
    Clear();
}


template<class T>
void* TList<T>::First()
{
	m_dataIterator = 0;
	if (m_dataIterator<m_dataCount)
	{
		return &m_dataArray[m_dataIterator];
	}
	return NULL;
}

template<class T>
void* TList<T>::Next()
{
	m_dataIterator++;
	if (m_dataIterator<m_dataCount)
	{
		return &m_dataArray[m_dataIterator];
	}
	return NULL;
}

template<class T>
short TList<T>::Count()
{
	return m_dataCount;
}

template<class T>
void TList<T>::Add(T R)
{
	short oldm_dataCount = m_dataCount;
    SetCount(m_dataCount+1);
	if (oldm_dataCount!=m_dataCount)
	{
		m_dataArray[m_dataCount-1]  = R;
	}
};


template<class T>
void TList<T>::Del(short index)
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

template<class T>
void TList<T>::Insert (short index, T x)
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

template<class T>
bool TList<T>::Contains (T x)
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

template<class T>
short TList<T>::IndexOf (T x)
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

template<class T>
short TList<T>::Capacity()
{
    return m_dataMaxCount;
};


template<class T>
short TList<T>::SetCount(short count)
{
    if (count<0) count = 0;

	if (!m_dataStatic)
	{
		if (count == 0)
		{
			if (m_dataArray!=NULL)
			{
				free(m_dataArray);
			}
			m_dataArray = NULL;
			m_dataCount = 0;
			m_dataMaxCount = 0;
			return m_dataCount;
		}

		if (count>m_dataMaxCount)
		{
			while (m_dataMaxCount<count)
			{
				if (m_dataMaxCount*sizeof(T)<1024)
				{
					m_dataMaxCount += m_dataMaxCount;
					if (m_dataMaxCount<8) m_dataMaxCount = 8;
				} else {
					m_dataMaxCount += 1024/sizeof(T);
				}
			}
			if (m_dataArray==NULL)
			{
				m_dataArray = (T*) malloc(m_dataMaxCount*sizeof(T));
				memset((void*)m_dataArray, 0, m_dataMaxCount*sizeof(T));
			} else {

				m_dataArray = (T*) realloc(m_dataArray,m_dataMaxCount*sizeof(T));
				memset((void*)(m_dataArray+count), 0, (m_dataMaxCount-count)*sizeof(T));
			}
		}
	} else {
		if (count>=m_dataMaxCount)
		{
			 count = m_dataMaxCount;
		}
	}
    m_dataCount = count;
    return m_dataCount;
};

template<class T>
void TList<T>::SetCapacity(short reservedCapacity)
{
	if (!m_dataStatic)
	{
		m_dataCount = 0;
		m_dataMaxCount   = 0;
		if (m_dataArray!=NULL)
		{
			free(m_dataArray);
			m_dataArray = NULL;
		}
		if (reservedCapacity>0)
		{
			m_dataArray = (T*) malloc(reservedCapacity*sizeof(T));
			memset((void*) m_dataArray, 0, reservedCapacity*sizeof(T));
			m_dataMaxCount = reservedCapacity;
		}
	}
}


template<class T>
void TList<T>::Clear()
{
    SetCount(0);    
};

template<class T>
T& TList<T>::operator [] (short index)
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
