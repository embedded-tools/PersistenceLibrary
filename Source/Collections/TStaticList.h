/*
 * Persistence Library / Collections / TStaticList
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

#ifndef TSTATICLIST___H
#define TSTATICLIST___H

#include "TEnumerable.h"
#include "TArray.h"

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
#ifndef COLLECTIONS_SIMPLIFY
: public TEnumerator<T>
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

#ifndef COLLECTIONS_SIMPLIFY
	virtual
#endif
	TEnumerator<T> GetEnumerator();

	short Count();
	void  Add(T R);
	void  RemoveAt(short index);	
	void  Insert (short index, T x);
	bool  Contains (T x) const;
	short IndexOf(T x, short startIndex=0);
	short LastIndexOf(T x);
	short Capacity() const;
	bool  SetCount(short count);
	bool  SetCapacity(short capacity);
	void  Clear();
	void  Reverse();
	void  Sort(bool ascending=true, bool deleteDoubleEntries=false);
	TArray<T> ToArray();

    T&  operator [] (short index);
	T&  Items (short index);

#ifdef STL_STYLE
	typedef T* iterator;
	T*         begin();
	T*         end();
	T*         data();
	T&         at(int i);
	void       push_back(T value);
	void       push_front(T value);
	void       pop_front();
	void       pop_back();
	T          front();
	T          back();
	bool       empty();
	int        size();
	int        max_size();
	void       clear();
	void       reverse();
	void       sort();	
#endif

};

#include "tstaticlist.cpp"
 
#endif

