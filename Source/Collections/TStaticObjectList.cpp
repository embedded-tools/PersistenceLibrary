/*
 * Persistence Library / Collections / TStaticObjectList
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

#ifndef TSTATICOBJECTLIST_INL
#define TSTATICOBJECTLIST_INL

#include "tstaticobjectlist.h"
#include <stddef.h>
#include <stdlib.h>


template<class T, int N>
TStaticObjectList<T, N>::TStaticObjectList()
{
    DataCount  = 0;
    DataMax    = 0;
    memset(&Data, sizeof(T)*N, 0);
}

template<class T, int N>
TStaticObjectList<T, N>::~TStaticObjectList()
{
    Clear();
}

template<class T, int N> T* TStaticObjectList<T, N>::Add()
{
    SetCount(DataCount+1);
    T* result = new T();
    Data[DataCount-1]=result;
    return result;
};

template<class T, int N>
bool TStaticObjectList<T, N>::Add(T* R)
{    
	int oldCount = DataCount;
    int newCount = SetCount(DataCount+1);
    if (newCount>0)
    {
        Data[newCount-1]=R;
    }
	return oldCount!=newCount;
};


template<class T, int N>
bool TStaticObjectList<T, N>::Del(int id)
{
    if (id<0) return false;
    if (id>=DataCount) return false;
    
    for (int j=id;j<DataCount-1;j++)
    {
        Data[j]=Data[j+1];
    }
    SetCount(DataCount-1);
	return true;
};

template<class T, int N>
bool TStaticObjectList<T, N>::Del(T* R)
{
    for (int i=0;i<DataCount;i++)
    {
        if (Data[i]==R)
        {
            Del(i);
            return true;
        }
    }
	return false;
};

template<class T, int N>
T* TStaticObjectList<T, N>::Insert (int i)
{
    SetCount(DataCount+1);

    if (i<0) i=0;
    if (i<DataCount)
    {
        memmove(&Data[i+1],&Data[i],(DataCount-1-i)*4);
    } else {
       i = DataCount-1;
    }
    T* result = new T();
    Data[i]=result;
    return result;
};

template<class T, int N>
void TStaticObjectList<T, N>::Insert (int i, T* R)
{
    SetCount(DataCount+1);

    if (i<0) i=0;
    if (i<DataCount)
    {
        memmove(&Data[i+1],&Data[i],(DataCount-1-i)*4);
    }
    Data[i]=R;
};

template<class T, int N>
short TStaticObjectList<T, N>::Count()
{
    return DataCount;
};

template<class T, int N>
short TStaticObjectList<T, N>::Capacity()
{
    return DataMax;
}

template<class T, int N>
short TStaticObjectList<T, N>::SetCount(short count)
{
    if (count<0) count = 0;   
    DataCount = count;
    return DataCount;
};

template<class T, int N>
short TStaticObjectList<T, N>::SetCountAndCreate(short count)
{
    SetCount(count);
    for (short i=0;i<DataCount;i++) 
    {
        Data[i]=new T();
    }
    return DataCount;
};

template<class T, int N>
void TStaticObjectList<T, N>::Clear()
{
    SetCount(0);    
	memset(Data, DataMax*sizeof(T*), 0);
};

template<class T, int N>
void TStaticObjectList<T, N>::UnallocAndClear()
{
    for(short i=0; i<DataCount; i++)
    {
        if (Data[i]!=NULL)
        {
            delete Data[i];
            Data[i] = NULL;
        }
    }
    SetCount(0);    
	memset(Data, DataMax*sizeof(T*), 0);
};

template<class T, int N>
T* TStaticObjectList<T, N>::First()
{
    return Data[0];
};

template<class T, int N>
T* TStaticObjectList<T, N>::Last()
{
    return Data[DataCount-1];
};

template<class T, int N>
T* TStaticObjectList<T, N>::operator [] (short id)
{
    if (id<0)
    {
        return NULL;
    }
    if (id>=DataCount)
    {
        return NULL;
    }
    return Data[id];
}

template<class T, int N>
T* TStaticObjectList<T, N>::Items (short id)
{
    if (id<0)
    {
        return NULL;
    }
    if (id>=DataCount)
    {
        return NULL;
    }
    return Data[id];
}



template<class T, int N>
short TStaticObjectList<T, N>::IndexOf(T* R)
{
    for (int i=0; i<DataCount; i++)
    {
        if (R == Data[i])
        {
            return i;
        }
    }
    return -1;
}

#endif
