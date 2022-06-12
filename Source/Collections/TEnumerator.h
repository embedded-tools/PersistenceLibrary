 /*
 * Persistence Library / Collections / TIterator
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

#ifndef TENUMERATOR___H
#define TENUMERATOR___H

/**
 *	TIterator is an interface implemented by all collection classes
 *
 */

#include "TArray.h"

template <typename T>
class TEnumerator
{
	private:
		T*            m_current;
		T*            m_last;
		short         m_pitch;

		T* Inc(T* ptr, int n);
		T* Dec(T* ptr, int n);

	public:
		TEnumerator();
		TEnumerator(TArray<T> a);
		TEnumerator(T* firstItem, T* lastItem);

		T&    Current();
		bool  MoveNext();
};

#include "TEnumerator.cpp"

#endif
