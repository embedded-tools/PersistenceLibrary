/*
 * Persistence Library / Collections / TObjectList
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

#ifndef TOBJECTLIST_INL
#define TOBJECTLIST_INL

#include "tobjectlist.h"
#include <stddef.h>
#include <stdlib.h>


template<class T>
TObjectList<T>::TObjectList()
{
    m_dataPointerCount  = 0;
    m_dataPointerMaxCount    = 0;
	m_dataStatic = false;
    m_dataPointerArray      = NULL;
}

template<class T>
TObjectList<T>::~TObjectList()
{
    Clear();
	if (!m_dataStatic)
	{
		if (m_dataPointerArray!=NULL) free(m_dataPointerArray);
		m_dataPointerArray=NULL;
	}    
}

template<class T> T* TObjectList<T>::Add()
{
    SetCount(m_dataPointerCount+1);
    T* result = new T();
    m_dataPointerArray[m_dataPointerCount-1]=result;
    return result;
};

template<class T>
bool TObjectList<T>::Add(T* R)
{    
	int oldCount = m_dataPointerCount;
    int newCount = SetCount(m_dataPointerCount+1);
    if (newCount>0)
    {
        m_dataPointerArray[newCount-1]=R;
    }
	return oldCount!=newCount;
};


template<class T>
bool TObjectList<T>::Del(int id)
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

template<class T>
bool TObjectList<T>::Del(T* R)
{
    for (int i=0;i<m_dataPointerCount;i++)
    {
        if (m_dataPointerArray[i]==R)
        {
            Del(i);
            return true;
        }
    }
	return false;
};

template<class T>
T* TObjectList<T>::Insert (int i)
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

template<class T>
void TObjectList<T>::Insert (int i, T* R)
{
    SetCount(m_dataPointerCount+1);

    if (i<0) i=0;
    if (i<m_dataPointerCount)
    {
        memmove(&m_dataPointerArray[i+1],&m_dataPointerArray[i],(m_dataPointerCount-1-i)*4);
    }
    m_dataPointerArray[i]=R;
};

template<class T>
bool  TObjectList<T>::Contains(T* R)
{
	int i;
	bool result = false;
    for (i=0; i<m_dataPointerCount-1;i++)
    {
        if (m_dataPointerArray[i]==R)
		{
			result = true;
		}
    }
	return result;
}

template<class T>
short TObjectList<T>::Count()
{
    return m_dataPointerCount;
};

template<class T>
short TObjectList<T>::Capacity()
{
    return m_dataPointerMaxCount;
}

template<class T>
short TObjectList<T>::SetCount(short count)
{
	if (!m_dataStatic)
	{
		if (count == 0)
		{
			if (m_dataPointerArray!=NULL)
			{
				free(m_dataPointerArray);
			}
			m_dataPointerArray = NULL;
			m_dataPointerCount = 0;
			m_dataPointerMaxCount = 0;
			return m_dataPointerCount;
		}
		if (count>m_dataPointerMaxCount)
		{			
			while (m_dataPointerMaxCount<count)
			{
				if (m_dataPointerMaxCount*sizeof(T)<1024)
				{
					m_dataPointerMaxCount += m_dataPointerMaxCount;					
					if (m_dataPointerMaxCount<8) m_dataPointerMaxCount = 8;
				} else {
					m_dataPointerMaxCount += 1024/sizeof(T);
				}
			}
			if (m_dataPointerArray==NULL)
			{
				m_dataPointerArray = (T**) malloc(m_dataPointerMaxCount*sizeof(T*));
				if (m_dataPointerArray==NULL)
				{
					m_dataPointerCount=0;
					m_dataPointerMaxCount=0;
					return 0;                
				}
				memset(m_dataPointerArray, 0, m_dataPointerMaxCount*sizeof(T*));
			} else {
				m_dataPointerArray = (T**) realloc(m_dataPointerArray,m_dataPointerMaxCount*sizeof(T*));
				if (m_dataPointerArray==NULL)
				{
					m_dataPointerCount=0;
					m_dataPointerMaxCount=0;
					return 0;
				}
				for(int i = m_dataPointerCount; i<m_dataPointerMaxCount; i++)
				{
					m_dataPointerArray[i] = NULL;
				}
			}
		}
	}
    m_dataPointerCount = count;
    return m_dataPointerCount;
};

template<class T>
short TObjectList<T>::SetCountAndCreate(short count)
{
    SetCount(count);
    for (short i=0;i<m_dataPointerCount;i++) 
    {
        m_dataPointerArray[i]=new T();
    }
    return m_dataPointerCount;
};

template<class T>
void TObjectList<T>::SetCapacity(short reservedCapacity)
{
    m_dataPointerCount = 0;
    m_dataPointerMaxCount   = reservedCapacity;
    if (m_dataPointerArray==NULL)
    {
        m_dataPointerArray = (T**) malloc(reservedCapacity*sizeof(T*));
        memset(m_dataPointerArray, 0, reservedCapacity*sizeof(T*));
    } else {
        m_dataPointerArray = (T**) realloc(m_dataPointerArray, m_dataPointerMaxCount*sizeof(T*));
        for(int i = m_dataPointerCount; i<m_dataPointerMaxCount; i++)
        {
            m_dataPointerArray[i] = NULL;
        }
    }
    m_dataPointerCount = 0;
}


template<class T>
void TObjectList<T>::Clear()
{
    SetCount(0);    
	memset(m_dataPointerArray, m_dataPointerMaxCount*sizeof(T*), 0);
};

template<class T>
void TObjectList<T>::UnallocAndClear()
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

template<class T>
void* TObjectList<T>::First()
{
	m_dataIterator = 0;
	if (m_dataPointerCount==0) return NULL;
    return m_dataPointerArray[m_dataIterator++];
};

template<class T>
void* TObjectList<T>::Next()
{
	if (m_dataIterator>m_dataPointerCount) return NULL;
    return m_dataPointerArray[m_dataIterator++];
};

template<class T>
T* TObjectList<T>::operator [] (short id)
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

template<class T>
T* TObjectList<T>::Items (short id)
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



template<class T>
short TObjectList<T>::IndexOf(T* R)
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
