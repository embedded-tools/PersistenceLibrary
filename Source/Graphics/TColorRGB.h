/*
 * Persistence Library / Graphics / TColorRGB
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

#ifndef TCOLOR_RGB___H
#define TCOLOR_RGB___H

struct TColorYUV;

struct TColorRGB
{

public:
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	TColorRGB();
    TColorRGB(short r, short g, short b);
    TColorRGB(unsigned long rgb);
	
	unsigned short ConvertToRGB565();
	TColorRGB operator = (TColorRGB color);
	TColorRGB operator = (unsigned long color);
	bool operator == (TColorRGB& color);	
};

#endif
