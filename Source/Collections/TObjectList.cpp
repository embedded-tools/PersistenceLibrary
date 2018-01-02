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
#include <string.h>


template<class T>
TObjectList<T>::TObjectList()
{
    m_dataPointerCount  = 0;
    m_dataPointerMaxCount    = 0;
    m_dataPointerArray      = NULL;
}

template<class T>
TObjectList<T>::~TObjectList()
{
    Clear();
	if (m_dataPointerArray!=NULL) free(m_dataPointerArray);
	m_dataPointerArray=NULL;
}

template<class T> 
TEnumerator<T*> TObjectList<T>::GetEnumerator()
{
	if (m_dataPointerArray)
	{
		TEnumerator<T*> enumerator(&m_dataPointerArray[0], &m_dataPointerArray[m_dataPointerCount]);
		return enumerator;
	} else {
		TEnumerator<T*> enumerator(NULL, NULL);
		return enumerator;
	}
}

template<class T> 
T* TObjectList<T>::Add()
{
    if (SetCount(m_dataPointerCount+1))
	{		
		if (m_dataPointerCount>0)
		{
			T* result = new T();
			m_dataPointerArray[m_dataPointerCount-1]=result;
			return result;	
		}				
	}
	return NULL;
};

template<class T>
bool TObjectList<T>::Add(T* R)
{    
    if (SetCount(m_dataPointerCount+1))
	{
		if (m_dataPointerCount>0)
		{
			m_dataPointerArray[m_dataPointerCount-1]=R;
			return true;
		}		
    }
	return false;
};


template<class T>
bool TObjectList<T>::RemoveAt(int id)
{
    if (id<0) return false;
    if (id>=m_dataPointerCount) return false;
    
    for (int j=id;j<m_dataPointerCount-1;j++)
    {
        m_dataPointerArray[j]=m_dataPointerArray[j+1];
    }
    return SetCount(m_dataPointerCount-1);
};

template<class T>
bool TObjectList<T>::Remove(T* R)
{
    for (int i=0;i<m_dataPointerCount;i++)
    {
        if (m_dataPointerArray[i]==R)
        {
            RemoveAt(i);
            return true;
        }
    }
	return false;
};

template<class T>
T* TObjectList<T>::Insert (int i)
{
    if (!SetCount(m_dataPointerCount+1))
	{
		return NULL;
	}
	if (m_dataPointerArray==NULL)
	{
		return NULL;
	}

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
	if (!SetCount(m_dataPointerCount+1))
	{
		return NULL;
	}
	if (m_dataPointerArray==NULL)
	{
		return NULL;
	}

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
bool TObjectList<T>::SetCount(short count)
{
	if (count == 0)
	{
		if (m_dataPointerArray)
		{
			free(m_dataPointerArray);
		}
		m_dataPointerArray = NULL;
		m_dataPointerCount = 0;
		m_dataPointerMaxCount = 0;
		return false;
	}
	if (count>m_dataPointerMaxCount)
	{			
		short newCapacity = m_dataPointerMaxCount;
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
    m_dataPointerCount = count;
    return true;
};

template<class T>
bool TObjectList<T>::SetCountAndCreate(short count)
{
    if (!SetCount(count))
	{
		return false;
	}
	for (short i=0;i<m_dataPointerCount;i++) 
	{
        m_dataPointerArray[i]=new T();
    }
    return true;
};

template<class T>
bool TObjectList<T>::SetCapacity(short reservedCapacity)
{
    m_dataPointerMaxCount   = reservedCapacity;
	if (m_dataPointerArray==NULL)
	{
		m_dataPointerArray = (T**) malloc(m_dataPointerMaxCount*sizeof(T*));
		if (m_dataPointerArray==NULL)
		{
			m_dataPointerCount=0;
			m_dataPointerMaxCount=0;
			return false;                
		}
		memset(m_dataPointerArray, 0, m_dataPointerMaxCount*sizeof(T*));
	} else {
		m_dataPointerArray = (T**) realloc(m_dataPointerArray,m_dataPointerMaxCount*sizeof(T*));
		if (m_dataPointerArray==NULL)
		{
			m_dataPointerCount=0;
			m_dataPointerMaxCount=0;
			return false;
		}
		for(int i = m_dataPointerCount; i<m_dataPointerMaxCount; i++)
		{
			m_dataPointerArray[i] = NULL;
		}
	}
	if (m_dataPointerCount>m_dataPointerMaxCount)
	{
		m_dataPointerCount = m_dataPointerMaxCount;
	}
	return true;
}


template<class T>
void TObjectList<T>::Clear()
{
    SetCount(0);   
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

template<class T>
T** TObjectList<T>::Data()
{
	return m_dataPointerArray;
}

#ifdef STL_STYLE

template<typename T>
T** TObjectList<T>::begin()
{
	if (m_dataPointerArray==NULL) return NULL;
	return &m_dataPointerArray[0];
}

template<typename T>
T** TObjectList<T>::end()
{
	if (m_dataPointerArray==NULL) return NULL;
	return &m_dataPointerArray[m_dataPointerCount];
}

template<typename T>
T** TObjectList<T>::data()
{
	return m_dataPointerArray;
}

template<typename T>
T* TObjectList<T>::at(int i)
{
	if ((i>=0) && (i<m_dataPointerCount))
	{
		return m_dataPointerArray[i];
	};
	return NULL;
}

template<typename T>
void TObjectList<T>::push_back(T* value)
{
	Add(value);
}

template<typename T>
void TObjectList<T>::push_front(T* value)
{
	Insert(0, value);
}

template<typename T>
void TObjectList<T>::pop_back()
{
	RemoveAt(m_dataPointerCount-1);
}

template<typename T>
void TObjectList<T>::pop_front()
{
	RemoveAt(0);
}

template<typename T>
T* TObjectList<T>::front()
{
	return (*this)[0];
}

template<typename T>
T* TObjectList<T>::back()
{
	return (*this)[m_dataPointerCount-1];
}

template<typename T>
bool TObjectList<T>::empty()
{
	return m_dataPointerCount==0;
}

template<typename T>
int TObjectList<T>::size()
{
	return m_dataPointerCount;
}

template<typename T>
int TObjectList<T>::max_size()
{
	return 32767;
}

template<typename T>
void TObjectList<T>::clear()
{
	Clear();
}

template<typename T>
void TObjectList<T>::unalloc_clear()
{
	UnallocAndClear();
}

#endif

#endif
