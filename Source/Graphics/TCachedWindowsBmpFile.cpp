/*
 * Persistence Library / Graphics / TCachedWindowsBmpFile
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

#include "TCachedWindowsBmpFile.h"
#include "TWindowsBmpFile.h"


TCachedWindowsBmpFile::TCachedWindowsBmpFile()
:	TGraphicsData(0,0,pfNone),
	m_bitmapDataOffset(0),
	m_bitmapDataNeedsUnalloc(false),
	m_fileHandle(0),
	m_lastLine(-1)
{

}

TCachedWindowsBmpFile::~TCachedWindowsBmpFile()
{
	CloseFile();
}

bool TCachedWindowsBmpFile::OpenFile(const char* filename, unsigned char* buffer, int bufferSize)
{
	TBitmapHeader header;
	unsigned long bytesRead = 0;
	unsigned char tmp;

	m_fileHandle = fopen(filename, "rb+");
	if (m_fileHandle==NULL)
	{
		return false;
	}

	bytesRead += (unsigned long)fread(&header.usMagicBytes, 1, 2, m_fileHandle);
	if (header.usMagicBytes!=0x4D42)
	{
		fclose(m_fileHandle);
		return false;
	}
	bytesRead += (unsigned long)fread(&header.ulTotalLength, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulReserved,    1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulBitmapDataOffset, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulBitmapHeaderSize, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulWidth, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulHeight, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.usPlanes, 1, 2, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.usBitsPerPixel, 1, 2, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulCompression, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulBitmapDataSize, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulPixelsPerMeterHorizontal, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulPixelsPerMeterVertical, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulNumberOfColors, 1, 4, m_fileHandle);
	bytesRead += (unsigned long)fread(&header.ulNumberOfImportantColors, 1, 4, m_fileHandle);	
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
		fclose(m_fileHandle);
		return false;
	}
	if ((header.ulBitmapDataOffset<0) || (header.ulBitmapHeaderSize<0) || (header.ulWidth<0) || (header.ulWidth>131072) || (header.ulHeight<0) || (header.ulHeight>131072))
	{
		fclose(m_fileHandle);
		return false;
	}
	if (header.ulBitmapDataOffset!=54)
	{
		m_colorPaletteSize = header.ulBitmapDataOffset - (sizeof(header) - 2);
		if ((m_colorPaletteSize>2048) || (m_colorPaletteSize<0))
		{
			fclose(m_fileHandle);
			return false;
		}
		m_colorCount = (unsigned short)(m_colorPaletteSize/4);

		m_colorPalette = (unsigned char*)malloc(m_colorCount*3);
		for(int i = 0; i<m_colorCount; i++)
		{
			fread(m_colorPalette+i*3, 1, 3, m_fileHandle);
			fread(&tmp, 1, 1, m_fileHandle);
		}
	}
	m_bitmapDataOffset = header.ulBitmapDataOffset;
	m_bitmapDataSize = header.ulTotalLength-header.ulBitmapDataOffset;	
	m_bitmapNeedsUnalloc = true;
	m_bitmapWidth = (short)header.ulWidth;
	m_bitmapHeight = (short)header.ulHeight;
	m_bytesPerLine = (short)(m_bitmapDataSize / m_bitmapHeight);
	m_flipImage = true;
	

	m_bitmapDataNeedsUnalloc = false;
	if (buffer==NULL)
	{
		m_bitmapData     = (unsigned char*)malloc(m_bytesPerLine);
		m_bitmapDataSize = m_bytesPerLine;
		m_bitmapDataNeedsUnalloc = true;

	} else {
		if (bufferSize<m_bytesPerLine)
		{
			fclose(m_fileHandle);
			return false;
		}
		m_bitmapData     = buffer;
		m_bitmapDataSize = bufferSize;
	}
	return true;
}

void TCachedWindowsBmpFile::CloseFile()
{
	WriteDataBack();
	m_lastLine = -1;

	if (m_fileHandle)
	{
		fclose(m_fileHandle);
		m_fileHandle = 0;
	}
	if (m_bitmapData)
	{
		if (m_bitmapDataNeedsUnalloc)
		{
			free(m_bitmapData);
			m_bitmapData = NULL;
		}
	}
	if (m_colorPalette)
	{
		if (m_colorPaletteNeedsUnalloc)
		{
			free(m_colorPalette);
			m_colorPalette = NULL;			
		}
	}
	m_bitmapWidth  = 0;
	m_bitmapHeight = 0;
	m_bytesPerLine = 0;
	m_bitmapDataSize = 0;
	m_bitmapNeedsUnalloc = false;
	m_colorPaletteNeedsUnalloc = false;
}

void TCachedWindowsBmpFile::WriteDataBack()
{	
	if (m_lastLine!=-1)
	{
		int offset = m_bitmapDataOffset;	
		if (m_flipImage)
		{	
			offset += (m_bitmapHeight - 1 - m_lastLine)*m_bytesPerLine;
		} else {
			offset += m_lastLine * m_bytesPerLine;
		}
		if (m_bitmapData)
		{
			fseek(m_fileHandle, offset, SEEK_SET);
			fwrite(m_bitmapData, 1, m_bytesPerLine, m_fileHandle);
		}
	}	
}

unsigned char* TCachedWindowsBmpFile::ScanLine(short line)
{
	if (m_lastLine!=line)
	{
		int offset = m_bitmapDataOffset;	
		if (m_lastLine!=-1)
		{
			WriteDataBack();
		}
		
		offset = m_bitmapDataOffset;	
		if (m_flipImage)
		{
			offset += (m_bitmapHeight - 1 - line)*m_bytesPerLine;
		} else {
			offset += line * m_bytesPerLine;
		}
		fseek(m_fileHandle, offset, SEEK_SET);

		long bytesRead = (long)fread(m_bitmapData, 1, m_bytesPerLine, m_fileHandle);
		if (bytesRead<m_bytesPerLine)
		{
			return NULL;
		}
		m_lastLine = line;
	}
	return m_bitmapData;	
}

