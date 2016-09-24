/*
 * Persistence Library / Collections / TList
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

#ifndef TLIST_INL
#define TLIST_INL

#include "tlist.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

template<class T>
TList<T>::TList()
{
	DataStatic = false;
    DataCount  = 0;
    DataMax    = 0;
	DataStatic = false;
    PData       = NULL;	
}

template<class T>
TList<T>::~TList()
{
    Clear();
}

template<class T>
void* TList<T>::First()
{
	DataIterator = 0;
	if (DataIterator<DataCount)
	{
		return &PData[DataIterator];
	}
	return NULL;
}

template<class T>
void* TList<T>::Next()
{
	DataIterator++;
	if (DataIterator<DataCount)
	{
		return &PData[DataIterator];
	}
	return NULL;
}

template<class T>
short TList<T>::Count()
{
	return DataCount;
}

template<class T>
void TList<T>::Add(T R)
{
	short oldDataCount = DataCount;
    SetCount(DataCount+1);
	if (oldDataCount!=DataCount)
	{
		PData[DataCount-1]  = R;
	}
};


template<class T>
void TList<T>::Del(short index)
{
    if ( (index<0) || (index>=DataCount) )
    {
        //invalid index -> nothing will be deleted
        return;
    }
    for (short j=index;j<DataCount-1;j++)
    {
        PData[j]=PData[j+1];
    }
    SetCount(DataCount-1);
};

template<class T>
void TList<T>::Insert (short index, T x)
{
    if (index<0) index = 0;
    if (index>DataCount) index = DataCount;

    SetCount(DataCount+1);
    if (index<DataCount-1)
    {
        memmove(&PData[index+1],&PData[index],(DataCount-1-index)*sizeof(T));
    }
    PData[index]=x ;
};

template<class T>
bool TList<T>::Contains (T x)
{
    bool result=false;
    for(short i = 0; i<DataCount; i++)
    {
        if (PData[i]==x)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<class T>
short TList<T>::IndexOf (T x)
{
    bool result=-1;
    for(short i = 0; i<DataCount; i++)
    {
        if (PData[i]==x)
        {
            result = i;
            break;
        }
    }
    return result;
}

template<class T>
short TList<T>::Capacity()
{
    return DataMax;
};


template<class T>
short TList<T>::SetCount(short count)
{
    if (count<0) count = 0;

	if (!DataStatic)
	{
		if (count == 0)
		{
			if (PData!=NULL)
			{
				free(PData);
			}
			PData = NULL;
			DataCount = 0;
			DataMax = 0;
			return DataCount;
		}

		if (count>DataMax)
		{
			while (DataMax<count)
			{
				if (DataMax*sizeof(T)<1024)
				{
					DataMax += DataMax;
					if (DataMax<8) DataMax = 8;
				} else {
					DataMax += 1024/sizeof(T);
				}
			}
			if (PData==NULL)
			{
				PData = (T*) malloc(DataMax*sizeof(T));
				memset((void*)PData, 0, DataMax*sizeof(T));
			} else {

				PData = (T*) realloc(PData,DataMax*sizeof(T));
				memset((void*)(PData+count), 0, (DataMax-count)*sizeof(T));
			}
		}
	} else {
		if (count>=DataMax)
		{
			 count = DataMax;
		}
	}
    DataCount = count;
    return DataCount;
};

template<class T>
void TList<T>::SetCapacity(short reservedCapacity)
{
	if (!DataStatic)
	{
		DataCount = 0;
		DataMax   = 0;
		if (PData!=NULL)
		{
			free(PData);
			PData = NULL;
		}
		if (reservedCapacity>0)
		{
			PData = (T*) malloc(reservedCapacity*sizeof(T));
			memset((void*) PData, 0, reservedCapacity*sizeof(T));
			DataMax = reservedCapacity;
		}
	}
}


template<class T>
void TList<T>::Clear()
{
    SetCount(0);    
};

template<class T>
T& TList<T>::operator [] (short index)
{	
    if ((index>=0) && (index<DataCount))
    {
		return PData[index];
    };

	static T buf;
	memset((void*)&buf, 0, sizeof(T));
    return buf;
}

#endif
