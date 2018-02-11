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

#ifndef TFONT___H
#define TFONT___H

#include <stdlib.h>
#include "TFontCharacter.h"

class TFont
{
	private:		
		ePixelFormat		    m_fontPixelFormat;
		TFontDataAlignment      m_fontDataAlignment;
		unsigned char		    m_fontCharacterCount;
		const unsigned char*	m_fontData;
		unsigned short		    m_fontDataLength;			
		const unsigned short*	m_fontIndexData;
		unsigned short		    m_fontIndexDataLength;
        unsigned short          m_fontWidth;
        unsigned short          m_fontHeight;
        unsigned short          m_fontBitsPerRow;
		unsigned short*         m_specialCharsTable;

        void CalculateBitsPerRow(ePixelFormat fontFormat, const unsigned short width, const unsigned short height, TFontDataAlignment fontDataAlignment);

	public:

	TFont();
	virtual ~TFont();
	
    void LoadMonospacedFont(ePixelFormat fontFormat, const unsigned short width, const unsigned short height, const unsigned char* pData, unsigned short dataLength, TFontDataAlignment fontDataAlignment, unsigned char characterCount);
    void LoadVariableLengthFont(ePixelFormat fontFormat, const unsigned short width, const unsigned short height, const unsigned char* pData, unsigned short dataLength, TFontDataAlignment fontDataAlignment, const unsigned short* pIndexData, unsigned short indexDataLength);

    
    bool GetCharacter(char ascii, TFontCharacter& charData);

};

#endif
