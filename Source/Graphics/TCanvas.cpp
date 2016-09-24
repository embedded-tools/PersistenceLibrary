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

#include "TCanvas.h"

TCanvas::TCanvas(TGraphicsData vram)
{

}

TCanvas::~TCanvas()
{

}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
void TCanvas::DrawCircle(TPosition center, short diameter)
{

}

void TCanvas::DrawGradient(TRectangle area, TColorRGB topLeftColor, TColorRGB topRightColor, TColorRGB bottomLeftColor, TColorRGB bottomRightColor)
{

}

void TCanvas::DrawLine(TPosition startPosition, TPosition endPosition)
{

}

void TCanvas::DrawPolyLine(TPosition* coordinates, short count)
{

}

void TCanvas::DrawRectangle(TRectangle rectangle)
{

}

void TCanvas::DrawRoundedRectangle(TRectangle rectangle, short radius)
{

}

void TCanvas::DrawText(TFont font, const char* text, TPosition position)
{

}

void TCanvas::DrawText(TFont font, const char* text, TRectangle area, TAlign horizontalAlign, TVerticalAlign verticalAlign)
{

}

void TCanvas::DrawTextVertical(TFont font, const char* text, TPosition position)
{

}

void TCanvas::DrawTextVertical(TFont font, const char* text, TRectangle area, TAlign horizontalAlign, TVerticalAlign verticalAlign)
{

}

void TCanvas::CopyRectangle(TPosition targetPosition, TGraphicsData sourceData, TRectangle sourceRectangle, unsigned char alpha)
{

}

void TCanvas::CopyMaskedRectangle(TPosition targetPosition, TGraphicsData sourceData, TRectangle sourceRectangle)
{

}

void TCanvas::CopyScaledRectangle(TRectangle targetArea, TGraphicsData sourceData, TRectangle sourceRectangle)
{

}

void TCanvas::SetBrushColor(TColorRGB brushColor)
{

}

TColorRGB TCanvas::GetBrushColor()
{
    static TColorRGB result(0);
	return result;
}

void TCanvas::ApplyFilter(TFilter3x3 filter)
{

}

void TCanvas::ApplyFilter(TFilter5x5 filter)
{

}

void TCanvas::ApplyFilter(TFilter7x7 filter)
{

}