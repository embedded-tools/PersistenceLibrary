/*
 * Persistence Library / Collections / TList
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

#ifndef TLIST_INL
#define TLIST_INL

#include "TList.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

template<typename T>
TList<T>::TList(int capacity)
{
    m_dataCount  = 0;
    m_dataMaxCount    = 0;
    m_dataArray       = NULL;	
	SetCapacity(capacity);
}

template<typename T>
TList<T>::TList(const TList<T>& list)
{
	m_dataCount       = 0;
	m_dataMaxCount    = 0;
	m_dataArray       = NULL;	

	for(int i = 0; i<list.Count(); i++)
	{
		Add(list.m_dataArray[i]);
	}
}

template<typename T>
TList<T>::~TList()
{
    if (m_dataArray!=NULL)
    {
        free(m_dataArray);
    }
    m_dataArray = NULL;
    m_dataCount = 0;
    m_dataMaxCount = 0;
}

template<typename T>
TEnumerator<T> TList<T>::GetEnumerator()
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

template<typename T>
short TList<T>::Count() const
{
	return m_dataCount;
}

template<typename T>
bool TList<T>::Add(T R)
{
	if (m_dataCount==32767) 
	{
		return false;
	}
	short oldm_dataCount = m_dataCount;
	if (SetCount(m_dataCount+1))
	{
		if (m_dataCount>0)
		{
			m_dataArray[m_dataCount-1]  = R;
		}		
        return true;
	}
    return false;
};

template<typename T>
bool TList<T>::Remove(T R)
{
    bool result = false;
	for(short i = 0; i<m_dataCount-1; i++)
	{
		if (m_dataArray[i]==R)
		{
			 RemoveAt(i);
             result = true;
		}
	}
    return result;
};

template<typename T>
void TList<T>::RemoveAt(short index)
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

template<typename T>
void TList<T>::Insert (short index, T x)
{
    if (index<0) index = 0;
    if (index>m_dataCount) index = m_dataCount;

	if (SetCount(m_dataCount+1))
	{
		if (index<m_dataCount-1)
		{
			memmove(&m_dataArray[index+1],&m_dataArray[index],(m_dataCount-1-index)*sizeof(T));
		}
		m_dataArray[index]=x;
	}
};

template<typename T>
bool TList<T>::Contains (T x) const
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

template<typename T>
short TList<T>::IndexOf (T x, short startIndex) const
{
    bool result=-1;
	if (startIndex<0)
	{
		return -1;
	}
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

template<typename T>
short TList<T>::LastIndexOf (T x) const
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

template<typename T>
short TList<T>::Capacity() const
{
    return m_dataMaxCount;
};


template<typename T>
bool TList<T>::SetCount(short count)
{
    if (count<0) count = 0;

	if (count>m_dataMaxCount)
	{
		short newCapacity = m_dataMaxCount;
		while (newCapacity<count)
		{
			if (newCapacity>=16384)
			{
				newCapacity = 32767;
			} else {
				newCapacity += newCapacity;
				if (newCapacity<8) newCapacity = 8;
			}
		}
		if (!SetCapacity(newCapacity))
		{
			return false;
		}
	}
    m_dataCount = count;
    return true;
};

template<typename T>
bool TList<T>::SetCapacity(short reservedCapacity)
{
	if (reservedCapacity==0)
	{
		if (m_dataArray!=NULL)
		{
			free(m_dataArray);
			m_dataArray = NULL;
		}
		m_dataCount = 0;
		m_dataMaxCount = 0;
		return true;
	}

	if (m_dataArray==NULL)
	{
		m_dataArray = (T*) malloc(reservedCapacity*sizeof(T));
		memset((void*) m_dataArray, 0, reservedCapacity*sizeof(T));
	} else {
		m_dataArray = (T*) realloc(m_dataArray, reservedCapacity*sizeof(T));
		if (m_dataCount>reservedCapacity)
		{
			m_dataCount = reservedCapacity;
		}
	}
	if (m_dataArray==NULL)
	{
		m_dataCount = 0;
		m_dataMaxCount = 0;
		return false;
	}
	m_dataMaxCount = reservedCapacity;	
	return true;		
}


template<typename T>
void TList<T>::Clear(bool unallocMemory)
{
    if (unallocMemory)
    {
        if (m_dataArray!=NULL)
        {
            free(m_dataArray);
        }
        m_dataArray = NULL;        
        m_dataMaxCount = 0;
    }
    m_dataCount = 0;
};

template<typename T>
void  TList<T>::Reverse()
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

template<typename T>
void  TList<T>::Sort(bool ascending, bool deleteDoubleEntries)
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

template<typename T>
TArray<T> TList<T>::ToArray()
{
	TArray<T> result(m_dataArray, m_dataCount);
	return result;
}

template<typename T>
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

template<typename T>
T& TList<T>::Items (short index)
{
	return (*this)[index];
}

template<typename T>
TList<T>& TList<T>::operator = (const TList<T>& list)
{
	Clear();
	for(int i = 0; i<list.Count(); i++)
	{
		Add(list.m_dataArray[i]);
	}
	return *this;
}

#ifdef STL_STYLE

template<typename T>
T* TList<T>::begin()
{
	if (m_dataArray==NULL) 
    {
        SetCapacity(8);
        if (m_dataArray==NULL) return NULL;
    }
	return &m_dataArray[0];
}

template<typename T>
T* TList<T>::end()
{
    if (m_dataArray==NULL) 
    {
        SetCapacity(8);
        if (m_dataArray==NULL) return NULL;
    }
	return &m_dataArray[m_dataCount];
}

template<typename T>
const T* TList<T>::cbegin() const
{
    if (m_dataArray==NULL) 
    {
        SetCapacity(8);
        if (m_dataArray==NULL) return NULL;
    }
    return &m_dataArray[0];
}

template<typename T>
const T* TList<T>::cend() const
{
    if (m_dataArray==NULL) 
    {
        SetCapacity(8);
        if (m_dataArray==NULL) return NULL;
    }
    return &m_dataArray[m_dataCount];
}

template<typename T>
T* TList<T>::data()
{
	return m_dataArray;
}

template<typename T>
T& TList<T>::at(int i)
{
	if ((i>=0) && (i<m_dataCount))
	{
		return m_dataArray[i];
	};

	static T tmp;
	memset((void*)&tmp, 0, sizeof(T));
	return tmp;
}

template<typename T>
void TList<T>::push_back(T value)
{
	Add(value);
}

template<typename T>
void TList<T>::push_front(T value)
{
	Insert(0, value);
}

template<typename T>
void TList<T>::pop_back()
{
	RemoveAt(m_dataCount-1);
}

template<typename T>
void TList<T>::pop_front()
{
	RemoveAt(0);
}

template<typename T>
void TList<T>::reserve(int i)
{
    if (i>=32767) i = 32767;
    return SetCount((short)i);
}

template<typename T>
T TList<T>::front()
{
	return (*this)[0];
}

template<typename T>
T TList<T>::back()
{
	return (*this)[m_dataCount-1];
}

template<typename T>
T* TList<T>::insert(T* it, T value)
{    
    if (it<m_dataArray) return NULL;
    if (it>(m_dataArray+m_dataMaxCount)) return NULL;

    if (m_dataCount>0)
    {
        if (!SetCount(m_dataCount+1))
        {
            return NULL;
        }
        T* dst = end() - 1;
        T* src = dst   - 1;
        while(it!=dst)
        {
            *dst-- = *src--;
        }
    } else {
        if (!SetCount(m_dataCount+1))
        {
            return NULL;
        }
    }
    *it = value;
    return it;
}

template<typename T>
void TList<T>::splice(T* position, TList<T>& list, T* first, T* last)
{
    if (first==NULL)
    {
        first=list.begin();
        last =list.end();
    }
    if (last==NULL)
    {
        last = first + 1;
    }    
    T* it = first;

    int n = list.size();
    while(it!=last)
    {
        if (n==0) break; //avoids infinite loop
        n--;

        if (insert(position++, *it++ )==NULL)
        {
            return;
        }        
    }    
}

template<typename T>
T* TList<T>::erase(T* first, T* last)
{
    if (first<m_dataArray) return NULL;
    if (first>(m_dataArray+m_dataMaxCount)) return NULL;

    if (last==NULL) last = first+1;

    while(true)
    {
        if (m_dataCount==0) break;

        if (first==end()) break;

        T* src = first + 1;
        T* dst = first;
        while(src!=end())
        {
            *dst++ = *src++;
        }
        if (!SetCount(m_dataCount-1)) break;

        first++;        
        if (first==last)
        {
            break;
        }   
    }
    return first;
}

template<typename T>
void TList<T>::unique()
{
    int n = m_dataCount;
    for(short i = m_dataCount-1; i>0; i--)
    {
        if (m_dataArray[i]==m_dataArray[i-1])
        {
            for(short j = i; j<m_dataCount-1; j++)
            {
                m_dataArray[j] = m_dataArray[j+1];
            }
            if (n>0) n--;
        }        
    }
    SetCount(n);
}

template<typename T>
bool TList<T>::empty()
{
	return m_dataCount==0;
}

template<typename T>
int TList<T>::size()
{
	return m_dataCount;
}

template<typename T>
int TList<T>::max_size()
{
	return 32767;
}

template<typename T>
void TList<T>::clear()
{
	Clear();
}

template<typename T>
void TList<T>::reverse()
{
	Reverse();
}

template<typename T>
void TList<T>::sort()
{
	Sort();
}

#endif


#endif
