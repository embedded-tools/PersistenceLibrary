/*
 * Persistence Library / Graphics / TDXTBlockCreator
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

#ifndef TDXTCOMPRESSION___H
#define TDXTCOMPRESSION___H

#include "TColorRGB.h"
#include "TColorYUV.h"

struct TDxtPixel
{
	TColorRGB rgb;
	TColorYUV yuv;
	char colorIndex;
};

class TDxtBlockCreator
{
	private:

        int  abs(short i);
		long AssignColors(TColorYUV yuv1, TColorYUV yuv4, short numberOfColors);
		void LeastSquare (short* valueArray, short& bestValue1, short &bestValue2, short numberOfColors);		

	public:

		TDxtBlockCreator();
		TColorRGB Color0;
		TColorRGB Color1;		
		TColorRGB TransparentColor;
		bool      TransparentColorUsed;
		TDxtPixel Pixel[16];

		void DoBlockCompression();		
};

#endif
