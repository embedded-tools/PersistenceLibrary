/*
 * Persistence Library / Collections / TObjectList
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

#ifndef TOBJECTLIST___H
#define TOBJECTLIST___H

#include "TEnumerator.h"
#include "TArray.h"

/**
 *  TObjectList is a list similar to TList in C#. TObjectList is designed for
 *  storing existing class instance (it never calls constructors). Optionally
 *  can unalloc memory occupied by object (by calling method UnallocAndClear).
 *  
 *  It allocates memory dynamically, but it preallocates a bit more memory than 
 *  it is needed to decrease memory fragmentation.
 */

template <class T> class TObjectList 
#ifndef COLLECTIONS_SIMPLIFY
: public TEnumerator<T*>
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
    bool    SetCount_NoCreate(short count);

public:

    TObjectList();
    ~TObjectList();

#ifndef COLLECTIONS_SIMPLIFY
	virtual
#endif
	TEnumerator<T*> GetEnumerator();
	
    T*    Add();
    bool  Add(T* R);
    bool  RemoveAt(int id);
    bool  Remove(T* R);
	short Count();
    T*    Insert (int i);
    void  Insert (int i, T* R);
	bool  Contains(T* R);
    short Capacity();
    bool  SetCount(short count);
    bool  SetCapacity(short maxItemCount);
    void  Clear();
    
    T* operator [] (short id);
	T* Items (short id);

	TArray<T*> ToArray();

    short IndexOf(T* R);

#ifdef STL_STYLE
	typedef T** iterator;
	T**         begin();
	T**         end();
	T**         data();
	T*          at(int i);
	void        push_back(T* value);
	void        push_front(T* value);
	void        pop_front();
	void        pop_back();
	T*          front();
	T*          back();
	bool        empty();
	int         size();
	int         max_size();
	void        clear();
#endif

};

#include "tobjectlist.cpp"

#endif

