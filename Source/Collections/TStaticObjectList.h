/*
 * Persistence Library / Collections / TStaticObjectList
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

#ifndef TSTATICOBJECTLIST___H
#define TSTATICOBJECTLIST___H

/**
 *  TStaticList is a list similar to TList in C#. TStaticObjectList is designed for
 *  storing existing class instance (it never calls constructors). Optionally
 *  can unalloc memory occupied by object (by calling method UnallocAndClear).
 *  
 *  It allocates memory statically, there is no way no to increase
 *  maximum number of items.
 *
 *  Static dictionary avoids memory fragmentation on embedded system with low RAM size.
 */

template <class T, int N> class TStaticObjectList
#ifdef TITERATOR_INHERITANCE
: public TIterator
#endif
{
protected:
    T*    m_dataPointerArray[N];
    short m_dataPointerCount;
    short m_dataIterator; 
    short m_dataPointerMaxCount;

public:

    TStaticObjectList();
    ~TStaticObjectList();

    T* Add();
    bool  Add(T* R);
    bool  Del(int id);
    bool  Del(T* R);
    T*    Insert (int i);
    void  Insert (int i, T* R);
    short Capacity();
    short SetCount(short count);
    short SetCountAndCreate(short count);
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
    short MaxCount();

    T* operator [] (short id);
	T* Item (short id);

    short IndexOf(T* R);

};

#include "tstaticobjectlist.cpp"

#endif

