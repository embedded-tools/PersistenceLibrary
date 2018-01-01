 /*
 * Persistence Library / Collections / TIterator
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

#ifndef TENUMERATOR___H
#define TENUMERATOR___H

/**
 *	TIterator is an interface implemented by all collection classes
 *
 */

template <typename T> 
class TEnumerator
{
	protected:
		T*            m_current;
		T*            m_last;

	public:
		TEnumerator();
		TEnumerator(T* firstItem, T* lastItem);
		TEnumerator(const TEnumerator& enumerator);

		T&    Current();
		bool  MoveNext();
};

#include "TEnumerator.cpp"

#endif
