/*
 * Persistence Library / Graphics / TPixelFormatConverter
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

#ifndef TPIXELFORMATCONVERTER___H
#define TPIXELFORMATCONVERTER___H

#include "TGraphicsData.h"
#include "TRectangle.h"
#include "TPosition.h"

typedef void (*FormatConverterFunction)(unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);

class TPixelFormatConverter
{
	static FormatConverterFunction GetFormatConverter(ePixelFormat targetFormat, ePixelFormat sourceFormat);

	static unsigned char* srcPalette;

	static void BGRPalette2toRGB565   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void BGRPalette4toRGB565   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void BGRPalette16toRGB565  (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void BGRPalette256toRGB565 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGB565toRGB565        (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGB888toRGB565        (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGBX8888toRGB565      (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGBA8888toRGB565      (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void DXT1toRGB565          (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);

	static void BGRPalette2toRGB888   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void BGRPalette4toRGB888   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void BGRPalette16toRGB888  (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void BGRPalette256toRGB888 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGB565toRGB888        (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGB888toRGB888        (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGBX8888toRGB888      (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void RGBA8888toRGB888      (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);
	static void DXT1toRGB888          (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam);

	bool ConvertBMPtoDXT(TGraphicsData& target, TGraphicsData& source);
public:
	TPixelFormatConverter();
	virtual ~TPixelFormatConverter();

	bool CopyRect(TGraphicsData& target, TPosition targetArea,  TGraphicsData& source);
	bool CopyRect(TGraphicsData& target, TPosition targetArea,  TGraphicsData& source, TRectangle sourceArea);
	bool CopyRect(TGraphicsData& target, TRectangle targetArea, TGraphicsData& source, TRectangle sourceArea);

};

#endif
