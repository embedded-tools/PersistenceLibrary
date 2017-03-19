/*
 * Persistence Library / Graphics / TFontCharacter
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

#ifndef TFONTCHARACTER___H
#define TFONTCHARACTER___H

#include <stdlib.h>
#include "TRectangle.h"
#include "TFontDataAlignment.h"
#include "TColorRGB.h"
#include "TSize.h"
#include "EPixelFormat.h"

struct TFontCharacter
{    
private: 
	ePixelFormat            m_charPixelFormat;
    unsigned short          m_charWidth;
    unsigned short          m_charHeight;
    const unsigned char*    m_charData;
    unsigned short          m_charDataLength;
    TFontDataAlignment      m_charDataAlignment;	

public:

	void        LoadSingleCharacter(ePixelFormat pixelFormat, const unsigned short width, const unsigned short height, const unsigned char* pData, unsigned short dataLength, TFontDataAlignment dataAlignment);
	TColorRGB	GetPixelColor(short x, short y);
	int         ScanLine(unsigned short lineIndex, unsigned char* pLine, unsigned short maxLineSize);
	TSize       GetSize();

};

#endif
