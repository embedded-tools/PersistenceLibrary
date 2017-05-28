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
    DataCount  = 0;
    DataMax = N;
    memset((void*)&_Key,   0, sizeof(_Key));
    memset((void*)&_Value, 0, sizeof(_Value));
}

template<typename K, typename V, int N>
TStaticDictionary<K,V,N>::~TStaticDictionary()
{
    Clear();
}

template<typename K, typename V, int N>
void TStaticDictionary<K,V,N>::Add(K key, V value)
{
	int oldDataCount = DataCount;
    if (DataCount<DataMax)
    {
		_Key[DataCount]   = key;
		_Value[DataCount] = value;
        DataCount++;
	}
};


template<typename K, typename V, int N>
void TStaticDictionary<K,V,N>::Del(K key)
{
	int index = -1;
	for(int i = 0; i<DataCount; i++)
	{
		if (_Key[i] == key) 
		{
			index = i;
		}
	}
	if (index==-1) return;		
    for (int j=index;j<DataCount-1;j++)
    {
        _Key[j]=_Key[j+1];
		_Value[j]=_Value[j+1];
    }
    DataCount--;
};

template<typename K, typename V, int N>
bool TStaticDictionary<K,V,N>::ContainsKey (K key)
{
    bool result=false;
    for(int i = 0; i<DataCount; i++)
    {
        if (_Key[i]==key)
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
    for(int i = 0; i<DataCount; i++)
    {
        if (_Value[i]==value)
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
    return DataCount;
};

template<typename K, typename V, int N>
K&  TStaticDictionary<K,V,N>::Key(int index)
{
	if ((index>=0) || (index<DataCount))
	{
		return _Key[index];
	}
	static K result;
	return result;
}


template<typename K, typename V, int N>
int TStaticDictionary<K,V,N>::Capacity()
{
    return DataMax;
};

template<typename K, typename V, int N>
void TStaticDictionary<K,V,N>::Clear()
{
    DataCount = 0;   
};

template<typename K, typename V, int N>
V& TStaticDictionary<K,V,N>::operator [] (K key)
{
	for(int i2 = 0; i2<DataCount; i2++)
	{
		if (_Key[i2] == key)
		{
			return _Value[i2];
		}
	}
    if (DataCount<DataMax)
    {
        _Key[DataCount] = key;
        return _Value[DataCount++];
    }        

	static V value;
    memset(&value, 0, sizeof(value));
    return value;
}

#endif
