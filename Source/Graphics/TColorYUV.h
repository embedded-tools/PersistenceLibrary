/*
 * Persistence Library / Graphics / TColorYUV
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

#ifndef TCOLOR_YUV___H
#define TCOLOR_YUV___H

struct TColorRGB;

struct TColorYUV
{

public:
	TColorYUV();
    TColorYUV(short y, short u, short v);

	unsigned char Y;
	char U;
	char V;

	TColorRGB ConvertToRGB();

	bool operator == (TColorYUV& color);

};

#endif
