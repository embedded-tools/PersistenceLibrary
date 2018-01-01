/*
 * Persistence Library / Collections / TStaticDictionary
 *
 * Copyright (c) 2016 Ondrej Sterba <osterba at atlas.cz>
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

#ifndef TSTATICDICTIONARY___H
#define TSTATICDICTIONARY___H

#include <stdlib.h> 
#include "TEnumerator.h"
#include "TPair.h"

/**
 *  TStaticDictionary stores key-value pairs. It allocates memory statically, 
 *  it preallocates defined amount of items. It is possible to add/remove items,
 *  but maximum number of items is fixed and there is no way how to increase it.
 *
 *  Static dictionary avoids memory fragmentation on embedded system with low RAM size.
 */
template <typename KEY, typename VALUE, int N> class TStaticDictionary 
{
protected:

	TPair<KEY,VALUE> m_pairArray[N];
	VALUE            m_defaultValue;

    short  m_dataCount;
    short  m_dataIterator; 
    short  m_dataMaxCount;   

    int  Capacity();

    TStaticDictionary(TStaticDictionary<KEY, VALUE, N> &dictionary);

public:

	TStaticDictionary();	
	~TStaticDictionary();

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

#include "tstaticdictionary.cpp"

#endif

