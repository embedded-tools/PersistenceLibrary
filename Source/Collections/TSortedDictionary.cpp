/*
 * Persistence Library / Collections / TSortedDictionary
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

#ifndef TDICTIONARY_INL
#define TDICTIONARY_INL

#include "TSortedDictionary.h"
#include <string.h>

template <typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::TSortedDictionary(short capacity)
{
    m_dataCount = 0;
    m_dataMaxCount   = 0;
    m_valueArray  = NULL;
    m_keyArray   = NULL;
	memset(&m_defaultValue, 0, sizeof(m_defaultValue));

    if (capacity>0)
    {
        SetCapacity(capacity);
    }    
}

template<typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::TSortedDictionary(TSortedDictionary<KEY, VALUE> &dictionary)
{
    m_dataCount = 0;
    m_dataMaxCount   = 0;
    m_valueArray  = NULL;
    m_keyArray   = NULL;
}

template<typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::~TSortedDictionary()
{
    Clear();
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::Clear()       
{
    if (m_valueArray!=NULL)
    {
        free(m_valueArray);
    }
    m_valueArray=NULL;
    if (m_keyArray!=NULL)
    {
        free(m_keyArray);
    }
    m_keyArray=NULL;
    m_dataCount = 0;
    m_dataMaxCount   = 0;
}

template<typename KEY, typename VALUE>
short  TSortedDictionary<KEY, VALUE>::Count()
{
    return m_dataCount;
}

template<typename KEY, typename VALUE>
short  TSortedDictionary<KEY, VALUE>::Capacity()
{
    return m_dataMaxCount;
}

template<typename KEY, typename VALUE>
short  TSortedDictionary<KEY, VALUE>::SetCount(short count)
{
    if (count == 0)
    {
        m_dataCount = 0;
        m_dataMaxCount = 0;
        return m_dataCount;
    }

    if (count>m_dataMaxCount)
    {
        short newCapacity = m_dataMaxCount;
		if (newCapacity==0)
		{
			newCapacity = 8;
		}
        while (count>newCapacity)
        {
			if (newCapacity*sizeof(KEY)<1024)
			{
				newCapacity += newCapacity;
				if (newCapacity<8) newCapacity = 8;
			} else {
				newCapacity += 1024/sizeof(KEY);
			}
        }
        SetCapacity(newCapacity);
    }
    m_dataCount = count;
    return m_dataCount;
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::SetCapacity(short capacity)
{
    if (m_valueArray==NULL)
    {
		m_keyArray  = (KEY*)   malloc(capacity*sizeof(KEY));
        m_valueArray = (VALUE*) malloc(capacity*sizeof(VALUE));
        if (m_valueArray!=NULL)
        {
            m_dataMaxCount = capacity;
        } else {
            m_dataMaxCount = 0;
        }
    } else {
		m_keyArray  = (KEY*)   realloc(m_keyArray, capacity*sizeof(KEY));
        m_valueArray = (VALUE*) realloc(m_valueArray, capacity*sizeof(VALUE));
        if (m_valueArray!=NULL)
        {
            m_dataMaxCount = capacity;
        } else {
            m_dataMaxCount = 0;
        }
    }
}

template<typename KEY, typename VALUE>
bool TSortedDictionary<KEY, VALUE>::ContainsKey(KEY key)
{
    bool result=false;
    short i = FindKeyIndex(key);
    return i!=-1;
}

template<typename KEY, typename VALUE>
short TSortedDictionary<KEY, VALUE>::FindKeyIndexRec(KEY key, short min, short max)
{	
    if (min==max)
	{	
		if (m_keyArray[min]>=key) return min;
		return -1;
	}
	short mid = (min + max)/2;
    short result1 = FindKeyIndexRec(key, min, mid); 
	short result2 = -1;
	if (mid!=min)
	{
		 result2 = FindKeyIndexRec(key, mid, max);
	}
	
	if (result1!=-1) return result1;
	if (result2!=-1) return result2;
    return -1;
}

template<typename KEY, typename VALUE>
short TSortedDictionary<KEY, VALUE>::FindKeyIndex(KEY key)
{
    if (m_dataCount==0)
    {
        return -1;
    }
    short result = FindKeyIndexRec(key, 0, m_dataCount);
    if (result != -1)
    {
        if (m_keyArray[result]!=key)
        {
            result = -1;
        }
    }
    return result;
}

template<typename KEY, typename VALUE>
short TSortedDictionary<KEY, VALUE>::FindNearestKeyIndex(KEY key)
{
	if (m_dataCount==0)
	{
		return -1;
	}
	return FindKeyIndexRec(key, 0, m_dataCount);	
}



template<typename KEY, typename VALUE>
bool TSortedDictionary<KEY, VALUE>::InsertKey(short index, KEY key)
{
    short oldCount = m_dataCount;
    short newCount = SetCount(oldCount+1);
    if (newCount==oldCount+1)
    {                
		for(short i = oldCount; i>index; i--)
		{
			m_keyArray[i] = m_keyArray[i-1];
			m_valueArray[i] = m_valueArray[i-1];
		}
		m_keyArray[index] = key;
        return true;
    }
    return false;
};

template<typename KEY, typename VALUE>
VALUE&   TSortedDictionary<KEY, VALUE>::operator [] (KEY key)
{
    static VALUE invalidValue;

	//finds nearest index
    short i = FindNearestKeyIndex(key);
	bool found = false;
	if (i!=-1)
	{
		//verifies if found index is the required result
		found = m_keyArray[i]==key;
	} else {
		i = m_dataCount;
	}
    if (found)
	{
		//yes - item exists, can be returned as the result;
		return m_valueArray[i];
	}
    
	//no - new item needs to be inserted
	//found index is a position where new item must be inserted
    if (InsertKey(i, key))
	{
		//sets default value
		m_valueArray[i] = m_defaultValue;
		
		//returns reference to dictionary item
		return m_valueArray[i];    
	}

	//returns invalid value if memory allocation failed
	return invalidValue;
}

template<typename KEY, typename VALUE>
VALUE* TSortedDictionary<KEY,VALUE>::GetDefaultValue()
{
	return &m_defaultValue;	
}

template<typename KEY, typename VALUE>
KEY&  TSortedDictionary<KEY,VALUE>::Key(int index)
{
	if ((index>=0) || (index<m_dataCount))
	{
		return m_keyArray[index];
	}
	static KEY result;
	memset(&result, 0, sizeof(result));
	return result;
}


#endif
