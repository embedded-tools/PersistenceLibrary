/*
 * Persistence Library / Collections / TStaticList
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

#ifndef TSTATICSTATICLIST_INL
#define TSTATICSTATICLIST_INL

#include "tstaticlist.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

template<class T, int N>
TStaticList<T, N>::TStaticList()
{
    DataCount  = 0;
    DataMax = N;
	memset((void*)&Data, 0, sizeof(Data));
}

template<class T, int N>
TStaticList<T, N>::~TStaticList()
{
    Clear();
}

template<class T, int N>
void TStaticList<T, N>::Add(T R)
{
	short oldDataCount = DataCount;
    SetCount(DataCount+1);
	if (oldDataCount!=DataCount)
	{
		Data[DataCount-1]  = R;
	}
};


template<class T, int N>
void TStaticList<T, N>::Del(short index)
{
    if ( (index<0) || (index>=DataCount) )
    {
        //invalid index -> nothing will be deleted
        return;
    }
    for (short j=index;j<DataCount-1;j++)
    {
        Data[j]=Data[j+1];
    }
    SetCount(DataCount-1);
};

template<class T, int N>
void TStaticList<T, N>::Insert (short index, T x)
{
    if (index<0) index = 0;
    if (index>DataCount) index = DataCount;

    SetCount(DataCount+1);
    if (index<DataCount-1)
    {
        memmove(&Data[index+1],&Data[index],(DataCount-1-index)*sizeof(T));
    }
    Data[index]=x ;
};

template<class T, int N>
bool TStaticList<T, N>::Contains (T x)
{
    bool result=false;
    for(short i = 0; i<DataCount; i++)
    {
        if (Data[i]==x)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<class T, int N>
short TStaticList<T, N>::IndexOf (T x)
{
    bool result=-1;
    for(short i = 0; i<DataCount; i++)
    {
        if (Data[i]==x)
        {
            result = i;
            break;
        }
    }
    return result;
}


template<class T, int N>
short TStaticList<T, N>::Count()
{
    return DataCount;
};

template<class T, int N>
short TStaticList<T, N>::MaxCount()
{
    return N;
};


template<class T, int N>
short TStaticList<T, N>::Capacity()
{
    return DataMax;
};


template<class T, int N>
short TStaticList<T, N>::SetCount(short count)
{
    if (count<0) count = 0;

	if (count>=DataMax)
	{
		 count = DataMax;
	}
    DataCount = count;
    return DataCount;
};

template<class T, int N>
void TStaticList<T, N>::Clear()
{
    SetCount(0);    
};

template<class T, int N>
void* TStaticList<T, N>::First()
{
    DataIterator = 0;
    if (DataCount>0)
    {
        return Data[0];
    }
    return NULL;    
};

template<class T, int N>
void* TStaticList<T, N>::Next()
{
    DataIterator++;
    if (DataIterator<DataCount)
    {
        return Data[DataIterator];
    }
    return NULL;
};

template<class T, int N>
T& TStaticList<T, N>::operator [] (short index)
{	
    if ((index>=0) && (index<DataCount))
    {
		return Data[index];
    };

	static T buf;
	memset((void*)&buf, 0, sizeof(T));
    return buf;
}

#endif
