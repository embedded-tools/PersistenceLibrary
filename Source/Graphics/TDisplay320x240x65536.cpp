#include "TDisplay320x240x65536.h"
#include <stdlib.h>

TDisplay320x240x65536::TDisplay320x240x65536()
: TGraphicsData(0,0,pfRGB555)
{
    m_bitmapData = (unsigned char*)0x40000;
	m_bitmapNeedsUnalloc = false;
    m_bitmapDataSize = 153600;	
    m_transparentColor = 0;
	m_transparentColorUsed = false;
    m_bitmapWidth = 320;
    m_bitmapHeight = 240;
    m_bytesPerLine = 640;
    m_pixelFormat = pfRGB565;
	m_bytesPerLine = m_bitmapWidth * 2;	
    m_colorPalette = NULL;
    m_colorPaletteSize = 0;
    m_colorPaletteNeedsUnalloc = false;
    m_colorCount = 0;
	m_flipImage = false;
	m_readOnly = false;
}