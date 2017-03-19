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
#include "TFixedPoint16M.h"
#include "TPosition.h"
#include "TSize.h"
#include "TRectangle.h"

TCanvas::TCanvas(TGraphicsData* vram)
{
    m_graphicsData = vram;
    m_backgroundColor = 0xFFFFFF;
    m_foregroundColor = 0x000000;
}

TCanvas::~TCanvas()
{
     m_graphicsData = NULL;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
void TCanvas::DrawElipse(TRectangle area)
{
	short i,j;
	short width;
	short height;
	TFixedPoint16M centerX;
	TFixedPoint16M centerY;
    TFixedPoint16M x1, x2;
    TFixedPoint16M y;

	width   = area.Right  - area.Left;
	height  = area.Bottom - area.Top;
	centerX = width;
    centerX -= 1;
    centerX /= 2;
	centerY = height;
    centerY -= 1;
    centerY /= 2;

    y = area.Top;
	for(i = 0; i<height; i++)
	{
        x1 = area.Left;
        x2 = area.Right-area.Left-1;
        for(j = (short)x1.Round(); j<(short)x2.Round(); j++)                         
        {
		    SetPixelColor(j, area.Top + i, m_foregroundColor);
        }
        y+=1;
	}	
}

void TCanvas::DrawGradient(TRectangle area, TColorRGB topLeftColor, TColorRGB topRightColor, TColorRGB bottomLeftColor, TColorRGB bottomRightColor)
{
    short i,j;
    for(i = area.Top; i<area.Bottom; i++)
    {
        for(j = area.Left; j<area.Right; j++)
        {


        }
    }        
}

void TCanvas::DrawLine(TPosition startPosition, TPosition endPosition)
{

}

void TCanvas::DrawPolyLine(TPosition* coordinates, short count)
{

}

void TCanvas::DrawRectangle(TRectangle rectangle)
{
    short x,y;
    for(y = rectangle.Top; y<rectangle.Bottom; y++)
    {
        for(x = rectangle.Left; x<rectangle.Right; x++)
        {
            if (m_graphicsData)
            {
                m_graphicsData->SetPixelColor(x, y, m_foregroundColor);
            }
        }
    }        
}

void TCanvas::DrawRoundedRectangle(TRectangle rectangle, short radius)
{

}

void TCanvas::DrawCharacter (TFontCharacter character, TPosition position)
{
    if (!m_graphicsData) return;

    TColorRGB srcColor,dstColor; 
    short x,y,i1,i2;

    TSize charSize = character.GetSize();
    short canvasWidth  = m_graphicsData->GetWidth();
    short canvasHeight = m_graphicsData->GetWidth();

    for(y = 0; y<charSize.Height; y++)
    {
        if (position.Y>=canvasHeight) break;

        for(x = 0; x<charSize.Width; x++)
        {
            if (position.X+x>=canvasWidth) continue;

            srcColor = character.GetPixelColor(x, y);
            

            i1 = (srcColor.R * 28 + srcColor.G * 57 + srcColor.B * 15)/100;               
            i2 = 255-i1;

            dstColor.R = (m_backgroundColor.R * i1 + m_foregroundColor.R * i2)>>8;
            dstColor.G = (m_backgroundColor.G * i1 + m_foregroundColor.G * i2)>>8;
            dstColor.B = (m_backgroundColor.B * i1 + m_foregroundColor.B * i2)>>8;

            m_graphicsData->SetPixelColor(position.X + x, position.Y+y, dstColor);               
        }
    }             
}

void TCanvas::DrawText(TFont font, const char* text, TPosition position)
{
    unsigned char* currentText = (unsigned char*)text;
    TPosition      currentPosition;
    TFontCharacter charData;     
    TSize          charSize;

    currentPosition = position;
    while(*currentText>0)
    {
       font.GetCharacter(*currentText, charData );
       DrawCharacter(charData, position);

       charSize = charData.GetSize();
       position.X += charSize.Width;
       currentText++;       
    }
}

void TCanvas::DrawText(TFont font, const char* text, TRectangle area, TAlign horizontalAlign, TVerticalAlign verticalAlign)
{
    unsigned char* currentText = (unsigned char*)text;
    TFontCharacter charData;
    TSize          charSize;
    TSize          textSize;
    TPosition      alignedPos;

    textSize.Width = 0;
    textSize.Height = 0;

    while(*currentText>0)
    {
        font.GetCharacter(*currentText, charData );        
        charSize = charData.GetSize();
        currentText++;

        textSize.Width  += charSize.Width;
        textSize.Height = charSize.Height;
    }
    switch(horizontalAlign)
    {
        case taLeft:    alignedPos.X  = area.Left; break; 
        case taCenter:  alignedPos.X  = area.Left + (area.Width() - textSize.Width)/2; break;
        case taRight:   alignedPos.X  = area.Left + (area.Width() - textSize.Width); break;
    }
    switch(horizontalAlign)
    {
        case tvaTop:    alignedPos.Y   = area.Top; break;
        case tvaCenter: alignedPos.Y   = area.Top + (area.Height()-textSize.Height)/2; break;
        case tvaBottom: alignedPos.Y   = area.Top + (area.Height()-textSize.Height); break; 
    }   

    DrawText(font, text, alignedPos);
    
}

void TCanvas::DrawCharacterVertical (TFontCharacter character, TPosition position, bool directionUp)
{
    if (!m_graphicsData) return;

    TColorRGB srcColor,dstColor; 
    short x,y,i1,i2;

    TSize charSize = character.GetSize();
    short canvasWidth  = m_graphicsData->GetWidth();
    short canvasHeight = m_graphicsData->GetWidth();

    for(y = 0; y<charSize.Height; y++)
    {
        if (position.Y>=canvasHeight) break;

        for(x = 0; x<charSize.Width; x++)
        {
            if (position.X+x>=canvasWidth) continue;

            srcColor = character.GetPixelColor(x, y);
            i1 = (srcColor.R * 28 + srcColor.G * 57 + srcColor.B * 15)/100;               
            i2 = 255-i1;

            dstColor.R = (m_backgroundColor.R * i1 + m_foregroundColor.R * i2)>>8;
            dstColor.G = (m_backgroundColor.G * i1 + m_foregroundColor.G * i2)>>8;
            dstColor.B = (m_backgroundColor.B * i1 + m_foregroundColor.B * i2)>>8;

            if (directionUp)
            {
                m_graphicsData->SetPixelColor(position.X + y, position.Y-x, dstColor);                    
            } else {
                m_graphicsData->SetPixelColor(position.X + charSize.Height-y, position.Y+x, dstColor);                    
            }  
        }
    }             
}

void TCanvas::DrawTextVertical(TFont font, const char* text, TPosition position, bool directionUp)
{
    unsigned char* currentText = (unsigned char*)text;
    TPosition      currentPosition;
    TFontCharacter charData;     
    TSize          charSize;

    currentPosition = position;
    while(*currentText>0)
    {
        font.GetCharacter(*currentText, charData );
        DrawCharacterVertical(charData, position, directionUp);

        charSize = charData.GetSize();
        if (directionUp)
        {
            position.Y -= charSize.Width;
        } else {
            position.Y += charSize.Width;
        }
        currentText++;       
    }     
}

void TCanvas::DrawTextVertical(TFont font, const char* text, TRectangle area, TAlign horizontalAlign, TVerticalAlign verticalAlign, bool directionUp)
{
    unsigned char* currentText = (unsigned char*)text;
    TFontCharacter charData;
    TSize          charSize;
    TSize          textSize;
    TPosition      alignedPos;

    textSize.Width = 0;
    textSize.Height = 0;

    while(*currentText>0)
    {
        font.GetCharacter(*currentText, charData );        
        charSize = charData.GetSize();
        currentText++;

        textSize.Width  = charSize.Height;
        textSize.Height += charSize.Width;
    }
    switch(horizontalAlign)
    {
        case taLeft:    alignedPos.X = area.Left; break; 
        case taCenter:  alignedPos.X = area.Left + (area.Width() - textSize.Width)/2; break;
        case taRight:   alignedPos.X = area.Left + (area.Width() - textSize.Width); break;
    }
    switch(horizontalAlign)
    {
        case tvaTop:    alignedPos.Y = area.Top; break;
        case tvaCenter: alignedPos.Y = area.Top + (area.Height()-textSize.Height)/2; break;
        case tvaBottom: alignedPos.Y = area.Top + (area.Height()-textSize.Height); break; 
    }   


    DrawTextVertical(font, text, alignedPos, directionUp);
}

void TCanvas::CopyRectangle(TPosition targetPosition, TGraphicsData* sourceData, TRectangle sourceRectangle, unsigned char alpha)
{
    short x,y;
    TColorRGB color;
    if (!m_graphicsData) return;

    for(y = sourceRectangle.Top; y<sourceRectangle.Bottom; y++)
    {
        for(x = 0; x<sourceRectangle.Right-sourceRectangle.Left; x++)
        {
            color = sourceData->GetPixelColor(x,y);
            m_graphicsData->SetPixelColor(x + targetPosition.X, y + targetPosition.Y, color);
        }
    }   
}

void TCanvas::CopyMaskedRectangle(TPosition targetPosition, TGraphicsData* sourceData, TRectangle sourceRectangle, TColorRGB maskColor)
{
    short x,y;
    TColorRGB color;
    if (!m_graphicsData) return;

    for(y = sourceRectangle.Top; y<sourceRectangle.Bottom; y++)
    {
        for(x = 0; x<sourceRectangle.Right-sourceRectangle.Left; x++)
        {
            color = sourceData->GetPixelColor(x,y);
            if (color==maskColor) continue;

            m_graphicsData->SetPixelColor(x + targetPosition.X, 
                                     y + targetPosition.Y,
                                     m_foregroundColor);
        }
    }   
}

void TCanvas::CopyScaledRectangle(TRectangle targetArea, TGraphicsData* sourceData, TRectangle sourceRectangle)
{

}

void TCanvas::SetForegroundColor(TColorRGB foregroundColor)
{
    m_foregroundColor = foregroundColor;
}

void TCanvas::SetBackgroundColor(TColorRGB backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

void TCanvas::SetPixelColor(short x, short y, TColorRGB color)
{    
    if (m_graphicsData)
    {
        m_graphicsData->SetPixelColor(x, y, color);
    }
}

void TCanvas::SetPixelColorIndex(short x, short y, unsigned char colorIndex)
{    
    if (m_graphicsData)
    {
        m_graphicsData->SetPixelColor(x, y, colorIndex);
    }
}

TColorRGB TCanvas::GetForegroundColor()
{
    return m_foregroundColor;   
}

TColorRGB TCanvas::GetBackgroundColor()
{
    return m_backgroundColor;
}

TColorRGB TCanvas::GetPixelColor(short x, short y)
{
    if (m_graphicsData)
    {
        return m_graphicsData->GetPixelColor(x, y);
    }
    static TColorRGB color(0xFF000000);
    color = 0;
    return color;
}

unsigned char TCanvas::GetPixelColorIndex(short x, short y)
{
    if (m_graphicsData)
    {
        return m_graphicsData->GetPixelColorIndex(x, y);
    }
    return 0;
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