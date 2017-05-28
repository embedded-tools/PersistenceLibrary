/*
 * Persistence Library / Collections / TIterator
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

#ifndef TENUMERATION___H
#define TENUMERATION___H

/* Notice - iterator inheritance does not work on 8-bit Atmel AVR MCUs,
            in such case you need to comment define statement below     */

#define TITERATOR_INHERITANCE

/**
 *	TIterator is an interface implemented by all collection classes
 *
 */
class TIterator
{
	public:
		virtual void*  First()=0;
		virtual void*  Next()=0;
		virtual short  Count()=0;
};

#endif
