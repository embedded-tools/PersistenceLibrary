/*
 * Persistence Library / Collections / TStaticList
 *
 * Copyright (c) 2016-2018 Ondrej Sterba <osterba@atlas.cz>
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

#include "TStaticList.h"
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
	if (SetCount(m_dataCount+1))
	{
		if (m_dataCount>0)
		{
			m_dataArray[m_dataCount-1]  = R;
		}		
	}
};


template<class T, int N>
void TStaticList<T, N>::RemoveAt(short index)
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
bool TStaticList<T, N>::Contains (T x) const
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
short TStaticList<T, N>::IndexOf (T x, short startIndex)
{
	if (startIndex<0)
	{
		return -1;
	}
    bool result=-1;
    for(short i = startIndex; i<m_dataCount; i++)
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
short TStaticList<T, N>::LastIndexOf (T x)
{
	bool result=-1;
	for(short i = m_dataCount-1; i>=0; i--)
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
bool TStaticList<T, N>::SetCount(short count)
{
    if (count<0) 
	{
		return false;
	}
	if (count>m_dataMaxCount)
	{
		return false;
	}

	m_dataCount = count;   
    return true;
};

template<class T, int N>
void TStaticList<T, N>::Clear()
{
    SetCount(0);    
};

template<class T, int N>
void  TStaticList<T, N>::Reverse()
{
	T tmp;

	int i1 = 0;
	int i2 = m_dataCount-1;	
	while(i1<i2)
	{
		tmp = m_dataArray[i1];
		m_dataArray[i1]=m_dataArray[i2];
		m_dataArray[i2]=tmp;		
		i1++; i2--;
	}
}

template<class T, int N>
void  TStaticList<T, N>::Sort(bool ascending, bool deleteDoubleEntries)
{
	T     valueExtreme;
	short valueExtremeIndex;

	for(int i = 0; i<m_dataCount; i++)
	{
		valueExtreme      = m_dataArray[i];
		valueExtremeIndex = i;
		for(int j = i+1; j<m_dataCount; j++)
		{
			if (ascending)
			{
				if (m_dataArray[j]<valueExtreme)
				{
					valueExtreme = m_dataArray[j];
					valueExtremeIndex = j;
				}
			} else {
				if (m_dataArray[j]>valueExtreme)
				{
					valueExtreme = m_dataArray[j];
					valueExtremeIndex = j;
				}
			}
		}
		if (valueExtremeIndex!=i)
		{
			m_dataArray[valueExtremeIndex]=m_dataArray[i];
			m_dataArray[i]=valueExtreme;
		}
	}
	if (deleteDoubleEntries)
	{
		for(short i = m_dataCount-1; i>0; i--)
		{
			if (m_dataArray[i]==m_dataArray[i-1])
			{
				for(short j=i; j<m_dataCount-1; j++)
				{
					m_dataArray[j]=m_dataArray[j+1];
				}
				m_dataCount--;
			}
		}
	}
}

template<typename T, int N>
TArray<T> TStaticList<T,N>::ToArray()
{
	TArray<T> result(m_dataArray, m_dataCount);
	return result;
}

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

template<typename T, int N>
T& TStaticList<T,N>::Items (short index)
{
	return (*this)[index];
}

template<class T, int N>
TEnumerator<T> TStaticList<T, N>::GetEnumerator()
{
	if (m_dataArray)
	{
		TEnumerator<T> result(&m_dataArray[0], &m_dataArray[m_dataCount]);
		return result;
	} else {
		TEnumerator<T> result(NULL, NULL);
		return result;
	}
}

#ifdef STL_STYLE

template<typename T, int N>
T* TStaticList<T, N>::begin()
{
	if (m_dataArray==NULL) return NULL;
	return &m_dataArray[0];
}

template<typename T, int N>
T* TStaticList<T, N>::end()
{
	if (m_dataArray==NULL) return NULL;
	return &m_dataArray[m_dataCount];
}

template<typename T, int N>
T* TStaticList<T, N>::data()
{
	return m_dataArray;
}

template<typename T, int N>
T& TStaticList<T, N>::at(int i)
{
	if ((i>=0) && (i<m_dataCount))
	{
		return m_dataArray[i];
	};

	static T tmp;
	memset((void*)&tmp, 0, sizeof(T));
	return tmp;
}

template<typename T, int N>
void TStaticList<T, N>::push_back(T value)
{
	Add(value);
}

template<typename T, int N>
void TStaticList<T, N>::push_front(T value)
{
	Insert(0, value);
}

template<typename T, int N>
void TStaticList<T, N>::pop_back()
{
	RemoveAt(m_dataCount-1);
}

template<typename T, int N>
void TStaticList<T, N>::pop_front()
{
	RemoveAt(0);
}

template<typename T, int N>
T TStaticList<T, N>::front()
{
	return (*this)[0];
}

template<typename T, int N>
T TStaticList<T, N>::back()
{
	return (*this)[m_dataCount-1];
}

template<typename T, int N>
bool TStaticList<T, N>::empty()
{
	return m_dataCount==0;
}

template<typename T, int N>
int TStaticList<T, N>::size()
{
	return m_dataCount;
}

template<typename T, int N>
int TStaticList<T, N>::max_size()
{
	return 32767;
}

template<typename T, int N>
void TStaticList<T, N>::clear()
{
	Clear();
}

template<typename T, int N>
void TStaticList<T, N>::reverse()
{
	Reverse();
}

template<typename T, int N>
void TStaticList<T, N>::sort()
{
	Sort();
}

#endif

#endif
