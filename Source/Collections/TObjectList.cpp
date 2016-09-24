/*
 * Persistence Library / Collections / TObjectList
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

#ifndef TOBJECTLIST_INL
#define TOBJECTLIST_INL

#include "tobjectlist.h"
#include <stddef.h>
#include <stdlib.h>


template<class T>
TObjectList<T>::TObjectList()
{
    DataCount  = 0;
    DataMax    = 0;
	DataStatic = false;
    PData      = NULL;
}

template<class T>
TObjectList<T>::~TObjectList()
{
    Clear();
	if (!DataStatic)
	{
		if (PData!=NULL) free(PData);
		PData=NULL;
	}    
}

template<class T> T* TObjectList<T>::Add()
{
    SetCount(DataCount+1);
    T* result = new T();
    PData[DataCount-1]=result;
    return result;
};

template<class T>
bool TObjectList<T>::Add(T* R)
{    
	int oldCount = DataCount;
    int newCount = SetCount(DataCount+1);
    if (newCount>0)
    {
        PData[newCount-1]=R;
    }
	return oldCount!=newCount;
};


template<class T>
bool TObjectList<T>::Del(int id)
{
    if (id<0) return false;
    if (id>=DataCount) return false;
    
    for (int j=id;j<DataCount-1;j++)
    {
        PData[j]=PData[j+1];
    }
    SetCount(DataCount-1);
	return true;
};

template<class T>
bool TObjectList<T>::Del(T* R)
{
    for (int i=0;i<DataCount;i++)
    {
        if (PData[i]==R)
        {
            Del(i);
            return true;
        }
    }
	return false;
};

template<class T>
T* TObjectList<T>::Insert (int i)
{
    SetCount(DataCount+1);

    if (i<0) i=0;
    if (i<DataCount)
    {
        memmove(&PData[i+1],&PData[i],(DataCount-1-i)*4);
    } else {
       i = DataCount-1;
    }
    T* result = new T();
    PData[i]=result;
    return result;
};

template<class T>
void TObjectList<T>::Insert (int i, T* R)
{
    SetCount(DataCount+1);

    if (i<0) i=0;
    if (i<DataCount)
    {
        memmove(&PData[i+1],&PData[i],(DataCount-1-i)*4);
    }
    PData[i]=R;
};

template<class T>
bool  TObjectList<T>::Contains(T* R)
{
	int i;
	bool result = false;
    for (i=0; i<DataCount-1;i++)
    {
        if (PData[i]==R)
		{
			result = true;
		}
    }
	return result;
}

template<class T>
short TObjectList<T>::Count()
{
    return DataCount;
};

template<class T>
short TObjectList<T>::Capacity()
{
    return DataMax;
}

template<class T>
short TObjectList<T>::SetCount(short count)
{
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
				PData = (T**) malloc(DataMax*sizeof(T*));
				if (PData==NULL)
				{
					DataCount=0;
					DataMax=0;
					return 0;                
				}
				memset(PData, 0, DataMax*sizeof(T*));
			} else {
				PData = (T**) realloc(PData,DataMax*sizeof(T*));
				if (PData==NULL)
				{
					DataCount=0;
					DataMax=0;
					return 0;
				}
				for(int i = DataCount; i<DataMax; i++)
				{
					PData[i] = NULL;
				}
			}
		}
	}
    DataCount = count;
    return DataCount;
};

template<class T>
short TObjectList<T>::SetCountAndCreate(short count)
{
    SetCount(count);
    for (short i=0;i<DataCount;i++) 
    {
        PData[i]=new T();
    }
    return DataCount;
};

template<class T>
void TObjectList<T>::SetCapacity(short reservedCapacity)
{
    DataCount = 0;
    DataMax   = reservedCapacity;
    if (PData==NULL)
    {
        PData = (T**) malloc(reservedCapacity*sizeof(T*));
        memset(PData, 0, reservedCapacity*sizeof(T*));
    } else {
        PData = (T**) realloc(PData, DataMax*sizeof(T*));
        for(int i = DataCount; i<DataMax; i++)
        {
            PData[i] = NULL;
        }
    }
    DataCount = 0;
}


template<class T>
void TObjectList<T>::Clear()
{
    SetCount(0);    
	memset(PData, DataMax*sizeof(T*), 0);
};

template<class T>
void TObjectList<T>::UnallocAndClear()
{
    for(short i=0; i<DataCount; i++)
    {
        if (PData[i]!=NULL)
        {
            delete PData[i];
            PData[i] = NULL;
        }
    }
    SetCount(0);    
	memset(PData, DataMax*sizeof(T*), 0);
};

template<class T>
void* TObjectList<T>::First()
{
	DataIterator = 0;
	if (DataCount==0) return NULL;
    return PData[DataIterator++];
};

template<class T>
void* TObjectList<T>::Next()
{
	if (DataIterator>DataCount) return NULL;
    return PData[DataIterator++];
};

template<class T>
T* TObjectList<T>::operator [] (short id)
{
    if (id<0)
    {
        return NULL;
    }
    if (id>=DataCount)
    {
        return NULL;
    }
    return PData[id];
}

template<class T>
T* TObjectList<T>::Items (short id)
{
    if (id<0)
    {
        return NULL;
    }
    if (id>=DataCount)
    {
        return NULL;
    }
    return PData[id];
}



template<class T>
short TObjectList<T>::IndexOf(T* R)
{
    for (int i=0; i<DataCount; i++)
    {
        if (R == PData[i])
        {
            return i;
        }
    }
    return -1;
}

#endif
