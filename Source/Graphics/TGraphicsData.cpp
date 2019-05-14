/*
* Persistence Library / Graphics / TGraphicsData
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

#include "TGraphicsData.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TGraphicsData::TGraphicsData(short width, short height, ePixelFormat pixelFormat, TColorRGB backgroundColor)
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
    int tmp = width;
    if ((pixelFormat==pfDXT1) || (pixelFormat==pfDXT3))
    {
        while(tmp & 3)
        {
            tmp++;
        }
    }
    m_bytesPerLine       = (tmp * bitsPerPixel + 7) / 8;
    m_bitmapDataSize     = m_bytesPerLine * height;
    m_bitmapData         = (unsigned char*)malloc(m_bitmapDataSize);
    memset(m_bitmapData, 0, m_bitmapDataSize);
    m_bitmapNeedsUnalloc = true;
}

void TGraphicsData::Clear()
{
    if (m_bitmapData)
    {
        if (m_bitmapNeedsUnalloc)
        {
            free(m_bitmapData);            
        }
        m_bitmapData = NULL;
    }
    m_bitmapWidth = 0;
    m_bitmapHeight = 0;    
    m_bytesPerLine = 0;
    m_planes = 0;
    m_pixelFormat = pfNone;

    m_bitmapNeedsUnalloc = false;
    if (m_colorPalette)
    {
        if (m_colorPaletteNeedsUnalloc)
        {
            free(m_colorPalette);
        }
        m_colorPalette = NULL;
    }
    m_colorPaletteSize = 0;
    m_colorPaletteNeedsUnalloc = false;
    m_colorCount = 0;
    m_flipImage = false;
    m_readOnly = false;

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
    case pfBGR888:
    case pfRGB888:   return 24;
    case pfBGRA8888:
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

TColorRGB TGraphicsData::GetPixelColor(short x, short y)
{
    static    TColorRGB color(0xFF000000);
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
        switch(x & 3)
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
        switch(x & 1)
        {
        case 0: colorIndex = ((*pp) & 0xF0) >> 4; break;
        case 1: colorIndex = ((*pp) & 0x0F); break;
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

unsigned char TGraphicsData::GetPixelColorIndex(short x, short y)
{
    if ((m_pixelFormat!=pfBGR2ColorsPalette)  &&
        (m_pixelFormat!=pfBGR4ColorsPalette)  &&
        (m_pixelFormat!=pfBGR16ColorsPalette) &&
        (m_pixelFormat!=pfBGR256ColorsPalette))
    {
        return 0;
    }

    unsigned char* pp = ScanLine(y);
    if (pp==NULL) return 0;

    pp+= (x * GetBitsPerPixel()) / 8;

    unsigned char colorIndex = 0;
    switch(m_pixelFormat)
    {
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
        break;

    case pfBGR4ColorsPalette:
        switch(x & 3)
        {
        case 0: colorIndex = ((*pp) & 0xC0) >> 6; break;
        case 1: colorIndex = ((*pp) & 0x30) >> 4; break;
        case 2: colorIndex = ((*pp) & 0x0C) >> 2; break;
        case 3: colorIndex = ((*pp) & 0x03);      break;
        }
        break;

    case pfBGR16ColorsPalette:
        switch(x & 1)
        {
        case 0: colorIndex = ((*pp) & 0xF0) >> 4; break;
        case 1: colorIndex = ((*pp) & 0x0F); break;
        }
        break;

    case pfBGR256ColorsPalette:
        {
            colorIndex = *pp; 
        }
        break;
    }   
    return colorIndex;
}

void TGraphicsData::SetPixelColor(short x, short y, TColorRGB color)
{
    if ((m_pixelFormat==pfBGR2ColorsPalette) ||
        (m_pixelFormat==pfBGR4ColorsPalette) ||
        (m_pixelFormat==pfBGR16ColorsPalette) ||
        (m_pixelFormat==pfBGR256ColorsPalette))
    {
        return;
    }

    unsigned char* pp = ScanLine(y);
    if (pp==NULL) return;

    pp+= (x * GetBitsPerPixel()) / 8;

    switch(m_pixelFormat)
    {
    case pfRGB332:  
        {
            unsigned char b = color.R & 0xE0 + ((color.G & 0xE0)>>3) + (color.B>>6);
            *pp++ = b;
        }
        break;

    case pfRGB555:  
        {
            unsigned char b1 = ((color.R & 0xF8)>>1) + ((color.G & 0xC0)>>6);
            unsigned char b2 = ((color.G & 0x38)<<2) + ((color.B & 0xF8)>>3);
            *pp++ = b1;
            *pp++ = b2;
        }
        break;

    case pfRGB565:
        {
            unsigned char b1 = (color.R  & 0xF8) + ((color.G & 0xE0)>>5);
            unsigned char b2 = ((color.G & 0x1C)<<2) + ((color.B & 0xF8)>>3);
            *pp++ = b1;
            *pp++ = b2;
        }
        break;

    case pfRGB888:	
        {
            *pp++ = color.R;
            *pp++ = color.G;
            *pp++ = color.B;
        }
        break;

    case pfRGBX8888:
    case pfRGBA8888:
        {
            *pp++ = color.R;
            *pp++ = color.G;
            *pp++ = color.B;
            *pp++ = color.A;
        }
        break;

    case pfBGR888:  
        {
            *pp++ = color.B;
            *pp++ = color.G;
            *pp++ = color.R;
        }
        break;

    case pfBGRA8888:
        {
            *pp++ = color.B;
            *pp++ = color.G;
            *pp++ = color.R;
            *pp++ = color.A;
        }
        break;
    }
}

void TGraphicsData::SetPixelColorIndex(short x, short y, unsigned char colorIndex)
{
    if ((m_pixelFormat!=pfBGR2ColorsPalette)  &&
        (m_pixelFormat!=pfBGR4ColorsPalette)  &&
        (m_pixelFormat!=pfBGR16ColorsPalette) &&
        (m_pixelFormat!=pfBGR256ColorsPalette))
    {
        return;
    }
    unsigned char* pp = ScanLine(y);
    if (pp==NULL) return;

    pp+= (x * GetBitsPerPixel()) / 8;

    unsigned char mask = 0;

    switch(m_pixelFormat)
    {
    case pfBGR2ColorsPalette:
        {
            switch(x&7)
            {
            case 0: *pp = (*pp & 0x7F) + ((colorIndex & 1)<<7); break;
            case 1: *pp = (*pp & 0xBF) + ((colorIndex & 1)<<6); break;
            case 2: *pp = (*pp & 0xDF) + ((colorIndex & 1)<<5); break;
            case 3: *pp = (*pp & 0xEF) + ((colorIndex & 1)<<4); break;
            case 4: *pp = (*pp & 0xF7) + ((colorIndex & 1)<<3); break;
            case 5: *pp = (*pp & 0xFB) + ((colorIndex & 1)<<2); break;
            case 6: *pp = (*pp & 0xFD) + ((colorIndex & 1)<<1); break;
            case 7: *pp = (*pp & 0xFE) + ((colorIndex & 1)); break;
            }                
        }
        break;

    case pfBGR4ColorsPalette:
        {
            switch(x&3)
            {
            case 0: *pp = (*pp & 0x3F) + ((colorIndex & 3)<<6); break;
            case 1: *pp = (*pp & 0xCF) + ((colorIndex & 3)<<4); break;
            case 2: *pp = (*pp & 0xF3) + ((colorIndex & 3)<<2); break;
            case 3: *pp = (*pp & 0xFC) + ((colorIndex & 3)); break;
            }                
        }
        break;

    case pfBGR16ColorsPalette:
        {
            switch(x&1)
            {
            case 0: *pp = (*pp & 0x0F) + ((colorIndex & 15)<<4); break;
            case 1: *pp = (*pp & 0xF0) + ((colorIndex & 15)); break;
            }                
        }
        break;

    case pfBGR256ColorsPalette:
        {
            *pp = colorIndex;
        }
        break;
    }
}

unsigned char* TGraphicsData::CreatePalette(unsigned short colorCount)
{
    m_colorCount = colorCount;
    m_colorPaletteSize = m_colorCount * 3;
    m_colorPalette = (unsigned char*)malloc(m_colorPaletteSize);
    if (m_colorPalette==NULL)
    {
        m_colorPaletteSize = 0;
        m_colorCount = 0;
    } else {
        memset(m_colorPalette, 0x80, m_colorPaletteSize);
    }    
    return m_colorPalette;
}

unsigned char*  TGraphicsData::GetPalette()
{
    return m_colorPalette;
}

TColorRGB TGraphicsData::GetPaletteColor(unsigned char colorIndex)
{
    TColorRGB result;
    if (m_colorPalette)
    {
        if (colorIndex<m_colorCount)
        {
            result.B = m_colorPalette[colorIndex*3 + 0];
            result.G = m_colorPalette[colorIndex*3 + 1];
            result.R = m_colorPalette[colorIndex*3 + 2];
        }
    }
    return result;
}

void  TGraphicsData::SetPaletteColor(unsigned char colorIndex, TColorRGB colorValue)
{    
    if (m_colorPalette)
    {
        if (colorIndex<m_colorCount)
        {
            TColorRGB* paletterColor = (TColorRGB*)(m_colorPalette + colorIndex*3);
            paletterColor->B = colorValue.B;
            paletterColor->G = colorValue.G;
            paletterColor->R = colorValue.R;
        }
    }
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

    Clear();

    if (image[0]!='G') return false;
    if (image[1]!='D') return false;

    m_bitmapData			= (unsigned char*)image + (*headerOffsets++);
    m_bitmapDataSize		= totalLength - bitmapOffset; 
    m_bitmapNeedsUnalloc    = false;

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
    m_bytesPerLine          = (short)(m_bitmapDataSize / m_bitmapHeight);

    m_colorPaletteSize = totalLength - headerSize - 18;	
    if (m_colorPaletteSize>0)
    {
        m_colorPalette = (unsigned char*)headerParams;
        m_colorPaletteNeedsUnalloc = false;
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

    Clear();

    FILE* file = fopen(filename, "rb");
    if (file==NULL)
    {
        return false;
    }
    fread(&magicByte1, 1, 1, file);
    fread(&magicByte2, 1, 1, file);
    if (magicByte1!='G')
    {
        fclose(file);
        return false;
    }
    if (magicByte2!='D')
    {
        fclose(file);
        return false;
    }

    bytesRead += (unsigned short)fread(&totalLength, 1, 4, file);
    bytesRead += (unsigned short)fread(&m_transparentColor, 1, 4, file);
    bytesRead += (unsigned short)fread(&bitmapOffset, 1, 4, file);
    bytesRead += (unsigned short)fread(&headerSize, 1, 4, file);
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
            m_colorPaletteNeedsUnalloc = true;
            fread(m_colorPalette, 1, m_colorPaletteSize, file);
            m_colorCount = (unsigned short)(m_colorPaletteSize / 3);
        } else {
            //not enough memory
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
    m_bytesPerLine   = (short)(m_bitmapDataSize/m_bitmapHeight);
    if (m_bitmapData==NULL)
    {
        //not enough memory
        m_bitmapDataSize = 0;
        fclose(file);
        return false;
    }
    m_bitmapNeedsUnalloc = true;

    fread(m_bitmapData, 1, m_bitmapDataSize, file);
    fclose(file);
    return true;
}

bool TGraphicsData::SaveToFile (const char* filename)
{
    unsigned char magicByte1    = 'G';
    unsigned char magicByte2    = 'D';
    unsigned long headerSize    = 14;    
    unsigned long bitmapOffset  = m_colorPaletteSize + headerSize + 18;
    unsigned long totalLength   = bitmapOffset + m_bitmapDataSize;    
    unsigned short flags        = m_transparentColorUsed?0x0100:0 + m_flipImage?0x0200:0;
    unsigned short bitsPerPixel = 0;
    switch(m_pixelFormat)
    {
        case pfNone: bitsPerPixel = 0; break;
        case pfBGR2ColorsPalette: bitsPerPixel = 1; break;
        case pfBGR4ColorsPalette: bitsPerPixel = 2; break;
        case pfBGR16ColorsPalette: bitsPerPixel = 4; break;
        case pfBGR256ColorsPalette: bitsPerPixel = 8; break;
        case pfRGB332: bitsPerPixel = 8; break;
        case pfRGB555: bitsPerPixel = 15; break;
        case pfRGB565: bitsPerPixel = 16; break;
        case pfRGB888: bitsPerPixel = 24; break;
        case pfRGBA8888: bitsPerPixel = 32; break;
        case pfRGBX8888: bitsPerPixel = 32; break;
        case pfBGR888:   bitsPerPixel = 24; break;
        case pfBGRA8888: bitsPerPixel = 32; break;
        case pfDXT1: bitsPerPixel = 2; break;
        case pfDXT3: bitsPerPixel = 4; break;
    }

    if (m_colorPalette==NULL)
    {
        m_colorCount = 0;
        m_colorPaletteSize =0;
    }

    FILE* file = fopen(filename, "wb");
    if (file==NULL)
    {
        return false;
    }
    fwrite(&magicByte1, 1, 1, file);
    fwrite(&magicByte2, 1, 1, file);
    fwrite(&totalLength, 1, 4, file);
    fwrite(&m_transparentColor, 1, 4, file);
    fwrite(&bitmapOffset, 1, 4, file);
    fwrite(&headerSize, 1, 4, file);
    fwrite(&m_bitmapWidth, 1, 2, file);
    fwrite(&m_bitmapHeight, 1, 2, file);
    fwrite(&m_planes, 1, 2, file);
    fwrite(&bitsPerPixel, 1, 2, file);
    fwrite(&flags, 1, 2, file);

    if (m_colorPaletteSize>0)
    {
        if (m_colorPalette)
        {
            fwrite(m_colorPalette, 1, m_colorPaletteSize, file);
        }		
    }
    fwrite(m_bitmapData, 1, m_bitmapDataSize, file);
    fclose(file);
    return true;
}
