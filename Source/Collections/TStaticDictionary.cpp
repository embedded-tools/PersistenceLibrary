/*
 * Persistence Library / Collections / TStaticDictionary
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

#ifndef TSTATICDICTIONARY_INL
#define TSTATICDICTIONARY_INL

#include "TStaticDictionary.h"
#include <stdlib.h>
#include <string.h>

template<typename K, typename V, int N>
TStaticDictionary<K,V,N>::TStaticDictionary()
{
    m_dataCount  = 0;
    m_dataMaxCount = N;
    memset((void*)&m_keyArray,   0, sizeof(m_keyArray));
    memset((void*)&m_valueArray, 0, sizeof(m_valueArray));
}

template<typename K, typename V, int N>
TStaticDictionary<K,V,N>::~TStaticDictionary()
{
    Clear();
}

template<typename K, typename V, int N>
void TStaticDictionary<K,V,N>::Add(K key, V value)
{
	int oldm_dataCount = m_dataCount;
    if (m_dataCount<m_dataMaxCount)
    {
		m_keyArray[m_dataCount]   = key;
		m_valueArray[m_dataCount] = value;
        m_dataCount++;
	}
};


template<typename K, typename V, int N>
void TStaticDictionary<K,V,N>::Del(K key)
{
	int index = -1;
	for(int i = 0; i<m_dataCount; i++)
	{
		if (m_keyArray[i] == key) 
		{
			index = i;
		}
	}
	if (index==-1) return;		
    for (int j=index;j<m_dataCount-1;j++)
    {
        m_keyArray[j]=m_keyArray[j+1];
		m_valueArray[j]=m_valueArray[j+1];
    }
    m_dataCount--;
};

template<typename K, typename V, int N>
bool TStaticDictionary<K,V,N>::ContainsKey (K key)
{
    bool result=false;
    for(int i = 0; i<m_dataCount; i++)
    {
        if (m_keyArray[i]==key)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<typename K, typename V, int N>
bool TStaticDictionary<K,V,N>::ContainsValue (V value)
{
    bool result=false;
    for(int i = 0; i<m_dataCount; i++)
    {
        if (m_valueArray[i]==value)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<typename K, typename V, int N>
short TStaticDictionary<K,V,N>::Count()
{
    return m_dataCount;
};

template<typename K, typename V, int N>
short TStaticDictionary<K,V,N>::MaxCount()
{
    return N;
};

template<typename K, typename V, int N>
K&  TStaticDictionary<K,V,N>::Key(int index)
{
	if ((index>=0) || (index<m_dataCount))
	{
		return m_keyArray[index];
	}
	static K result;
	return result;
}

template<typename K, typename V, int N>
V&  TStaticDictionary<K,V,N>::Value(int index)
{
    if ((index>=0) || (index<m_dataCount))
    {
        return m_valueArray[index];
    }
    static V result;
    return result;
}



template<typename K, typename V, int N>
int TStaticDictionary<K,V,N>::Capacity()
{
    return m_dataMaxCount;
};

template<typename K, typename V, int N>
void TStaticDictionary<K,V,N>::Clear()
{
    m_dataCount = 0;
};

template<typename K, typename V, int N>
V& TStaticDictionary<K,V,N>::operator [] (K key)
{
	for(int i2 = 0; i2<m_dataCount; i2++)
	{
		if (m_keyArray[i2] == key)
		{
			return m_valueArray[i2];
		}
	}
    if (m_dataCount<m_dataMaxCount)
    {
        m_keyArray[m_dataCount] = key;

		//sets default value
		m_valueArray[m_dataCount] = m_defaultValue;
        return m_valueArray[m_dataCount++];
    }        

	static V value;
    memset(&value, 0, sizeof(value));
    return value;
}

template<typename K, typename V, int N>
V* TStaticDictionary<K,V,N>::GetDefaultValue()
{
	return &m_defaultValue;	
}

#endif
