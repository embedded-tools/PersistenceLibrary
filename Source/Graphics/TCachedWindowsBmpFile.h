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

#ifndef TCACHEDWINDOWSBMPFILE___H
#define TCACHEDWINDOWSBMPFILE___H

#include "TGraphicsData.h"
#include <stdlib.h>
#include <stdio.h>

/**
 *  Opens large windows bitmap. You can operate with large amount of graphics
 *  data even if you don't have enough RAM to load whole file at once.
 */
class TCachedWindowsBmpFile : public TGraphicsData
{
private:
	FILE*          m_fileHandle;
	unsigned long  m_bitmapDataOffset;		
	bool           m_bitmapDataNeedsUnalloc;	
	short          m_lastLine;

protected:
	virtual unsigned char* ScanLine(short line);

	void           WriteDataBack();

public:
	TCachedWindowsBmpFile();
	virtual ~TCachedWindowsBmpFile();

	bool OpenFile(const char* filename, unsigned char* buffer=NULL, int bufferSize=0);
	void CloseFile();

	

};

#endif
