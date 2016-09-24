/*
 * Persistence Library / Graphics / TColorConverter
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

#include "TColorRGB.h"
#include "TColorYUV.h"

class TColorConverter
{
public:
	static TColorYUV RGBtoYUV(TColorRGB rgb);	
	static TColorRGB YUVToRGB(TColorYUV yuv);

};

