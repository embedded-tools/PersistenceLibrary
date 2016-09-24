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

template <typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::TSortedDictionary(short capacity)
{
    ItemCount = 0;
    ItemMax   = 0;
    Item  = NULL;
    Key   = NULL;

    if (capacity>0)
    {
        SetCapacity(capacity);
    }    
}

template<typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::TSortedDictionary(TSortedDictionary<KEY, VALUE> &dictionary)
{
    ItemCount = 0;
    ItemMax   = 0;
    Item  = NULL;
    Key   = NULL;
}

template<typename KEY, typename VALUE>
TSortedDictionary<KEY, VALUE>::~TSortedDictionary()
{
    Clear();
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::Clear()       
{
    if (Item!=NULL)
    {
        free(Item);
    }
    Item=NULL;
    if (Key!=NULL)
    {
        free(Key);
    }
    Key=NULL;
    ItemCount = 0;
    ItemMax   = 0;
}

template<typename KEY, typename VALUE>
short  TSortedDictionary<KEY, VALUE>::Count()
{
    return ItemCount;
}

template<typename KEY, typename VALUE>
short  TSortedDictionary<KEY, VALUE>::Capacity()
{
    return ItemMax;
}

template<typename KEY, typename VALUE>
short  TSortedDictionary<KEY, VALUE>::SetCount(short count)
{
    if (count == 0)
    {
        ItemCount = 0;
        ItemMax = 0;
        return ItemCount;
    }

    if (count>ItemMax)
    {
        short newCapacity = ItemMax;
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
    ItemCount = count;
    return ItemCount;
}

template<typename KEY, typename VALUE>
void TSortedDictionary<KEY, VALUE>::SetCapacity(short capacity)
{
    if (Item==NULL)
    {
		Key  = (KEY*)   malloc(capacity*sizeof(KEY));
        Item = (VALUE*) malloc(capacity*sizeof(VALUE));
        if (Item!=NULL)
        {
            ItemMax = capacity;
        } else {
            ItemMax = 0;
        }
    } else {
		Key  = (KEY*)   realloc(Key, capacity*sizeof(KEY));
        Item = (VALUE*) realloc(Item, capacity*sizeof(VALUE));
        if (Item!=NULL)
        {
            ItemMax = capacity;
        } else {
            ItemMax = 0;
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
		if (Key[min]>=key) return min;
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
    if (ItemCount==0)
    {
        return -1;
    }
    short result = FindKeyIndexRec(key, 0, ItemCount);
    if (result != -1)
    {
        if (Key[result]!=key)
        {
            result = -1;
        }
    }
    return result;
}

template<typename KEY, typename VALUE>
short TSortedDictionary<KEY, VALUE>::FindNearestKeyIndex(KEY key)
{
	if (ItemCount==0)
	{
		return -1;
	}
	return FindKeyIndexRec(key, 0, ItemCount);	
}



template<typename KEY, typename VALUE>
bool TSortedDictionary<KEY, VALUE>::InsertKey(short index, KEY key)
{
    short oldCount = ItemCount;
    short newCount = SetCount(oldCount+1);
    if (newCount==oldCount+1)
    {                
		for(short i = oldCount; i>index; i--)
		{
			Key[i] = Key[i-1];
			Item[i] = Item[i-1];
		}
		Key[index] = key;
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
		found = Key[i]==key;
	} else {
		i = ItemCount;
	}
    if (found)
	{
		//yes
		return Item[i];
	}
    
	//found index is a position where new item must be inserted
    if (InsertKey(i, key))
	{
		//returns reference to dictionary item
		return Item[i];    
	}

	//returns invalid value if memory allocation failed
	return invalidValue;
}

template<typename KEY, typename VALUE>
void* TSortedDictionary<KEY, VALUE>::First()
{
	DataIterator=0;
	if (ItemCount==0) return NULL;
	return (void*)&Key[DataIterator++];
}

template<typename KEY, typename VALUE>
void* TSortedDictionary<KEY, VALUE>::Next()
{
	if (DataIterator>ItemCount) return NULL;
	return (void*)&Key[DataIterator++];
}

template<typename KEY, typename VALUE>
KEY&  TSortedDictionary<KEY,VALUE>::Keys(int index)
{
	if ((index>=0) || (index<DataCount))
	{
		return _Key[index];
	}
	static K result;
	memset(&result, 0, sizeof(result));
	return result;
}


#endif
