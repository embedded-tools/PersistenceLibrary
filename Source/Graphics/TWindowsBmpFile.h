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

#ifndef TWINDOWSBMPFILE___H
#define TWINDOWSBMPFILE___H

#include "TGraphicsData.h"

struct TBitmapHeader
{
    unsigned short usMagicBytes;
    unsigned long  ulTotalLength;
    unsigned long  ulReserved;
    unsigned long  ulBitmapDataOffset;
    unsigned long  ulBitmapHeaderSize;
    unsigned long  ulWidth;
    unsigned long  ulHeight;
    unsigned short usPlanes;
    unsigned short usBitsPerPixel;
	//end of os/2 bitmap header

    unsigned long  ulCompression;
    unsigned long  ulBitmapDataSize;
    unsigned long  ulPixelsPerMeterHorizontal;
    unsigned long  ulPixelsPerMeterVertical;
    unsigned long  ulNumberOfColors;
    unsigned long  ulNumberOfImportantColors;
	//end of windows bmp header

	/*
    unsigned long* pbColorPallete; //not part of bmp header
    unsigned long* pbBitmapData;   //not part of bmp header
	*/

    
};


class TWindowsBmpFile : public TGraphicsData
{  
public:
	TWindowsBmpFile();
    TWindowsBmpFile(short width, short height, ePixelFormat pixelFormat, unsigned long backgroundColor=0); 
    bool LoadFromWindowsBmp(const char* filename);	
    bool SaveToWindowsBmp(const char* filename);

	void SwitchRedAndBlue();

};



#endif
