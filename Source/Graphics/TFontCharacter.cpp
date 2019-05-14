/*
* Persistence Library / Graphics / TFontCharacter
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

#include "TFontCharacter.h"
#include "TFontDataAlignment.h"
#include "TPixelFormatConverter.h"
#include <string.h>

void TFontCharacter::LoadSingleCharacter(ePixelFormat pixelFormat, const unsigned short width, const unsigned short height, const unsigned char* pData, unsigned short dataLength, TFontDataAlignment dataAlignment)
{
    m_charPixelFormat = pixelFormat;
    m_charWidth = width;
    m_charHeight = height;
    m_charData = pData;
    m_charDataLength = dataLength;
    m_charDataAlignment = dataAlignment;
}

int TFontCharacter::ScanLine(unsigned short lineIndex, unsigned char* pLine, unsigned short maxLineSize)
{
    unsigned char offset = 0;
    unsigned char bitsPerPixel = 0;

    switch(m_charPixelFormat)
    {
        case pfBGR2ColorsPalette:   bitsPerPixel = 1;
        case pfBGR4ColorsPalette:   bitsPerPixel = 2;
        case pfBGR16ColorsPalette:  bitsPerPixel = 4;
        case pfBGR256ColorsPalette: bitsPerPixel = 8;
        case pfBGR888: bitsPerPixel = 24;
        case pfBGRA8888: bitsPerPixel = 32;
        case pfRGB332: bitsPerPixel = 8;
        case pfRGB555: bitsPerPixel = 16;
        case pfRGB565: bitsPerPixel = 16;
        case pfRGB888: bitsPerPixel = 32;
        case pfRGBA8888: bitsPerPixel = 32;
        case pfRGBX8888: bitsPerPixel = 32;
        default: pLine = NULL; return 0;
    }

    switch(m_charDataAlignment)
    {
        case ffHorizontalAligned: offset = lineIndex * ( (m_charWidth  * bitsPerPixel+7)/8); break;
        case ffHorizontal:        offset = lineIndex *   (m_charWidth  * bitsPerPixel/8);    break;
        case ffVerticalAligned:   offset = lineIndex * ( (m_charHeight * bitsPerPixel+7)/8); break;
        case ffVertical:          offset = lineIndex *   (m_charHeight * bitsPerPixel/8);    break;
        default: pLine = NULL; return 0;
    }
    memcpy( pLine, m_charData+offset, (m_charWidth*bitsPerPixel+7)/8 );

}

TColorRGB TFontCharacter::GetPixelColor(short x, short y)
{
    unsigned short offset = 0;
    unsigned short bitsPerPixel = 0;
    unsigned short bitIndex = 0;
    unsigned long  colorValue = 0;

    switch(m_charPixelFormat)
    {
        case pfBGR2ColorsPalette:   bitsPerPixel = 1; break;
        case pfBGR4ColorsPalette:   bitsPerPixel = 2; break;
        case pfBGR16ColorsPalette:  bitsPerPixel = 4; break;
        case pfBGR256ColorsPalette: bitsPerPixel = 8; break;
        case pfBGR888: bitsPerPixel = 24; break;
        case pfBGRA8888: bitsPerPixel = 32; break;
        case pfRGB332: bitsPerPixel = 8; break;
        case pfRGB555: bitsPerPixel = 16; break;
        case pfRGB565: bitsPerPixel = 16; break;
        case pfRGB888: bitsPerPixel = 32; break;
        case pfRGBA8888: bitsPerPixel = 32; break;
        case pfRGBX8888: bitsPerPixel = 32; break;
        default: return TColorRGB(0xFFFFFFFF); break;
    }

    switch(m_charDataAlignment)
    {
        case ffHorizontalAligned: bitIndex = y * ((m_charWidth  * bitsPerPixel+7)/8)*8  + x * bitsPerPixel; break;
        case ffHorizontal:        bitIndex = y *   m_charWidth  * bitsPerPixel          + x * bitsPerPixel; break;
        case ffVerticalAligned:   bitIndex = x * ((m_charHeight * bitsPerPixel+7)/8)*8  + y * bitsPerPixel; break;
        case ffVertical:          bitIndex = x *   m_charHeight * bitsPerPixel          + y * bitsPerPixel; break;
        default: return TColorRGB(0xFFFFFFFF);
    }

    TPixelFormatConverter converter;

    unsigned short n;
    switch(m_charDataAlignment)
    {
        case ffHorizontal:        offset = bitIndex / 8; n = (bitIndex & 7); break;
        case ffHorizontalAligned: offset = bitIndex / 8; n = x; break;
        case ffVertical:          offset = bitIndex / 8; n = (bitIndex & 7); break;
        case ffVerticalAligned:   offset = bitIndex / 8; n = y; break;
    }

    unsigned char tmp;
    switch(m_charPixelFormat)
    {
        case pfBGR2ColorsPalette:   tmp = m_charData[offset]; tmp = tmp << (n & 7);     return converter.DecodeColor(&tmp, 1, m_charPixelFormat);
        case pfBGR16ColorsPalette:  tmp = m_charData[offset]; tmp = tmp << ((n & 1)*4); return converter.DecodeColor(&tmp, 1, m_charPixelFormat);
        case pfBGR256ColorsPalette: tmp = m_charData[offset]; return converter.DecodeColor(&tmp, 1, m_charPixelFormat);
    }
    return converter.DecodeColor(m_charData + offset, 4, m_charPixelFormat);     
}

TSize TFontCharacter::GetSize()
{
    TSize result;
    result.Width = m_charWidth;
    result.Height = m_charHeight;
    return result;
}

