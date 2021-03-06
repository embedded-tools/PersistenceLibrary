/*
 * Persistence Library / FileUtils / TStringList
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

#ifndef EPIXELFORMAT___H
#define EPIXELFORMAT___H

enum ePixelFormat
{
    pfNone,
    pfBGR2ColorsPalette,
	pfBGR4ColorsPalette,
	pfBGR16ColorsPalette,
	pfBGR256ColorsPalette,
	pfRGB332,
	pfRGB555,
	pfRGB565,
	pfRGB888,
	pfRGBA8888,
	pfRGBX8888,
	pfBGR888,
	pfBGRA8888,
    pfDXT1,
    pfDXT3,
};

#endif
