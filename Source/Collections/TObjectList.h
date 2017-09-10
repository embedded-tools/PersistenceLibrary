/*
 * Persistence Library / Collections / TObjectList
 *
 * Copyright (c) 2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TOBJECTLIST___H
#define TOBJECTLIST___H

#include "TIterator.h"

/**
 *  TObjectList is a list similar to TList in C#. TObjectList is designed for
 *  storing existing class instance (it never calls constructors). Optionally
 *  can unalloc memory occupied by object (by calling method UnallocAndClear).
 *  
 *  It allocates memory dynamically, but it preallocates a bit more memory than 
 *  it is needed to decrease memory fragmentation.
 */

template <class T> class TObjectList 
#ifdef TITERATOR_INHERITANCE
: public TIterator
#endif
{

protected:
    T**		m_dataPointerArray;
    short	m_dataPointerCount;     //number of items (really inserted to list)
    short	m_dataPointerMaxCount;  //number of items preallocated in memory
	short	m_dataIterator;           //index of current item (used by iterator)
	bool	m_dataStatic;    //flag TRUE means that data was allocated 
	                           //dynamically therefore need to unalloc
    TObjectList(TObjectList<T> &list){};

public:

    TObjectList();
    ~TObjectList();

    T* Add();
    bool  Add(T* R);
    bool  Del(int id);
    bool  Del(T* R);
    T*    Insert (int i);
    void  Insert (int i, T* R);
	bool  Contains(T* R);
    short Capacity();
    short SetCount(short count);
    short SetCountAndCreate(short count);
    void  SetCapacity(short maxItemCount);
    void  Clear();
    void  UnallocAndClear();

#ifdef TITERATOR_INHERITANCE
    virtual void* First();
    virtual void* Next();
    virtual short Count();
#else 
    void* First();
    void* Next();
    short Count();
#endif			
    
    T* operator [] (short id);
	T* Items (short id);

    short IndexOf(T* R);

};

#include "tobjectlist.cpp"

#endif

