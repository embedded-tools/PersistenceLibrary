/*
 * Persistence Library / Graphics / TRectangle
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

#ifndef TSIZE___H
#define TSIZE___H

class TSize
{
public:
    short Width;
    short Height;

	TSize();
	TSize(const TSize &src);
    TSize(short width, short height);	
	TSize& operator = (const TSize &scr);

};

#endif

