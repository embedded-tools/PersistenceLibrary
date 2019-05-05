/*
 * Persistence Library / Collections / TList
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

#ifndef TLIST___H
#define TLIST___H

#include "TEnumerator.h"
#include "TEnumerable.h"
#include "TArray.h"

/**
 *  TList is a list similar to List in C#. TList is designed for
 *  storing simple value data types like char, int etc.
 *  
 *  It allocates memory dynamically, but it preallocates a bit
 *  more memory than it is needed to decrease memory fragmentation
 *  on embedded systems with low RAM size (e.g. 32kB or less)
 */
template <typename T> class TList 
#ifndef COLLECTIONS_SIMPLIFY
: public TEnumerator<T>
#endif
{
protected:

	T*		m_dataArray;		   
	short	m_dataCount;     //number of items (really inserted to list)
	short	m_dataMaxCount;  //number of items preallocated in memory

public:

	TList(int capacity=8);
	TList(const TList& list);
	~TList();

#ifndef COLLECTIONS_SIMPLIFY
	virtual
#endif
	TEnumerator<T> GetEnumerator();
	
	short Count() const;
	bool  Add(T R);
	bool  Remove(T R);
	void  RemoveAt(short index);
	void  Insert (short index, T x);
	bool  Contains (T x) const;
	short IndexOf(T x, short startIndex=0) const;
	short LastIndexOf(T x) const;
	short Capacity() const;
	bool  SetCount(short count);
	bool  SetCapacity(short capacity);
	void  Clear(bool unallocMemory=true);
	void  Reverse();
	void  Sort(bool ascending=true, bool deleteDoubleEntries=false);
	TArray<T> ToArray();

	T&  operator [] (short index);
	T&  Items (short index);
	TList<T>& operator = (const TList<T>& list);

#ifdef STL_STYLE
	typedef T* iterator;
	typedef const T* const_iterator;

	T*         begin();
	T*         end();
	const T*   cbegin() const;
	const T*   cend() const;
	T*         data();
	T&         at(int i);
	void       push_back(T value);
	void       push_front(T value);
	void       pop_front();
	void       pop_back();
	void       reserve(int i);
	T          front();
	T          back();
	T*         insert(T* it, T value);
	void       splice(T* position, TList<T>& list, T* first=NULL, T* last=NULL);
	T*         erase (T* first, T* last=NULL);
	void       unique();
	bool       empty();
	int        size();
	int        max_size();
	void       clear();
	void       reverse();
	void       sort();	
#endif

};

#include "TList.cpp"
 
#endif

