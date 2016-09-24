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

#include "TColorConverter.h"

TColorYUV TColorConverter::RGBtoYUV(TColorRGB rgb)
{
	TColorYUV result(( 76*rgb.R+150*rgb.G+ 29*rgb.B + 128)/255,
					 (-43*rgb.R -84*rgb.G+128*rgb.B - 64)/255,
		             (127*rgb.R-107*rgb.G- 20*rgb.B + 64)/255);
	return result;
}

TColorRGB TColorConverter::YUVToRGB(TColorYUV yuv)
{
	TColorRGB result((255*yuv.Y + 358*yuv.V + 128)/255,
		             (255*yuv.Y -  88*yuv.U - 182*yuv.V)/255,
		             (255*yuv.Y + 451*yuv.U + 128)/255
				    );
	return result;
}
