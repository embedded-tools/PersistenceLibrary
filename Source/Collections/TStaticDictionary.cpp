/*
 * Persistence Library / Collections / TStaticDictionary
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

#ifndef TSTATICDICTIONARY_INL
#define TSTATICDICTIONARY_INL

#include "TStaticDictionary.h"
#include <stdlib.h>
#include <string.h>

template<typename KEY, typename VALUE, int N>
TStaticDictionary<KEY, VALUE, N>::TStaticDictionary()
{
    m_dataCount  = 0;
    m_dataMaxCount = N;
    memset((void*)&m_pairArray,   0, sizeof(m_pairArray));
}

template<typename KEY, typename VALUE, int N>
TStaticDictionary<KEY, VALUE, N>::~TStaticDictionary()
{
    Clear();
}

template<typename KEY, typename VALUE, int N>
bool TStaticDictionary<KEY, VALUE, N>::Add(KEY key, VALUE value)
{
    if (m_dataCount<m_dataMaxCount)
    {
		m_pairArray[m_dataCount].first   = key;
		m_pairArray[m_dataCount].second  = value;
        m_dataCount++;
		return true;
	}
	return false;
};

template<typename KEY, typename VALUE, int N>
bool TStaticDictionary<KEY, VALUE, N>::Add(TPair<KEY,VALUE> pair)
{
	if (m_dataCount<m_dataMaxCount)
	{
		m_pairArray[m_dataCount].first   = pair.first;
		m_pairArray[m_dataCount].second  = pair.second;
		m_dataCount++;
		return true;
	}
	return false;	
};

template<typename KEY, typename VALUE, int N>
bool TStaticDictionary<KEY, VALUE, N>::Remove(KEY key)
{
	int index = -1;
	for(int i = 0; i<m_dataCount; i++)
	{
		if (m_pairArray[i].first == key) 
		{
			index = i;
		}
	}
	if (index==-1) return false;		
    for (int j=index;j<m_dataCount-1;j++)
    {
        m_pairArray[j].first  = m_pairArray[j+1].first;
		m_pairArray[j].second = m_pairArray[j+1].second;
    }
    m_dataCount--;
	return true;
};

template<typename KEY, typename VALUE, int N>
bool TStaticDictionary<KEY, VALUE, N>::ContainsKey (KEY key)
{
    bool result=false;
    for(int i = 0; i<m_dataCount; i++)
    {
        if (m_pairArray[i].first==key)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<typename KEY, typename VALUE, int N>
short TStaticDictionary<KEY, VALUE, N>::Count()
{
    return m_dataCount;
};

template<typename KEY, typename VALUE, int N>
void TStaticDictionary<KEY, VALUE, N>::Clear()
{
    m_dataCount = 0;
};

template<typename KEY, typename VALUE, int N>
VALUE& TStaticDictionary<KEY, VALUE, N>::operator [] (KEY key)
{
	for(int i2 = 0; i2<m_dataCount; i2++)
	{
		if (m_pairArray[i2].first == key)
		{
			return m_pairArray[i2].second;
		}
	}
	if (AddKeysAutomatically)
	{
		if (m_dataCount<m_dataMaxCount)
		{
			m_pairArray[m_dataCount].first = key;

			//sets default value
			m_pairArray[m_dataCount].second = m_defaultValue;
			return m_pairArray[m_dataCount++].second;
		}
	}

	static VALUE value;
    memset(&value, 0, sizeof(value));
    return value;
}

template<typename KEY, typename VALUE, int N>
VALUE* TStaticDictionary<KEY, VALUE, N>::GetDefaultValue()
{
	return &m_defaultValue;	
}

template<typename KEY, typename VALUE, int N>
TEnumerator<TPair<KEY, VALUE> > TStaticDictionary<KEY, VALUE, N>::GetEnumerator()
{
	if (m_pairArray)
	{
		TEnumerator<TPair<KEY, VALUE> > result(&m_pairArray[0], &m_pairArray[m_dataCount]);
		return result;
	} else {
		TEnumerator<TPair<KEY, VALUE> > result(NULL, NULL);
		return result;
	}		
}

#ifdef STL_STYLE
template<typename KEY, typename VALUE, int N>
TPair<KEY, VALUE>* TStaticDictionary<KEY, VALUE, N>::begin()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[0];
}

template<typename KEY, typename VALUE, int N>
TPair<KEY, VALUE>* TStaticDictionary<KEY, VALUE, N>::end()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[m_dataCount];
}

template<typename KEY, typename VALUE, int N>
TPair<KEY, VALUE>* TStaticDictionary<KEY, VALUE, N>::data()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[0];	
}

template<typename KEY, typename VALUE, int N>
VALUE& TStaticDictionary<KEY, VALUE, N>::at(KEY key)
{
	return (*this)[key];
}

template<typename KEY, typename VALUE, int N>
void TStaticDictionary<KEY, VALUE, N>::insert(KEY key, VALUE value)
{
	Add(key, value);
}

template<typename KEY, typename VALUE, int N>
void TStaticDictionary<KEY, VALUE, N>::insert(TPair<KEY,VALUE> value)
{
	Add(value.first, value.second);
}

template<typename KEY, typename VALUE, int N>
bool TStaticDictionary<KEY, VALUE, N>::empty()
{
	return m_dataCount==0;
}

template<typename KEY, typename VALUE, int N>
int TStaticDictionary<KEY, VALUE, N>::size()
{
	return m_dataCount;
}

template<typename KEY, typename VALUE, int N>
int TStaticDictionary<KEY, VALUE, N>::max_size()
{
	return 32767;
}

template<typename KEY, typename VALUE, int N>
void TStaticDictionary<KEY, VALUE, N>::clear()
{
	Clear();
}
#endif


#endif
