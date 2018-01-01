/*
 * Persistence Library / Collections / TSortedDictionary
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

#ifndef TDICTIONARY___H
#define TDICTIONARY___H

#include <stdlib.h>
#include "TEnumerator.h"
#include "TEnumerable.h"
#include "TPair.h"

/**
 *  TSortedDictionary stores key-value pairs. Internally it sorts all inserted data,
 *  therefore Find methods can use binary search algorithm that is much faster than
 *  ordinary TDictonary object.
 *
 *  It allocates memory dynamically, but it preallocates a bit more memory than 
 *  it is needed to decrease memory fragmentation.
 */



template <typename KEY, typename VALUE> class TDictionary 
{
public:

private:
    TPair<KEY,VALUE>*  m_pairArray;
	VALUE  m_defaultValue;
	
    short  m_dataCount;
    short  m_dataMaxCount;
	short  m_dataIterator;

    bool  SetCount(short count);
    bool  SetCapacity(short capacity);
	
	short FindKeyIndex(KEY key);
    short FindKeyIndexRec(KEY key, short min, short max);   
	short FindNearestKeyIndex(KEY key); 
	bool  InsertKey(short index, KEY key);

public:

    TDictionary(short capacity=8);    
	TDictionary(const TDictionary<KEY,VALUE> &dictionary);
    ~TDictionary();	

	TEnumerator<TPair<KEY, VALUE>> GetEnumerator();

	bool  Add(KEY key, VALUE value);
	bool  Add(TPair<KEY,VALUE> pair);
	bool  Remove(KEY key);	
	short Count();
	bool  ContainsKey (KEY key);
	void  Clear();
	TPair<KEY,VALUE>* Data();

	bool   AddKeysAutomatically;
    VALUE& operator [] (KEY key);
	VALUE* GetDefaultValue();	

#ifdef STL_STYLE
	typedef TPair<KEY,VALUE>* iterator;
	TPair<KEY, VALUE>*	begin();
	TPair<KEY, VALUE>*	end();
	TPair<KEY, VALUE>*  data();
	VALUE&  at(KEY key);
	void    insert(KEY key, VALUE value);
	void    insert(TPair<KEY,VALUE> value);
	bool    empty();
	int     size();
	int     max_size();
	void    clear();
#endif

};

#include "TDictionary.cpp"

#endif

