/*
 * Persistence Library / Graphics / TFont
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

#include "TFont.h"
#include "TFontDataAlignment.h"
#include <string.h>

TFont::TFont()
{

}

TFont::~TFont()
{

}


void TFont::LoadMonospacedFont(ePixelFormat fontFormat, const unsigned short width, const unsigned short height, const unsigned char* pData, unsigned short dataLength, TFontDataAlignment fontDataAlignment, unsigned char characterCount)
{
	m_fontPixelFormat = fontFormat;
	m_fontDataAlignment = fontDataAlignment;
	m_fontCharacterCount = characterCount;
	m_fontData = pData;
	m_fontDataLength = dataLength;
	m_fontIndexData = NULL;
	m_fontIndexDataLength = 0;	
    m_fontWidth = width;
    m_fontHeight = height;
    CalculateBitsPerRow(fontFormat, width, height, fontDataAlignment);
}

void TFont::LoadVariableLengthFont(ePixelFormat fontFormat, const unsigned short width, const unsigned short height, const unsigned char* pData, unsigned short dataLength, TFontDataAlignment fontDataAlignment, const unsigned short* pIndexData, unsigned short indexDataLength)
{
	m_fontPixelFormat = fontFormat;
    m_fontDataAlignment = fontDataAlignment;
	m_fontCharacterCount = indexDataLength/2-1;
	m_fontData = pData;
	m_fontDataLength = dataLength;
	m_fontIndexData = pIndexData;
	m_fontIndexDataLength = indexDataLength;	
    m_fontWidth = width;
    m_fontHeight = height;
    CalculateBitsPerRow(fontFormat, width, height, fontDataAlignment);
}

void TFont::CalculateBitsPerRow(ePixelFormat fontFormat, const unsigned short width, const unsigned short height, TFontDataAlignment fontDataAlignment)
{
    unsigned short n = 0;
    if ((fontDataAlignment==ffHorizontal) || (fontDataAlignment==ffHorizontalAligned))
    {
        n = width;
    } else {
        n = height;
    }
    switch (fontFormat)
    {
        case pfBGR2ColorsPalette:  m_fontBitsPerRow = n; break;
        case pfBGR4ColorsPalette:  m_fontBitsPerRow = n*2; break;
        case pfBGR16ColorsPalette: m_fontBitsPerRow = n*4; break;
        case pfBGR256ColorsPalette: m_fontBitsPerRow = n*8;    break;

        case pfRGB332: m_fontBitsPerRow = n*8; break;
        case pfRGB555: 
        case pfRGB565: m_fontBitsPerRow = n*16; break;
        case pfBGR888:
        case pfRGB888: m_fontBitsPerRow = n*24; break;            
        case pfBGRA8888:
        case pfRGBA8888: 
        case pfRGBX8888: m_fontBitsPerRow = n*32; break;              
        default: m_fontBitsPerRow = 0; break;
    }
    if (
        (fontDataAlignment==ffHorizontalAligned) ||
        (fontDataAlignment==ffVerticalAligned)
        )
    {
        while(m_fontBitsPerRow&7) m_fontBitsPerRow++;
    }
}



bool TFont::GetCharacter(char ascii, TFontCharacter& charData)
{
    memset(&charData, 0, sizeof(charData));
    if (m_fontIndexData==NULL)
    {
        unsigned short bytesPerCharacter = m_fontDataLength / m_fontCharacterCount;

        charData.LoadSingleCharacter(m_fontPixelFormat, m_fontWidth, m_fontHeight, m_fontData + bytesPerCharacter*(ascii-32), bytesPerCharacter, m_fontDataAlignment  );        
        return true;
    } 

    if ((ascii-32)>m_fontCharacterCount)
    {
        memset(&charData, 0, sizeof(charData));
        return false;
    }
    unsigned short charDataLength = m_fontIndexData[ascii-31] - m_fontIndexData[ascii-32];
    unsigned short charWidth = charDataLength*8/m_fontBitsPerRow;

    charData.LoadSingleCharacter(m_fontPixelFormat, charWidth, m_fontHeight, m_fontData + m_fontIndexData[ascii-32], charDataLength, m_fontDataAlignment);
    return true;
}