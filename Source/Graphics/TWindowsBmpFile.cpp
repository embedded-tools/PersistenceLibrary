/*
 * Persistence Library / Graphics / TWindowsBmpFile
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

#include "TWindowsBmpFile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TWindowsBmpFile::TWindowsBmpFile()
    :TGraphicsData(0,0,pfNone)
{

}

TWindowsBmpFile::TWindowsBmpFile(short width, short height, ePixelFormat pixelFormat, unsigned long backgroundColor, bool flipImage)
    :TGraphicsData(0,0,pixelFormat)
{
	TColorRGB tmpColor;
	unsigned char convertedBackgroundColor[4];

    memset(convertedBackgroundColor, 0, sizeof(convertedBackgroundColor));

    if (pixelFormat==pfBGR4ColorsPalette)
    {
        //windows bitmap does not support 2 bits per pixel
        return;
    }

    m_bitmapWidth  = width;
    m_bitmapHeight = height;
    m_pixelFormat  = pixelFormat;
    m_bytesPerLine = width * GetBitsPerPixel() / 8;
    while (m_bytesPerLine & 3) m_bytesPerLine++;
    m_bitmapDataSize = m_bytesPerLine * height;
    m_bitmapData = (unsigned char*)malloc(m_bitmapDataSize);
    m_bitmapNeedsUnalloc = true;
	m_flipImage = flipImage;
	
	switch(m_pixelFormat)
	{
        case pfBGR2ColorsPalette:
            {
                CreatePalette(2);
                if (m_colorPalette)
                {
                    m_colorPalette[0] = 0;    m_colorPalette[1] = 0;    m_colorPalette[2] = 0; 
                    m_colorPalette[3] = 0xFF; m_colorPalette[4] = 0xFF; m_colorPalette[5] = 0xFF;         
                }
                convertedBackgroundColor[0] = backgroundColor;
            }
            break;
        case pfBGR4ColorsPalette:
            {
                CreatePalette(4);
                if (m_colorPalette)
                {
                    m_colorPalette[0] = 0;    m_colorPalette[1] = 0;     m_colorPalette[2] = 0;
                    m_colorPalette[3] = 0x50; m_colorPalette[4] = 0x55;  m_colorPalette[5] = 0x55;
                    m_colorPalette[6] = 0xAA; m_colorPalette[7] = 0xAA;  m_colorPalette[8] = 0xAA;
                    m_colorPalette[9] = 0xFF; m_colorPalette[10] = 0xFF; m_colorPalette[11]= 0xFF;
                }
                convertedBackgroundColor[0] = backgroundColor;
            }
            break;
        case pfBGR16ColorsPalette:
            {
                CreatePalette(16);
                if (m_colorPalette)
                {
                    int n = 0;
                    for(int i = 0; i<m_colorCount; i++)
                    {
                        m_colorPalette[n++] = i*16+8;
                        m_colorPalette[n++] = i*16+8;
                        m_colorPalette[n++] = i*16+8;
                    }
                }
                convertedBackgroundColor[0] = backgroundColor;
            }
            break;
        case pfBGR256ColorsPalette:
            {
                CreatePalette(256);
                if (m_colorPalette)
                {
                    int n = 0;
                    for(int i = 0; i<m_colorCount; i++)
                    {
                        m_colorPalette[n++] = i;
                        m_colorPalette[n++] = i;
                        m_colorPalette[n++] = i;
                    }
                }
                convertedBackgroundColor[0] = backgroundColor;
            }
            break;

		case pfBGR888:
		case pfBGRA8888: memcpy(convertedBackgroundColor, (void*)&backgroundColor, 4); break;
		case pfRGB888:
		case pfRGBA8888:
		case pfRGBX8888:
		{
			convertedBackgroundColor[0] = (unsigned char)(backgroundColor>>16);
			convertedBackgroundColor[1] = (unsigned char)(backgroundColor>>8);
			convertedBackgroundColor[2] = (unsigned char)(backgroundColor);
			convertedBackgroundColor[3] = (unsigned char)(backgroundColor>>24);
		}
		break;
		case pfRGB565:
		{
			tmpColor = backgroundColor;
			backgroundColor = tmpColor.ConvertToRGB565();
			convertedBackgroundColor[0] = (unsigned char)backgroundColor;
			convertedBackgroundColor[1] = (unsigned char)(backgroundColor>>8);
		}
		break;
	}

    for(short i = 0; i<height; i++)
    {
        unsigned char* cc = (unsigned char*)ScanLine(i);
        short bytesPerPixel = GetBitsPerPixel()/8;
        if (bytesPerPixel==0) bytesPerPixel = 1;

		unsigned char* src = (unsigned char*)cc;
		unsigned char* dst = src + bytesPerPixel;
		memcpy(src, &convertedBackgroundColor, 4);
		for(short j = m_bytesPerLine-bytesPerPixel; j>0; j--)
		{
			*dst++=*src++;
		}
    }
}

bool TWindowsBmpFile::LoadFromWindowsBmp(const char* filename)
{
    TBitmapHeader header;
	unsigned long bytesRead = 0;
	unsigned char tmp;

    FILE* hFile = fopen(filename, "rb");
    if (hFile==NULL)
    {
        return false;
    }
    
    bytesRead += fread(&header.usMagicBytes, 1, 2, hFile);
    if (header.usMagicBytes!=0x4D42)
    {
        fclose(hFile);
        return false;
    }
    bytesRead += fread(&header.ulTotalLength, 1, 4, hFile);
    bytesRead += fread(&header.ulReserved,    1, 4, hFile);
    bytesRead += fread(&header.ulBitmapDataOffset, 1, 4, hFile);
    bytesRead += fread(&header.ulBitmapHeaderSize, 1, 4, hFile);
    bytesRead += fread(&header.ulWidth, 1, 4, hFile);
    bytesRead += fread(&header.ulHeight, 1, 4, hFile);
    bytesRead += fread(&header.usPlanes, 1, 2, hFile);
    bytesRead += fread(&header.usBitsPerPixel, 1, 2, hFile);
    bytesRead += fread(&header.ulCompression, 1, 4, hFile);
    bytesRead += fread(&header.ulBitmapDataSize, 1, 4, hFile);
    bytesRead += fread(&header.ulPixelsPerMeterHorizontal, 1, 4, hFile);
    bytesRead += fread(&header.ulPixelsPerMeterVertical, 1, 4, hFile);
    bytesRead += fread(&header.ulNumberOfColors, 1, 4, hFile);
    bytesRead += fread(&header.ulNumberOfImportantColors, 1, 4, hFile);	
    switch(header.usBitsPerPixel)
    {
		case 1:  m_pixelFormat = pfBGR2ColorsPalette; break;
		case 2:  m_pixelFormat = pfBGR4ColorsPalette; break;
		case 4:  m_pixelFormat = pfBGR16ColorsPalette; break;
		case 8:  m_pixelFormat = pfBGR256ColorsPalette; break;
		case 24: m_pixelFormat = pfBGR888; break;
		case 32: m_pixelFormat = pfBGRA8888; break;
	}

    if (bytesRead!=54)
    { 
        fclose(hFile);
        return false;
    }
    if ((header.ulBitmapDataOffset<0) || (header.ulBitmapHeaderSize<0) || (header.ulWidth<0) || (header.ulWidth>131072) || (header.ulHeight<0) || (header.ulHeight>131072))
    {
        fclose(hFile);
        return false;
    }
    if (header.ulBitmapDataOffset!=54)
    {
        m_colorPaletteSize = header.ulBitmapDataOffset - (sizeof(header) - 2);
        if ((m_colorPaletteSize>2048) || (m_colorPaletteSize<0))
        {
            fclose(hFile);
            return false;
        }
		m_colorCount = (unsigned short)(m_colorPaletteSize/4);

        m_colorPalette = (unsigned char*)malloc(m_colorCount*3);
		for(int i = 0; i<m_colorCount; i++)
		{
			fread(m_colorPalette+i*3, 1, 3, hFile);
			fread(&tmp, 1, 1, hFile);
		}
    }
   
    m_bitmapDataSize = header.ulTotalLength-header.ulBitmapDataOffset;
    m_bitmapData = (unsigned char*)malloc(m_bitmapDataSize);
    m_bitmapNeedsUnalloc = true;
    m_bitmapWidth = (short)header.ulWidth;
    m_bitmapHeight = (short)header.ulHeight;
    m_bytesPerLine = (short)(m_bitmapDataSize / m_bitmapHeight);
	m_flipImage = true;

    bytesRead = fread(m_bitmapData, 1, m_bitmapDataSize, hFile);
    fclose(hFile);

	if ((m_pixelFormat==pfBGR888) || (m_pixelFormat==pfBGRA8888))
	{
		SwitchRedAndBlue();
	}
    return true;
}

bool TWindowsBmpFile::SaveToWindowsBmp(const char* filename)
{
    TBitmapHeader header;
	int bytesWritten = 0;
	unsigned char tmp = 0;
    if (m_colorPalette==NULL)
    {
        m_colorPaletteSize = 0;
        m_colorCount = 0;
    }

    header.usMagicBytes = 0x4D42;
    header.ulTotalLength = 54 + m_colorPaletteSize + m_bytesPerLine * m_bitmapHeight;
    header.ulReserved = 0;
    header.ulBitmapDataOffset = 54 + m_colorCount*4;
    header.ulBitmapHeaderSize = 40;
    header.ulWidth = m_bitmapWidth;
    header.ulHeight = m_bitmapHeight;
    header.usPlanes = 1;
    if (m_bitmapWidth==0)
    {
        header.usBitsPerPixel = 0;
    } else {
        header.usBitsPerPixel = m_bytesPerLine * 8 /m_bitmapWidth;
    }    
    header.ulCompression = 0;
    header.ulBitmapDataSize = m_bytesPerLine * m_bitmapHeight;
    header.ulPixelsPerMeterHorizontal = 3000;
    header.ulPixelsPerMeterVertical = 3000;
    header.ulNumberOfColors = 0;
    header.ulNumberOfImportantColors = 0;
    
    FILE* hFile = fopen(filename, "wb");
    if (hFile==NULL)
    {
        return false;
    }
    bytesWritten += fwrite(&header.usMagicBytes, 1, 2, hFile);
    bytesWritten += fwrite(&header.ulTotalLength, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulReserved,    1, 4, hFile);
    bytesWritten += fwrite(&header.ulBitmapDataOffset, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulBitmapHeaderSize, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulWidth,  1, 4, hFile);
    bytesWritten += fwrite(&header.ulHeight, 1, 4, hFile);
    bytesWritten += fwrite(&header.usPlanes, 1, 2, hFile);
    bytesWritten += fwrite(&header.usBitsPerPixel, 1, 2, hFile);
    bytesWritten += fwrite(&header.ulCompression, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulBitmapDataSize, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulPixelsPerMeterHorizontal, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulPixelsPerMeterVertical, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulNumberOfColors, 1, 4, hFile);
    bytesWritten += fwrite(&header.ulNumberOfImportantColors, 1, 4, hFile);
    if (bytesWritten!=54)
    {
        fclose(hFile);
        return false;
    }
    if (m_colorPalette!=NULL)
    {
		tmp = 0;
		for(int i = 0; i<m_colorCount; i++)
		{			
			fwrite(m_colorPalette + i*3, 1, 3, hFile);
			fwrite(&tmp, 1, 1, hFile);
		}        
    }
	
	bool needsToSwitchComponents = (m_pixelFormat==pfRGB888) || (m_pixelFormat==pfRGBA8888);

	if (needsToSwitchComponents) SwitchRedAndBlue();
    unsigned long n = fwrite(m_bitmapData, 1, header.ulBitmapDataSize, hFile);
	if (needsToSwitchComponents) SwitchRedAndBlue();
    fclose(hFile);

    return n == header.ulBitmapDataSize;
}

//Windows bitmap file uses unusual BGR pixel format.
//Therefore this method switches Red and Blue component
//to achieve more used RGB format
void TWindowsBmpFile::SwitchRedAndBlue()
{
	switch(m_pixelFormat)
	{
		case pfBGR888:
		case pfRGB888:
			{
				for(short cy = 0; cy<GetHeight(); cy++)
				{
					unsigned char x;
					unsigned char* pp = ScanLine(cy);
					for (short cx = 0; cx<GetWidth(); cx++)
					{
						x = pp[0];
						pp[0] = pp[2];
						pp[2] = x;
						pp+=3;
					}
				}			
				if (m_pixelFormat == pfRGB888)
				{
					m_pixelFormat = pfBGR888;
				} else {
					m_pixelFormat = pfRGB888;
				}
			}
			break;
		case pfBGRA8888:
		case pfRGBA8888:
			{
				for(short cy = 0; cy<GetHeight(); cy++)
				{
					unsigned char x;
					unsigned char* pp = ScanLine(cy);
					for (short cx = 0; cx<GetWidth(); cx++)
					{
						x = pp[0];
						pp[0] = pp[2];
						pp[2] = x;
						pp+=4;
					}
				}			
				if (m_pixelFormat == pfRGBA8888)
				{
					m_pixelFormat = pfBGRA8888;
				} else {
					m_pixelFormat = pfRGBA8888;
				}
			}
			break;
	}
}

