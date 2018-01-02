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

#ifndef TSORTEDDICTIONARY_INL
#define TSORTEDDICTIONARY_INL

#include "TSortedDictionary.h"
#include <string.h>

template <typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::TSortedDictionary(short capacity)
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
TSortedDictionary<KEY, VALUE>::TSortedDictionary(TSortedDictionary<KEY, VALUE> &dictionary)
{
    m_dataCount = 0;
    m_dataMaxCount   = 0;
    m_pairArray  = NULL;
}

template<typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::~TSortedDictionary()
{
    Clear();
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::Clear()       
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
short  TSortedDictionary<KEY, VALUE>::Count()
{
    return m_dataCount;
}

template<typename KEY, typename VALUE>
bool  TSortedDictionary<KEY, VALUE>::Add(KEY key, VALUE value)
{
	if (m_dataCount==32767) return false;

	//finds nearest index
	short i = FindNearestKeyIndex(key);
	bool found = false;
	if (i!=-1)
	{
		//verifies if found index is the required result
		found = m_pairArray[i].first==key;
	} else {
		i = m_dataCount;
	}
	if (found)
	{
		//yes - item exists, can be returned as the result;
		return false;
	}

	//no - new item needs to be inserted
	//found index is a position where new item must be inserted
	if (InsertKey(i, key))
	{
		//sets default value
		m_pairArray[i].second = value;

		//returns reference to dictionary item
		return true;    
	}
	return false;
}

template<typename KEY, typename VALUE>
bool  TSortedDictionary<KEY, VALUE>::Add(TPair<KEY,VALUE> pair)
{
	return Add(pair.first, pair.second);
}

template<typename KEY, typename VALUE>
bool  TSortedDictionary<KEY, VALUE>::Remove(KEY key)
{

}

template<typename KEY, typename VALUE>
bool  TSortedDictionary<KEY, VALUE>::SetCount(short count)
{
	if (count == 0)
	{
		if (m_pairArray)
		{
			delete m_pairArray;
		}
		m_pairArray = NULL;
		m_dataCount = 0;
		m_dataMaxCount = 0;
		return true;
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
bool TSortedDictionary<KEY, VALUE>::SetCapacity(short capacity)
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
		if (m_pairArray[min].first>=key) return min;
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
        if (m_pairArray[result].first!=key)
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
    if (SetCount(m_dataCount+1))
    {                
		for(short i = m_dataCount-1; i>index; i--)
		{
			m_pairArray[i].first  = m_pairArray[i-1].first;
			m_pairArray[i].second = m_pairArray[i-1].second;
		}
		m_pairArray[index].first = key;
        return true;
    }
    return false;
};

template<typename KEY, typename VALUE>
VALUE&   TSortedDictionary<KEY, VALUE>::operator [] (KEY key)
{
	//finds nearest index
    short i = FindNearestKeyIndex(key);
	bool found = false;
	if (i!=-1)
	{
		//verifies if found index is the required result
		found = m_pairArray[i].first==key;
	} else {
		i = m_dataCount;
	}
    if (found)
	{
		//yes - item exists, can be returned as the result;
		return m_pairArray[i].second;
	}
    
	if (AddKeysAutomatically)
	{
		//no - new item needs to be inserted
		//found index is a position where new item must be inserted
		if (InsertKey(i, key))
		{
			//sets default value
			m_pairArray[i].second = m_defaultValue;
			
			//returns reference to dictionary item
			return m_pairArray[i].second;    
		}
	}

	//returns invalid value if memory allocation failed
	static VALUE invalidValue;
	memset(&invalidValue, 0, sizeof(invalidValue) );
	return invalidValue;
}

template<typename KEY, typename VALUE>
VALUE* TSortedDictionary<KEY,VALUE>::GetDefaultValue()
{
	return &m_defaultValue;	
}

template<typename KEY, typename VALUE>
TEnumerator<TPair<KEY, VALUE> > TSortedDictionary<KEY, VALUE>::GetEnumerator()
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
TPair<KEY, VALUE>* TSortedDictionary<KEY, VALUE>::begin()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[0];
}

template<typename KEY, typename VALUE>
TPair<KEY, VALUE>* TSortedDictionary<KEY, VALUE>::end()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[m_dataCount];
}

template<typename KEY, typename VALUE>
TPair<KEY, VALUE>* TSortedDictionary<KEY, VALUE>::data()
{
	if (m_dataCount==0) return NULL;
	return &m_pairArray[0];	
}

template<typename KEY, typename VALUE>
VALUE& TSortedDictionary<KEY, VALUE>::at(KEY key)
{
	return (*this)[key];
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::insert(KEY key, VALUE value)
{
	Add(key, value);
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::insert(TPair<KEY,VALUE> value)
{
	Add(value.first, value.second);
}

template<typename KEY, typename VALUE>
bool TSortedDictionary<KEY, VALUE>::empty()
{
	return m_dataCount==0;
}

template<typename KEY, typename VALUE>
int TSortedDictionary<KEY, VALUE>::size()
{
	return m_dataCount;
}

template<typename KEY, typename VALUE>
int TSortedDictionary<KEY, VALUE>::max_size()
{
	return 32767;
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::clear()
{
	Clear();
}
#endif


#endif
