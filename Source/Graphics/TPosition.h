/*
 * Persistence Library / Graphics / TPosition
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

#ifndef TPOSITION___H
#define TPOSITION___H

class TPosition
{

public:
	TPosition();
	TPosition(short x, short y);
	TPosition(const TPosition& src);
	short X;
	short Y;

	TPosition& operator=(TPosition &pos);

};

#endif
