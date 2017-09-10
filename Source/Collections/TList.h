/*
 * Persistence Library / Collections / TList
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

#ifndef TLIST___H
#define TLIST___H

#include "TIterator.h"

/**
 *  TList is a list similar to TList in C#. TList is designed for
 *  storing simple value data types like char, int etc.
 *  
 *  It allocates memory dynamically, but it preallocates a bit
 *  more memory than it is needed to decrease memory fragmentation
 *  on embedded systems with low RAM size (e.g. 32kB or less)
 */
template <typename T> class TList
#ifdef TITERATOR_INHERITANCE
: public TIterator
#endif
{
protected:

    T*		m_dataArray;		   
    short	m_dataCount;     //number of items (really inserted to list)
    short	m_dataMaxCount;       //number of items preallocated in memory
	short	m_dataIterator;  //index of current item (used by iterator)
	bool	m_dataStatic;    //flag TRUE means that data was allocated statically
                             //therefore no unallocation is needed
							 
    TList(TList<T> &list){};

public:

    TList();
    ~TList();

#ifdef TITERATOR_INHERITANCE
    virtual void* First();
    virtual void* Next();
    virtual short Count();
#else 
    void* First();
    void* Next();
    short Count();
#endif			

    void  Add(T R);
    void  Del(short index);
    void  Insert (short index, T x);
    bool  Contains (T x);
    short IndexOf(T x);    
    short Capacity();
    short SetCount(short count);
    void  SetCapacity(short capacity);
    void  Clear();

    T&  operator [] (short index);

};

#include "tlist.cpp"
 
#endif

