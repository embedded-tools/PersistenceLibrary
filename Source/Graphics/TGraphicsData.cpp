/*
 * Persistence Library / Graphics / TGraphicsData
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

#include "TGraphicsData.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TGraphicsData::TGraphicsData(short width, short height, ePixelFormat pixelFormat)
:	m_bitmapData(NULL),
	m_bitmapNeedsUnalloc(false),
	m_bitmapDataSize(0),
	m_bytesPerLine(0),
	m_transparentColor(0),
	m_transparentColorUsed(false),
	m_bitmapWidth(0),
	m_bitmapHeight(0),
	m_planes(0),
	m_pixelFormat(pfNone),
	m_colorPalette(NULL),
	m_colorPaletteSize(0),  
	m_colorPaletteNeedsUnalloc(false),
	m_colorCount(0),
	m_flipImage(false)
{
	short bitsPerPixel = 0;

	m_pixelFormat        = pixelFormat;

	if ((width==0) || (height==0)) return;
	bitsPerPixel         = GetBitsPerPixel();
	m_bitmapWidth        = width;
	m_bitmapHeight       = height;	
	m_planes             = 1;	
	m_bytesPerLine       = width * bitsPerPixel / 8;
	m_bitmapDataSize     = m_bytesPerLine * height;
	m_bitmapData         = (unsigned char*)malloc(m_bitmapDataSize);
	m_bitmapNeedsUnalloc = true;
}



TGraphicsData::~TGraphicsData()
{
    if (m_bitmapNeedsUnalloc)
    {
        if (m_bitmapData!=NULL)
        {
            free(m_bitmapData);
            m_bitmapData = NULL;
        }
    }
    if (m_colorPaletteNeedsUnalloc)
    {
        if (m_colorPalette!=NULL)
        {
            free(m_colorPalette);
            m_colorPalette = NULL;
        }
    }
}

short TGraphicsData::GetWidth()
{
	return m_bitmapWidth;
}

short TGraphicsData::GetHeight()
{
	return m_bitmapHeight;
}

ePixelFormat TGraphicsData::GetPixelFormat()
{
	return m_pixelFormat;
}

unsigned short TGraphicsData::GetBytesPerLine()
{
	return m_bytesPerLine;
}

unsigned long TGraphicsData::GetBytesTotal()
{
	return m_bytesPerLine*m_bitmapHeight;
}

short  TGraphicsData::GetBitsPerPixel()
{
    switch(m_pixelFormat)
    {
        case pfBGR2ColorsPalette: return 1;
		case pfBGR4ColorsPalette: return 2;
		case pfBGR16ColorsPalette: return 4;
		case pfBGR256ColorsPalette: return 8;
        case pfRGB332:   return 8;
        case pfRGB555:
        case pfRGB565:   return 16;
        case pfRGB888:   return 24;
        case pfRGBA8888: return 32;
        case pfDXT1:     return 16;
        case pfDXT3:     return 32;
    }
    return 0;
}

unsigned char* TGraphicsData::ScanLine(short line)
{
    if (!m_flipImage)
    {
        if (line<0) return NULL;
        if (line>=m_bitmapHeight) return NULL;
        return m_bitmapData + m_bytesPerLine * line;
    } else {
		if (line<0) return NULL;
        if (line>=m_bitmapHeight) return NULL;        
        return m_bitmapData + m_bytesPerLine * (m_bitmapHeight - 1 - line);
    }   
}

TColorRGB TGraphicsData::GetPixel(short x, short y)
{
	TColorRGB color;
	short     colorIndex;

	unsigned char* pp = ScanLine(y);
	if (pp==NULL) return color;

	pp+= (x * GetBitsPerPixel()) / 8;

	switch(m_pixelFormat)
	{
		case pfRGB332:  color.R = (*pp) & 0xE0; 
			            color.G = ((*pp) & 0x1C)<<3; 
					    color.B = ((*pp) & 0x03)<<6; 
					    break;

		case pfRGB555:  color.R = (pp[0]) & 0x7C; 
					    color.G = (((pp[0]) & 0x03)<<6) |
				                  (((pp[1]) & 0xE0)>>2);
						color.B = ((pp[1]) & 0x1F)<<3; 
						break;

		case pfRGB565:	color.R = (pp[0])   & 0xF8; 
						color.G = (((pp[0]) & 0x03)<<6) |
						          (((pp[1]) & 0xE0)>>2);
						color.B = ((pp[1])  & 0x1F)<<3; 
						break;

		case pfRGB888:	color.R = *pp++;
						color.G = *pp++;
						color.B = *pp++;
						break;

		case pfRGBX8888:
		case pfRGBA8888:color.R = *pp++;
						color.G = *pp++;
						color.B = *pp++;
						color.A = *pp;
						break;

		case pfBGR888:  color.B = *pp++;
						color.G = *pp++;
						color.R = *pp++;
						break;

		case pfBGR2ColorsPalette:
						switch(x & 7)
						{
							case 0: colorIndex = ((*pp) & 0x80)? 1 : 0; break;
							case 1: colorIndex = ((*pp) & 0x40)? 1 : 0; break;
							case 2: colorIndex = ((*pp) & 0x20)? 1 : 0; break;
							case 3: colorIndex = ((*pp) & 0x10)? 1 : 0; break;
							case 4: colorIndex = ((*pp) & 0x08)? 1 : 0; break; 
							case 5: colorIndex = ((*pp) & 0x04)? 1 : 0; break;
							case 6: colorIndex = ((*pp) & 0x02)? 1 : 0; break;
							case 7: colorIndex = ((*pp) & 0x01)? 1 : 0; break;
						}
						pp = m_colorPalette + colorIndex*3;
						color.B = *pp++;
						color.G = *pp++;
						color.R = *pp++;
						break;

		case pfBGR4ColorsPalette:
						switch(x & 7)
						{
							case 0: colorIndex = ((*pp) & 0xC0) >> 6; break;
							case 1: colorIndex = ((*pp) & 0x30) >> 4; break;
							case 2: colorIndex = ((*pp) & 0x0C) >> 2; break;
							case 3: colorIndex = ((*pp) & 0x03);      break;
						}
						pp = m_colorPalette + colorIndex*3;
						color.B = *pp++;
						color.G = *pp++;
						color.R = *pp++;
						break;

		case pfBGR16ColorsPalette:
						switch(x & 7)
						{
							case 0: colorIndex = ((*pp) & 0xC0) >> 6; break;
							case 1: colorIndex = ((*pp) & 0x30) >> 4; break;
							case 2: colorIndex = ((*pp) & 0x0C) >> 2; break;
							case 3: colorIndex = ((*pp) & 0x03);      break;
						}
						pp = m_colorPalette + colorIndex*3;
						color.B = *pp++;
						color.G = *pp++;
						color.R = *pp++;
						break;

		case pfBGR256ColorsPalette:
						colorIndex = *pp; 
						pp = m_colorPalette + colorIndex*3;
						color.B = *pp++;
						color.G = *pp++;
						color.R = *pp++;
						break;						
	}
	return color;
}

void            SetPixel(short x, short y, TColorRGB color);

unsigned char*  TGraphicsData::GetPalette()
{
	return m_colorPalette;
}

short TGraphicsData::GetColorCount()
{
	return m_colorCount;
}

TColorRGB TGraphicsData::GetTransparentColor()
{
	return m_transparentColor;
}

bool      TGraphicsData::IsTransparentColorUsed()
{
	return m_transparentColorUsed;
}

void TGraphicsData::SetTransparentColor(TColorRGB color)
{
	m_transparentColorUsed = true;
	m_transparentColor = color;
}

void TGraphicsData::ClearTransparentColor()
{
	m_transparentColorUsed = false;
	m_transparentColor = 0x00000;
}

bool TGraphicsData::LoadFromROM (const unsigned char* image)
{
	unsigned long* headerOffsets	= (unsigned long*)(image+2);
	unsigned long totalLength		= *headerOffsets++;
	m_transparentColor              = *headerOffsets++;
	unsigned long bitmapOffset		= *headerOffsets++;
	unsigned long headerSize		= *headerOffsets++;
	unsigned short flags            = 0;
    unsigned short bitsperpixel     = 0;

	if (image[0]!='B') return false;
	if (image[1]!='M') return false;

	m_bitmapData					= (unsigned char*)image + (*headerOffsets++);
	m_bitmapDataSize				= totalLength - bitmapOffset; 
	m_bitmapNeedsUnalloc			= false;
	
	if (headerSize != 14)
	{
		return false;
	}

	//standard os2/bitmap params
	unsigned short* headerParams = (unsigned short*)headerOffsets;
	m_bitmapWidth  = *headerParams++;
	m_bitmapHeight = *headerParams++;
	m_planes       = *headerParams++;
	bitsperpixel   = *headerParams++;
	flags          = *headerParams++;

	m_pixelFormat  = (ePixelFormat)(flags & 255);
	m_transparentColorUsed	= (flags & 0x0100) ? true : false;
	m_flipImage				= (flags & 0x0200) ? true : false;

	m_colorPaletteSize = totalLength - headerSize - 18;	
	if (m_colorPaletteSize>0)
	{
		m_colorPalette = (unsigned char*)headerParams;
		m_colorCount = (unsigned short)(m_colorPaletteSize / 3);
		
	} else {
		m_colorPaletteSize = 0;
		m_colorPalette = NULL;
	}
	return true;
}

bool TGraphicsData::LoadFromFile (const char* filename)
{
	unsigned char magicByte1    = 0;
	unsigned char magicByte2    = 0;
	unsigned long totalLength   = 0;
	unsigned long bitmapOffset  = 0;
	unsigned long headerSize    = 0;
	unsigned short flags        = 0;
	unsigned short bitsperpixel = 0;
	unsigned short bytesRead    = 0;

	FILE* file = fopen(filename, "rb");
	if (file==NULL)
	{
		return false;
	}
	fread(&magicByte1, 1, 1, file);
	fread(&magicByte2, 1, 1, file);
	if (magicByte1!='B')
	{
		fclose(file);
		return false;
	}
	if (magicByte2!='M')
	{
		fclose(file);
		return false;
	}

	bytesRead += fread(&totalLength, 1, 4, file);
	bytesRead += fread(&m_transparentColor, 1, 4, file);
	bytesRead += fread(&bitmapOffset, 1, 4, file);
	bytesRead += fread(&headerSize, 1, 4, file);
	if (bytesRead!=18)
	{
		fclose(file);
		return false;
	}
	if (headerSize != 14)
	{
		fclose(file);
		return false;
	}

	fread(&m_bitmapWidth, 1, 2, file);
	fread(&m_bitmapHeight, 1, 2, file);
	fread(&m_planes, 1, 2, file);
	fread(&bitsperpixel, 1, 2, file);
	fread(&flags, 1, 2, file);
	
	m_transparentColorUsed	= (flags & 0x0100) ? true : false;
	m_flipImage				= (flags & 0x0200) ? true : false;
	m_pixelFormat           = (ePixelFormat)(flags  & 0x00FF);

	m_colorPaletteSize = totalLength - headerSize - 18;	
	if (m_colorPaletteSize>0)
	{
		m_colorPalette = (unsigned char*)malloc(m_colorPaletteSize);			
		if (m_colorPalette)
		{
			fread(m_colorPalette, 1, m_colorPaletteSize, file);
			m_colorCount = (unsigned short)(m_colorPaletteSize / 3);
		} else {
			m_colorCount = 0;
			m_colorPaletteSize = 0;
			fclose(file);
			return false;
		}		
	} else {
		m_colorPaletteSize = 0;
		m_colorPalette = NULL;
	}
	m_bitmapDataSize = totalLength - bitmapOffset; 
	m_bitmapData	 = (unsigned char*)malloc(m_bitmapDataSize);
	if (m_bitmapData==NULL)
	{
		m_bitmapDataSize = 0;
		fclose(file);
		return false;
	}
	m_bitmapNeedsUnalloc			= false;

	fread(m_bitmapData, 1, m_bitmapDataSize, file);
	fclose(file);
	return true;
}

