/*
 * Persistence Library / Graphics / TCanvas
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

#ifndef TCANVAS___H
#define TCANVAS___H

#include "TAlign.h"
#include "TColorRGB.h"
#include "TFont.h"
#include "TGraphicsData.h"
#include "TPosition.h"
#include "TRectangle.h"
#include "TVerticalAlign.h"
#include "TFilter3x3.h"
#include "TFilter5x5.h"
#include "TFilter7x7.h"
#include "TColorRGB.h"

class TCanvas
{
private:
    TGraphicsData* m_graphicsData;
    TColorRGB      m_backgroundColor;
    TColorRGB      m_foregroundColor;

public:
    TCanvas(TGraphicsData* vram);
	virtual ~TCanvas();
	
	void DrawElipse(TRectangle area);
	void DrawGradient(TRectangle area, TColorRGB topLeftColor, TColorRGB topRightColor, TColorRGB bottomLeftColor, TColorRGB bottomRightColor);
	void DrawLine(TPosition startPosition, TPosition endPosition);
	void DrawPolyLine(TPosition* coordinates, short count);
	void DrawRectangle(TRectangle rectangle);
	void DrawRoundedRectangle(TRectangle rectangle, short radius);
    void DrawCharacter (TFontCharacter character, TPosition position);
    void DrawCharacterVertical (TFontCharacter character, TPosition position, bool directionUp);
	void DrawText(TFont font, const char* text, TPosition position);
	void DrawText(TFont font, const char* text, TRectangle area, TAlign horizontalAlign, TVerticalAlign verticalAlign);
	void DrawTextVertical(TFont font, const char* text, TPosition position, bool directionUp=false);
	void DrawTextVertical(TFont font, const char* text, TRectangle area, TAlign horizontalAlign, TVerticalAlign verticalAlign, bool directionUp=false);
	void CopyRectangle(TPosition targetPosition, TGraphicsData* sourceData, TRectangle sourceRectangle, unsigned char alpha);
	void CopyMaskedRectangle(TPosition targetPosition, TGraphicsData* sourceData, TRectangle sourceRectangle, TColorRGB maskColor);
	void CopyScaledRectangle(TRectangle targetArea, TGraphicsData* sourceData, TRectangle sourceRectangle);
    void SetForegroundColor(TColorRGB foregroundColor);
	void SetBackgroundColor(TColorRGB backgroundColor);
    void SetPixelColor(short x, short y, TColorRGB color);
    void SetPixelColorIndex(short x, short y, unsigned char colorIndex);

	TColorRGB GetForegroundColor();
    TColorRGB GetBackgroundColor();
    TColorRGB GetPixelColor(short x, short y);
    unsigned char GetPixelColorIndex(short x, short y);

	void ApplyFilter(TFilter3x3 filter);
	void ApplyFilter(TFilter5x5 filter);
	void ApplyFilter(TFilter7x7 filter);

private:
	TGraphicsData* m_vram;

};

#endif
