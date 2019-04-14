/*
 * Persistence Library / Collections
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

#ifndef TARRAY___H
#define TARRAY___H

/**
 *  TArray is a read only array that is usually returned as a result
 *  of ToArray() function. ToArray() function is implemented by almost
 *  all collection classes. 
 *
 */
template <typename T> class TArray
{
private:
	T* 		m_itemArray;		   
	short   m_itemCount;
	short   m_itemPitch;

public:	
	TArray(T* pData, short itemCount, short itemPitch = 0);
	TArray(const TArray& a);
    ~TArray();

	T& operator [] (int index);

	operator T*();
	operator void*();

	int Length() { return m_itemCount; };
	T*  Pointer() { return m_itemArray; };

#ifdef STL_STYLE
	T* begin();
	T* end();
#endif

};

#include "TArray.cpp"


#endif