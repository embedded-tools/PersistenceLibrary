/*
 * Persistence Library / Collections / TDictionary
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

#ifndef TDICTIONARY_INL
#define TDICTIONARY_INL

#include "TDictionary.h"
#include <string.h>

template <typename KEY, typename VALUE>
TDictionary<KEY, VALUE>::TDictionary(short capacity)
{
    m_dataCount = 0;
    m_dataMaxCount   = 0;
    m_pairArray  = NULL;
	AddKeysAutomatically = false;

	memset(&m_defaultValue, 0, sizeof(m_defaultValue));

    if (capacity>0)
    {
        SetCapacity(capacity);
    }    
}

template<typename KEY, typename VALUE>
TDictionary<KEY, VALUE>::TDictionary(const TDictionary<KEY, VALUE> &dictionary)
{
    m_dataCount = 0;
    m_dataMaxCount   = 0;
    m_pairArray  = NULL;
}

template<typename KEY, typename VALUE>
TDictionary<KEY, VALUE>::~TDictionary()
{
    Clear();
}

template<typename KEY, typename VALUE>
bool  TDictionary<KEY, VALUE>::SetCount(short count)
{
    if (count == 0)
    {
        m_dataCount = 0;
        m_dataMaxCount = 0;
		if (m_pairArray)
		{
			free(m_pairArray);
		}
		m_pairArray = NULL;
        return false;
    }

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
}

template<typename KEY, typename VALUE>
bool TDictionary<KEY, VALUE>::SetCapacity(short capacity)
{
	if (capacity==0)
	{
		if (m_pairArray!=NULL)
		{
			free(m_pairArray);
			m_pairArray = NULL;
		}
		m_dataCount = 0;
		m_dataMaxCount = 0;
		return true;
	} 

	if (m_pairArray==NULL)
	{
		m_pairArray = (TPair<KEY,VALUE>*) malloc(capacity*sizeof(TPair<KEY,VALUE>));
		memset((void*) m_pairArray, 0, capacity*sizeof(TPair<KEY,VALUE>));
	} else {
		m_pairArray = (TPair<KEY,VALUE>*) realloc(m_pairArray, capacity*sizeof(TPair<KEY,VALUE>));
		if (m_dataCount>capacity)
		{
			m_dataCount = capacity;
		}
	}
	if (m_pairArray==NULL)
	{
		m_dataCount = 0;
		m_dataMaxCount = 0;
		return false;
	}	
	m_dataMaxCount = capacity;	
	return true;		
}

template<typename KEY, typename VALUE>
TEnumerator<TPair<KEY, VALUE> > TDictionary<KEY, VALUE>::GetEnumerator()
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
template<typename KEY, typename VALUE>
TPair<KEY, VALUE>* TDictionary<KEY, VALUE>::begin()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[0];
}

template<typename KEY, typename VALUE>
TPair<KEY, VALUE>* TDictionary<KEY, VALUE>::end()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[m_dataCount];
}

template<typename KEY, typename VALUE>
const TPair<KEY, VALUE>* TDictionary<KEY, VALUE>::cbegin() const
{
    if (m_dataCount==0) return NULL;
    return (const TPair<KEY, VALUE>*)&m_pairArray[0];
}

template<typename KEY, typename VALUE>
const TPair<KEY, VALUE>* TDictionary<KEY, VALUE>::cend() const
{
    if (m_dataCount==0) return NULL;
    return (const TPair<KEY, VALUE>)*&m_pairArray[m_dataCount];
}

template<typename KEY, typename VALUE>
TPair<KEY, VALUE>* TDictionary<KEY, VALUE>::data()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[0];	
}

template<typename KEY, typename VALUE>
VALUE& TDictionary<KEY, VALUE>::at(KEY key)
{
	return (*this)[key];
}

template<typename KEY, typename VALUE>
void TDictionary<KEY, VALUE>::insert(KEY key, VALUE value)
{
	Add(key, value);
}

template<typename KEY, typename VALUE>
void TDictionary<KEY, VALUE>::insert(TPair<KEY,VALUE> value)
{
	Add(value.first, value.second);
}

template<typename KEY, typename VALUE>
bool TDictionary<KEY, VALUE>::empty()
{
	return m_dataCount==0;
}

template<typename KEY, typename VALUE>
int TDictionary<KEY, VALUE>::size()
{
	return m_dataCount;
}

template<typename KEY, typename VALUE>
int TDictionary<KEY, VALUE>::max_size()
{
	return 32767;
}

template<typename KEY, typename VALUE>
void TDictionary<KEY, VALUE>::clear()
{
	Clear();
}
#endif

template<typename KEY, typename VALUE>
bool TDictionary<KEY, VALUE>::Add(TPair<KEY,VALUE> pair)
{
	if (m_dataCount==32767)
	{
		//maximum number of items achieved
		return false;
	}

	//finds nearest index
	bool alreadyExists = false;
	for(int i = 0; i<m_dataCount; i++)
	{
		if (m_pairArray[i].first==pair.first)
		{
			alreadyExists = true;
		}
	}
	if (alreadyExists)
	{
		return false;
	}

	if (SetCount(m_dataCount+1))
	{
		if (m_dataCount>0)
		{
			m_pairArray[m_dataCount-1].first  = pair.first;
			m_pairArray[m_dataCount-1].second = pair.second;
		}
		return true;
	}

	//key was not added (maybe not enough memory)
	return false;
}

template<typename KEY, typename VALUE>
bool TDictionary<KEY, VALUE>::Add(KEY key, VALUE value)
{
	if (m_dataCount==32767)
	{
		//maximum number of items achieved
		return false;
	}

	//finds nearest index
	bool alreadyExists = false;
	for(int i = 0; i<m_dataCount; i++)
	{
		if (m_pairArray[i].first==key)
		{
			alreadyExists = true;
		}
	}
	if (alreadyExists)
	{
		return false;
	}

	if (SetCount(m_dataCount+1))
	{
		if (m_dataCount>0)
		{
			m_pairArray[m_dataCount-1].first = key;
			m_pairArray[m_dataCount-1].second = value;
		}
		return true;
	}

	//key was not added (maybe not enough memory)
	return false;
}

template<typename KEY, typename VALUE>
bool TDictionary<KEY, VALUE>::Remove(KEY key)
{
	int keyIndex = -1;
	for(short i = 0; i<m_dataCount; i++)
	{
		if (m_pairArray[i].key==key)
		{
			keyIndex = i;
		}
	}
	if (keyIndex==-1)
	{
		return false;
	}
	for(int i = keyIndex; i<m_dataCount-1; i++)
	{
		m_pairArray[i]=m_pairArray[i+1];
	}
	m_dataCount--;	
	return true;
};

template<typename KEY, typename VALUE>
short  TDictionary<KEY, VALUE>::Count()
{
	return m_dataCount;
}

template<typename KEY, typename VALUE>
bool TDictionary<KEY, VALUE>::ContainsKey(KEY key)
{
	bool keyFound = false;
	for(short i = 0; i<m_dataCount; i++)
	{
		if (m_pairArray[i].first==key)
		{
			keyFound = true;
		}
	}
	return keyFound;
}

template<typename KEY, typename VALUE>
void TDictionary<KEY, VALUE>::Clear()       
{
	if (m_pairArray!=NULL)
	{
		free(m_pairArray);
	}
	m_pairArray=NULL;
	m_dataCount = 0;
	m_dataMaxCount   = 0;
}

template<typename KEY, typename VALUE>
TPair<KEY,VALUE>* TDictionary<KEY, VALUE>::Data()
{
	return m_pairArray;
}


template<typename KEY, typename VALUE>
VALUE&   TDictionary<KEY, VALUE>::operator [] (KEY key)
{    
	//finds nearest index
	short keyIndex = -1;
	for(int i = 0; i<m_dataCount; i++)
	{
		if (m_pairArray[i].first==key)
		{
			keyIndex = i;
		}
	}
	if (keyIndex!=-1)
	{
		return m_pairArray[keyIndex].second;
	}
    
	if (AddKeysAutomatically)
	{
		if (m_dataCount<32767)
		{
			Add(key, m_defaultValue);
			if (m_dataCount>0)
			{
				return m_pairArray[m_dataCount-1].second;
			}		
		}
	}

	//returns invalid value if memory allocation failed
	static VALUE invalidValue;
	memset(&invalidValue, 0, sizeof(VALUE));
	return invalidValue;
}

template<typename KEY, typename VALUE>
VALUE* TDictionary<KEY,VALUE>::GetDefaultValue()
{
	return &m_defaultValue;	
}

#endif
