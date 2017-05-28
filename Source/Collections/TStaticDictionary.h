/*
 * Persistence Library / Collections / TStaticDictionary
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

#ifndef TSTATICDICTIONARY___H
#define TSTATICDICTIONARY___H

#include <stdlib.h> 

/**
 *  TStaticDictionary stores key-value pairs. It allocates memory statically, 
 *  it preallocates defined amount of items. It is possible to add/remove items,
 *  but maximum number of items is fixed and there is no way how to increase it.
 *
 *  Static dictionary avoids memory fragmentation on embedded system with low RAM size.
 */
template <typename K, typename V, int N> class TStaticDictionary 
{
protected:

	K      _Key[N];
	V      _Value[N];
    short  DataCount;
    short  DataIterator; 
    short  DataMax;   

    int  Capacity();

public:

	TStaticDictionary();
	TStaticDictionary(TStaticDictionary<K, V, N> &dictionary);
	~TStaticDictionary();

    void Add(K key, V value);
    void Del(K key);	
    short Count();

    bool ContainsKey (K key);
	bool ContainsValue (V value);    

	K&   Key(int index);
    void Clear();

    V& operator [] (K key);


};

#include "tstaticdictionary.cpp"

#endif

