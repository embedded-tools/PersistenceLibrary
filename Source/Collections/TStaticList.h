/*
 * Persistence Library / Collections / TStaticList
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

#ifndef TSTATICLIST___H
#define TSTATICLIST___H


/**
 *  TStaticList is a list similar to TList in C#. TStaticList is designed for
 *  storing simple value data types like char, int etc.
 *  
 *  It allocates memory statically, there is no way no to increase
 *  maximum number of items.
 *
 *  Static dictionary avoids memory fragmentation on embedded system with low RAM size.
 */
template <typename T, int N> class TStaticList
#ifdef TITERATOR_INHERITANCE
: public TIterator
#endif
{
protected:

    T      m_dataArray[N];
    short  m_dataCount;
    short  m_dataIterator; 
    short  m_dataMaxCount;

public:

    TStaticList();
    ~TStaticList();

    void  Add(T R);
    void  Del(short index);
    void  Insert (short index, T x);
    bool  Contains (T x);
    short IndexOf(T x);
    short Capacity();
    short SetCount(short count);
    void  Clear();

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

    T&  operator [] (short index);

};

#include "tstaticlist.cpp"
 
#endif

