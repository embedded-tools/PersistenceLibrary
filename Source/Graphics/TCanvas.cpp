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
#include "TVector.h"
#include "TEndlessLine.h"
#include "TPosition.h"

TCanvas::TCanvas(TGraphicsData* vram)
{
    m_graphicsData = vram;
    m_backgroundColor = 0xFFFFFF;
    m_foregroundColor = 0x000000;
    m_blending = false;
}

TCanvas::~TCanvas()
{
     m_graphicsData = NULL;
}

int TCanvas::FastSqrt(int x)
{
	if (x<1)
	{
		return 0;
	}
    int y0 = -2147483647;
    int y1 = 1;
    int timeout = 0;
    while(y1!=y0)
    {
        y0 = y1;
        y1 = (x / y0 + y0) / 2;
        timeout++;
        if (timeout==20) break;        
    }
    return y1;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
void TCanvas::DrawElipse(TRectangle area, unsigned char antialiasingFactor)
{
	short cX, cY;
    short ec1X, ec1Y, ec2X, ec2Y;
    int   e,d,d2,div;
	short width   = area.Right  - area.Left;
	short height  = area.Bottom - area.Top;
    short a = width / 2;
    short b = height / 2;

    cX = width / 2;
    cY = height / 2;
    if (width>height)
    {
        e = (width*width)/4-(height*height)/4;
        e = FastSqrt(e);
        ec1X = cX - e;
        ec1Y = cY;
        ec2X = cX + e;
        ec2Y = cY;
    } else {
        e = (height*height)/4 - (width*width)/4;
        e = FastSqrt(e);
        ec1X = cX;
        ec1Y = cY - e;
        ec2X = cX;
        ec2Y = cY + e;
    }
    div = 1;
	if ((a*b)>40000) div = 100;
	if ((a*b)>250000) div = 10000;

    d   = a * a * b /div * b;
    d2  = (a-antialiasingFactor)*(a-antialiasingFactor)*(b-antialiasingFactor)/div*(b-antialiasingFactor);

    int dx, dy, delta, deltaY;
    int colorThresholdTable[15];
    colorThresholdTable[0]  = d;
    colorThresholdTable[14] = d2;
    delta = colorThresholdTable[14] - colorThresholdTable[0];
    for(short i = 1; i<14; i++)
    {
        colorThresholdTable[i] = delta * i / 15 + colorThresholdTable[0];
    }
    unsigned char colorIndex;
    TColorRGB     colorTable[16];
    TColorRGB     colorRGB, colorVector, srcColor, dstColor;

    if (!m_blending)
    {
        colorTable[0]  = m_backgroundColor;
        colorTable[15] = m_foregroundColor;
        colorVector.R  = m_foregroundColor.R - m_backgroundColor.R;
        colorVector.G  = m_foregroundColor.G - m_backgroundColor.G;
        colorVector.B  = m_foregroundColor.B - m_backgroundColor.B;
        for(short i = 1; i<15; i++)
        {
            colorTable[i].R = colorVector.R * i / 15 + colorTable[0].R;
            colorTable[i].G = colorVector.G * i / 15 + colorTable[0].G;
            colorTable[i].B = colorVector.B * i / 15 + colorTable[0].B;
        }
    }

	for(short y = 0; y<height; y++)
	{
        deltaY = 0;
        dy = (cY - y); dy*=dy; dy *= a; dy /= div; dy *= a; deltaY+=dy;        
        for(short x = 0; x<width; x++)
        {
            delta = 0;
            dx = (cX - x); dx*=dx; dx *= b; dx /= div; dx *= b; delta += dx;           
            delta += deltaY;
            colorIndex = 0;
            if (delta > colorThresholdTable[0])
            {
                //background color
                if (!m_blending) continue;
            } else
            if (delta < colorThresholdTable[15])
            {
                //foreground color
                colorIndex = 15;
            } else {
                for(short i = 1; i<15; i++)
                {
                    if (delta < colorThresholdTable[i-1])
                    {
                        colorIndex = (unsigned char)i;                            
                    }
                }
            }
            if (m_blending)
            {
                srcColor = GetPixelColor(x, y);
                dstColor.B = (m_foregroundColor.B * colorIndex) + (srcColor.B * (15 - colorIndex));
                dstColor.G = (m_foregroundColor.G * colorIndex) + (srcColor.G * (15 - colorIndex));
                dstColor.R = (m_foregroundColor.R * colorIndex) + (srcColor.R * (15 - colorIndex));                
                SetPixelColor(x, y, dstColor);
            } else {
                SetPixelColor(x, y, colorTable[colorIndex]);
            }            
        }
	}	
}

void TCanvas::DrawGradient(TRectangle area, TColorRGB topLeftColor, TColorRGB topRightColor, TColorRGB bottomLeftColor, TColorRGB bottomRightColor)
{
    short width = area.Width();
    short height = area.Height();

    TColorRGB colorVectorY1, colorVectorY2, colorVectorX;
    TColorRGB colorLeft, colorRight, color;

    colorVectorY1.R = bottomLeftColor.R - topLeftColor.R;
    colorVectorY1.G = bottomLeftColor.G - topLeftColor.G;
    colorVectorY1.B = bottomLeftColor.B - topLeftColor.B;
    colorVectorY2.R = bottomRightColor.R - topRightColor.R;
    colorVectorY2.G = bottomRightColor.G - topRightColor.G;
    colorVectorY2.B = bottomRightColor.B - topRightColor.B;
    for(short y = 0; y<height; y++)
    {
        colorLeft.R = (unsigned char)((colorVectorY1.R * y)/height) + (topLeftColor.R);
        colorLeft.G = (unsigned char)((colorVectorY1.G * y)/height) + (topLeftColor.G);
        colorLeft.B = (unsigned char)((colorVectorY1.B * y)/height) + (topLeftColor.B);
        colorRight.R = (unsigned char)((colorVectorY1.R * y)/height) + (topRightColor.R);
        colorRight.G = (unsigned char)((colorVectorY1.G * y)/height) + (topRightColor.G);
        colorRight.B = (unsigned char)((colorVectorY1.B * y)/height) + (topRightColor.B);
        colorVectorX.R = colorRight.R - colorLeft.R;
        colorVectorX.G = colorRight.G - colorLeft.G;
        colorVectorX.B = colorRight.B - colorLeft.B;
        for(short x = 0; x<width; x++)
        {            
            color.R = (unsigned char)(colorVectorX.R * x / width) + colorLeft.R;
            color.G = (unsigned char)(colorVectorX.G * x / width) + colorLeft.G;
            color.B = (unsigned char)(colorVectorX.B * x / width) + colorLeft.B;
            SetPixelColor(area.Left + x, area.Top + y, color);
        }
    }        
}

void TCanvas::DrawLine(TPosition startPosition, TPosition endPosition, unsigned char startWidth, unsigned char endWidth, unsigned char antialiasFactor)
{
	DrawLine(startPosition, endPosition, startWidth, endWidth, antialiasFactor, NULL, NULL);
}

void TCanvas::DrawLine(TPosition startPosition, TPosition endPosition, unsigned char startWidth, unsigned char endWidth, unsigned char antialiasFactor, TPosition* previousPoint, TPosition* nextPoint)
{
	TVector vector = endPosition - startPosition;
	vector.Normalize();
	
	TVector normalVector;
	normalVector.DirectionX =  vector.DirectionY;
	normalVector.DirectionY = -vector.DirectionX;
	normalVector.Normalize();

	TPosition tmpPoint11 = startPosition + (normalVector * (startWidth/2));
	TPosition tmpPoint12 = endPosition   + (normalVector * (endWidth/2  ));
	TPosition tmpPoint21 = tmpPoint11    - (normalVector *  startWidth   );	
	TPosition tmpPoint22 = tmpPoint12    - (normalVector *  endWidth );

	TVector normalVectorStart;
	if (previousPoint)
	{
		TVector previousVector = startPosition - (*previousPoint);
		normalVectorStart.DirectionX =  previousVector.DirectionY;
		normalVectorStart.DirectionY = -previousVector.DirectionX;
		normalVectorStart.Normalize();
		normalVectorStart.DirectionX += normalVector.DirectionX;
		normalVectorStart.DirectionY += normalVector.DirectionY;
	} else {
		normalVectorStart.DirectionX = normalVector.DirectionX;
		normalVectorStart.DirectionY = normalVector.DirectionY;
	}	
	normalVectorStart.Simplify();

	TVector normalVectorEnd;
	if (nextPoint)
	{
		TVector nextVector;
		nextVector = (*nextPoint) - endPosition;
		normalVectorEnd.DirectionX =  nextVector.DirectionY;
		normalVectorEnd.DirectionY = -nextVector.DirectionX;
		normalVectorEnd.Normalize();
		normalVectorEnd.DirectionX += normalVector.DirectionX;
		normalVectorEnd.DirectionY += normalVector.DirectionY;
	} else {
		normalVectorEnd.DirectionX = normalVector.DirectionX;
		normalVectorEnd.DirectionY = normalVector.DirectionY;
	}
	normalVectorEnd.Simplify();
	normalVector.Simplify();


	TPosition centerPosition;
	centerPosition.X = (startPosition.X + endPosition.X) / 2;
	centerPosition.Y = (startPosition.Y + endPosition.Y) / 2;

	TEndlessLine leftLine (tmpPoint11, tmpPoint12);
	TEndlessLine rightLine(tmpPoint21, tmpPoint22);	
	TEndlessLine startLine(startPosition, normalVectorStart);	
	TEndlessLine endLine  (endPosition, normalVectorEnd);

	TPosition startLeft  = startLine.Intersection(leftLine);
	TPosition startRight = startLine.Intersection(rightLine);
	TPosition endLeft    = endLine.Intersection(leftLine);
	TPosition endRight   = endLine.Intersection(rightLine);

	long minY =  2147483647;
	long maxY = -2147483647;
	long minX =  2147483647;
	long maxX = -2147483647;

	if (startLeft.Y<minY)  minY = startLeft.Y;
	if (startRight.Y<minY) minY = startRight.Y;
	if (endLeft.Y<minY)    minY = endLeft.Y;
	if (endRight.Y<minY)   minY = endRight.Y;
	if (startLeft.Y>maxY)  maxY = startLeft.Y;
	if (startRight.Y>maxY) maxY = startRight.Y;
	if (endLeft.Y>maxY)    maxY = endLeft.Y;
	if (endRight.Y>maxY)   maxY = endRight.Y;

	if (startLeft.X<minX)  minX = startLeft.X;
	if (startRight.X<minX) minX = startRight.X;
	if (endLeft.X<minX)    minX = endLeft.X;
	if (endRight.X<minX)   minX = endRight.X;
	if (startLeft.X>maxX)  maxX = startLeft.X;
	if (startRight.X>maxX) maxX = startRight.X;
	if (endLeft.X>maxX)    maxX = endLeft.X;
	if (endRight.X>maxX)   maxX = endRight.X;

	leftLine.Normalize();
	rightLine.Normalize();
	startLine.Normalize();
	endLine.Normalize();

	if (leftLine.Distance(centerPosition)<0)
	{
		leftLine.A  = -leftLine.A;  leftLine.B  = -leftLine.B;  leftLine.C  = -leftLine.C;
	}
	if (rightLine.Distance(centerPosition)<0)
	{
		rightLine.A  = -rightLine.A;  rightLine.B  = -rightLine.B;  rightLine.C  = -rightLine.C;
	}
	if (startLine.Distance(centerPosition)<0)
	{
		startLine.A  = -startLine.A;  startLine.B  = -startLine.B;  startLine.C  = -startLine.C;
	}
	if (endLine.Distance(centerPosition)<0)
	{
		endLine.A  = -endLine.A;  endLine.B  = -endLine.B;  endLine.C  = -endLine.C;
	}

	unsigned char colorIndex;
	TColorRGB     colorTable[16];
	TColorRGB     colorRGB, colorVector, srcColor, dstColor;

	if (!m_blending)
	{
		colorTable[0]  = m_backgroundColor;
		colorTable[15] = m_foregroundColor;
		colorVector.R  = m_foregroundColor.R - m_backgroundColor.R;
		colorVector.G  = m_foregroundColor.G - m_backgroundColor.G;
		colorVector.B  = m_foregroundColor.B - m_backgroundColor.B;
		for(short i = 1; i<15; i++)
		{
			colorTable[i].R = colorVector.R * i / 15 + colorTable[0].R;
			colorTable[i].G = colorVector.G * i / 15 + colorTable[0].G;
			colorTable[i].B = colorVector.B * i / 15 + colorTable[0].B;
		}
	}

	TPosition p1, p2, pnt;
	int dist1, dist2, dist3, dist4, minDist;

	if ((maxY-minY)>(maxX-minX))
	{
		for(short y = (short)(minY-1); y<(short)(maxY+1); y++)
		{
			TEndlessLine horzAxis(TPosition(0, y), TPosition(1, y));
			TPosition p1 = horzAxis.Intersection(leftLine);
			TPosition p2 = horzAxis.Intersection(rightLine);

			minX =  2147483647;
			maxX = -2147483647;

			if (p1.X<minX) minX = p1.X;
			if (p2.X<minX) minX = p2.X;
			if (p1.X>maxX) maxX = p1.X;
			if (p2.X>maxX) maxX = p2.X;

			for(short x = (short)(minX-1); x<(short)(maxX+1); x++)
			{	
				pnt.X = x;
				pnt.Y = y;

				dist1 = leftLine.Distance(pnt); 
				if (dist1<0) continue;

				dist2 = rightLine.Distance(pnt);	
				if (dist2<0) continue;

				dist3 = startLine.Distance(pnt);				
				if (previousPoint)
				{
					//dist3 += UNIFIED_VECTOR_SIZE;
					if (dist3<0) continue;
					dist3 = 2147483647;
				} else {
					if (dist3<0) continue;
				}

				dist4 = endLine.Distance(pnt);
				if (nextPoint)
				{
					//dist4 += UNIFIED_VECTOR_SIZE;
					if (dist4<0) continue;
					dist4 = 2147483647;
				} else {
					if (dist4<0) continue;
				}

				minDist = 2147483647;
				if (dist1<minDist) minDist = dist1;
				if (dist2<minDist) minDist = dist2;
				if (dist3<minDist) minDist = dist3;
				if (dist4<minDist) minDist = dist4;

				if (antialiasFactor==0)
				{
					colorIndex = 15;
				} else {
					colorIndex = minDist / (UNIFIED_VECTOR_SIZE * antialiasFactor / 16);
					if (colorIndex>15) colorIndex = 15;
				}						

				if (m_blending)
				{
					srcColor = GetPixelColor(x, y);
					dstColor.B = (m_foregroundColor.B * colorIndex) + (srcColor.B * (15 - colorIndex));
					dstColor.G = (m_foregroundColor.G * colorIndex) + (srcColor.G * (15 - colorIndex));
					dstColor.R = (m_foregroundColor.R * colorIndex) + (srcColor.R * (15 - colorIndex));                
					SetPixelColor(x, y, dstColor);
				} else {
					SetPixelColor(x, y, colorTable[colorIndex]);
				}  
			}
		}
	} else {
		for(short x = (short)(minX-1); x<(short)(maxX+1); x++)
		{
			TEndlessLine vertAxis(TPosition(x, 0), TPosition(x, 1));
			TPosition p1 = vertAxis.Intersection(leftLine);
			TPosition p2 = vertAxis.Intersection(rightLine);

			minY =  2147483647;
			maxY = -2147483647;

			if (p1.Y<minY) minY = p1.Y;
			if (p2.Y<minY) minY = p2.Y;
			if (p1.Y>maxY) maxY = p1.Y;
			if (p2.Y>maxY) maxY = p2.Y;

			for(short y = (short)(minY-1); y<(short)(maxY+1); y++)
			{	
				pnt.X = x;
				pnt.Y = y;

				dist1 = leftLine.Distance(pnt); 
				if (dist1<0) 
				{
					continue;
				} else
				{
					dist2 = rightLine.Distance(pnt);	
					if (dist2<0) continue;
				}
				dist3 = startLine.Distance(pnt);				
				if (previousPoint)
				{
					//dist3 += UNIFIED_VECTOR_SIZE;
					if (dist3<0) continue;
					dist3 = 2147483647;
				} else {
					if (dist3<0) continue;
				}

				dist4 = endLine.Distance(pnt);
				if (nextPoint)
				{
					//dist4 += UNIFIED_VECTOR_SIZE;
					if (dist4<0) continue;
					dist4 = 2147483647;
				} else {
					if (dist4<0) continue;
				}

				minDist = 2147483647;
				if (dist1<minDist) minDist = dist1;
				if (dist2<minDist) minDist = dist2;
				if (dist3<minDist) minDist = dist3;
				if (dist4<minDist) minDist = dist4;

				if (antialiasFactor==0)
				{
					colorIndex = 15;
				} else {
					colorIndex = minDist / (UNIFIED_VECTOR_SIZE * antialiasFactor / 16);
					if (colorIndex>15) colorIndex = 15;
				}						

				if (m_blending)
				{
					srcColor = GetPixelColor(x, y);
					dstColor.B = (m_foregroundColor.B * colorIndex) + (srcColor.B * (15 - colorIndex));
					dstColor.G = (m_foregroundColor.G * colorIndex) + (srcColor.G * (15 - colorIndex));
					dstColor.R = (m_foregroundColor.R * colorIndex) + (srcColor.R * (15 - colorIndex));                
					SetPixelColor(x, y, dstColor);
				} else {
					SetPixelColor(x, y, colorTable[colorIndex]);
				}  
			}
		}
	}
}

void TCanvas::DrawPolyLine(TPosition* positionArray, short positionArrayLength, unsigned char width, unsigned char antialiasFactor)
{
	TPosition* p0 = NULL;
	TPosition  p1,p2;
	TPosition* p3 = NULL;

	int n = 0;
	while(n<positionArrayLength-1)
	{
		if (n>0) p0 = &positionArray[n-1];
		else p0 = NULL;

		p1 = positionArray[n];
		p2 = positionArray[n+1];
		if (n<positionArrayLength-2) p3 = &positionArray[n+2];
		else p3 = NULL;

		DrawLine(p1, p2, width, width, antialiasFactor, p0, p3);
	}
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

void TCanvas::DrawRoundedRectangle(TRectangle rectangle, short radius, unsigned char antialiasFactor)
{
	short width  = rectangle.Width();
	short height = rectangle.Height();	

	TEndlessLine topLine    ( TPosition(rectangle.Left + radius, rectangle.Top + radius), 
		                      TPosition(rectangle.Right- radius, rectangle.Top + radius)
							);
	TEndlessLine leftLine   ( TPosition(rectangle.Left + radius, rectangle.Top), 
		                      TPosition(rectangle.Left + radius, rectangle.Bottom)
							);
	TEndlessLine rightLine  ( TPosition(rectangle.Right - radius, rectangle.Top), 
							  TPosition(rectangle.Right - radius, rectangle.Bottom)
							);
	TEndlessLine bottomLine ( TPosition(rectangle.Left + radius, rectangle.Bottom - radius), 
							  TPosition(rectangle.Right- radius, rectangle.Bottom - radius)
		                    );

	TPosition centerPoint;
	centerPoint.X  = (rectangle.Left + rectangle.Right)  / 2;
	centerPoint.Y  = (rectangle.Top  + rectangle.Bottom) / 2;

	TPosition topLeft     = topLine.Intersection(leftLine);
	TPosition topRight    = topLine.Intersection(rightLine);
	TPosition bottomLeft  = bottomLine.Intersection(leftLine);
	TPosition bottomRight = bottomLine.Intersection(rightLine);

	topLine.Normalize();
	leftLine.Normalize();
	rightLine.Normalize();
	bottomLine.Normalize();

	if (topLine.Distance(centerPoint)>0)
	{
		topLine.A = -topLine.A; topLine.B = -topLine.B; topLine.C = -topLine.C;
	}
	if (leftLine.Distance(centerPoint)>0)
	{
		leftLine.A = -leftLine.A; leftLine.B = -leftLine.B; leftLine.C = -leftLine.C;
	}
	if (bottomLine.Distance(centerPoint)>0)
	{
		bottomLine.A = -bottomLine.A; bottomLine.B = -bottomLine.B; bottomLine.C = -bottomLine.C;
	}
	if (rightLine.Distance(centerPoint)>0)
	{
		rightLine.A = -rightLine.A; rightLine.B = -rightLine.B; rightLine.C = -rightLine.C;
	}

	TPosition pnt;
	int dist1, dist2, dist3, dist4;
	int tmp;


	unsigned char colorIndex;
	TColorRGB     colorTable[16];
	TColorRGB     colorRGB, colorVector, srcColor, dstColor;

	if (!m_blending)
	{
		colorTable[0]  = m_backgroundColor;
		colorTable[15] = m_foregroundColor;
		colorVector.R  = m_foregroundColor.R - m_backgroundColor.R;
		colorVector.G  = m_foregroundColor.G - m_backgroundColor.G;
		colorVector.B  = m_foregroundColor.B - m_backgroundColor.B;
		for(short i = 1; i<15; i++)
		{
			colorTable[i].R = colorVector.R * i / 15 + colorTable[0].R;
			colorTable[i].G = colorVector.G * i / 15 + colorTable[0].G;
			colorTable[i].B = colorVector.B * i / 15 + colorTable[0].B;
		}
	}

	int dist;
	int colorThresholdTable[15];
	colorThresholdTable[0]  = radius * UNIFIED_VECTOR_SIZE;
	colorThresholdTable[14] = (radius - antialiasFactor)*UNIFIED_VECTOR_SIZE ;
	dist = colorThresholdTable[14] - colorThresholdTable[0];
	for(short i = 1; i<14; i++)
	{
		colorThresholdTable[i] = dist * i / 15 + colorThresholdTable[0];
	}
	
	for(short y = rectangle.Top; y<rectangle.Bottom; y++)
	{
		for(short x = rectangle.Left; x<rectangle.Right; x++)
		{
			pnt.X = x;
			pnt.Y = y;
			dist1 = topLine.Distance(pnt);    
			dist2 = leftLine.Distance(pnt);   
			dist3 = rightLine.Distance(pnt);  
			dist4 = bottomLine.Distance(pnt); 
			if (dist1>0)
			{
				if (dist2>0)
				{
					tmp = (pnt.X - topLeft.X);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist = tmp;

					tmp = (pnt.Y - topLeft.Y);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist += tmp;

					if (dist<2500000) 
					{
						dist *= UNIFIED_VECTOR_SIZE;
						dist = TCanvas::FastSqrt(dist);
					} else {
						//avoids Int32 overflow
						//but it is less accurate
						dist = TCanvas::FastSqrt(dist);
						dist *= UNIFIED_VECTOR_SIZE_SQRT;
					}

				} else
				if (dist3>0)
				{
					tmp = (pnt.X - topRight.X);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist = tmp;

					tmp = (pnt.Y - topRight.Y);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist += tmp;

					if (dist<2500000) 
					{
						dist *= UNIFIED_VECTOR_SIZE;
						dist = TCanvas::FastSqrt(dist);
					} else {
						//avoids Int32 overflow
						//but it is less accurate
						dist = TCanvas::FastSqrt(dist);
						dist *= UNIFIED_VECTOR_SIZE_SQRT;
					}

				} else 
				dist = dist1;
			} else
			if (dist4>0)
			{
				if (dist2>0)
				{
					tmp = (pnt.X - bottomLeft.X);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist = tmp;

					tmp = (pnt.Y - bottomLeft.Y);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist += tmp;

					if (dist<2500000) 
					{
						dist *= UNIFIED_VECTOR_SIZE;
						dist = TCanvas::FastSqrt(dist);
					} else {
						//avoids Int32 overflow
						//but it is less accurate
						dist = TCanvas::FastSqrt(dist);
						dist *= UNIFIED_VECTOR_SIZE_SQRT;
					}
				} else
				if (dist3>0)
				{
					tmp = (pnt.X - bottomRight.X);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist = tmp;

					tmp = (pnt.Y - bottomRight.Y);
					tmp *= UNIFIED_VECTOR_SIZE_SQRT;
					tmp *= tmp;
					dist += tmp;

					if (dist<2500000) 
					{
						dist *= UNIFIED_VECTOR_SIZE;
						dist = TCanvas::FastSqrt(dist);
					} else {
						//avoids Int32 overflow
						//but it is less accurate
						dist = TCanvas::FastSqrt(dist);
						dist *= UNIFIED_VECTOR_SIZE_SQRT;
					}
				} else
				dist = dist4;
			}
			else {
				dist = colorThresholdTable[14]-1;
				if (dist1>dist) dist = dist1;
				if (dist2>dist) dist = dist2;
				if (dist3>dist) dist = dist3;
				if (dist4>dist) dist = dist4;
			}

			if (dist > colorThresholdTable[0])
			{
				//background color
				if (!m_blending) continue;
			} else
			if (dist < colorThresholdTable[15])
			{
				//foreground color
				colorIndex = 15;
			} else {
				colorIndex = 0;
				for(short i = 1; i<15; i++)
				{
					if (dist < colorThresholdTable[i-1])
					{
						colorIndex = (unsigned char)i;                            
					}
				}				
			}
	
			if (m_blending)
			{
				srcColor = GetPixelColor(x, y);
				dstColor.B = (m_foregroundColor.B * colorIndex) + (srcColor.B * (15 - colorIndex));
				dstColor.G = (m_foregroundColor.G * colorIndex) + (srcColor.G * (15 - colorIndex));
				dstColor.R = (m_foregroundColor.R * colorIndex) + (srcColor.R * (15 - colorIndex));                
				SetPixelColor(x, y, dstColor);
			} else {
				SetPixelColor(x, y, colorTable[colorIndex]);
			} 
		}
	}
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

void TCanvas::CopyScaledRectangle(TRectangle targetArea, TGraphicsData* image, TRectangle* imageRectangle)
{
	TRectangle defaultRectangle;
	defaultRectangle.Left   = 0;
	defaultRectangle.Top    = 0;
	defaultRectangle.Right  = image->GetWidth();
	defaultRectangle.Bottom = image->GetHeight();

	if (imageRectangle==NULL)
	{
		imageRectangle = &defaultRectangle;
	}

	short targetWidth  = targetArea.Width();
	short targetHeight = targetArea.Height();

	long stepX = (imageRectangle->Width()  * 65536 / targetArea.Width());
	long stepY = (imageRectangle->Height() * 65536 / targetArea.Height());
	
	TColorRGB pixel1;
	TColorRGB pixel2;
	TColorRGB pixel3;
	TColorRGB pixel4;
	TColorRGB pixel5;

	TScaledCoordinate sy; 
	sy.Part.Integral = imageRectangle->Top;
	sy.Part.Fractional = 0;

	for(short y = targetArea.Top; y<targetArea.Bottom; y++)
	{
		TScaledCoordinate sx;
		sx.Part.Integral = imageRectangle->Left;
		sx.Part.Fractional = 0;
		for(short x = targetArea.Left; x<targetArea.Right; x++)
		{
			pixel1 = image->GetPixelColor(sx.Part.Integral, sy.Part.Integral);
			pixel2 = image->GetPixelColor(sx.Part.Integral+1, sy.Part.Integral);
			pixel3 = image->GetPixelColor(sx.Part.Integral, sy.Part.Integral+1);
			pixel4 = image->GetPixelColor(sx.Part.Integral+1, sy.Part.Integral+1);

			pixel5.R  = (
				          (pixel1.R * (65536 - sx.Part.Fractional) + pixel2.R * sx.Part.Fractional)/65536 * (65536 - sy.Part.Fractional) +
						  (pixel3.R * (65536 - sx.Part.Fractional) + pixel4.R * sx.Part.Fractional)/65536 * sy.Part.Fractional
						) /65536;
			pixel5.G  = (
				          (pixel1.G * (65536 - sx.Part.Fractional) + pixel2.G * sx.Part.Fractional)/65536 * (65536 - sy.Part.Fractional) +
				          (pixel3.G * (65536 - sx.Part.Fractional) + pixel4.G * sx.Part.Fractional)/65536 * sy.Part.Fractional
						) /65536;
			pixel5.B  = (
				          (pixel1.B * (65536 - sx.Part.Fractional) + pixel2.B * sx.Part.Fractional)/65536 * (65536 - sy.Part.Fractional) +
			              (pixel3.B * (65536 - sx.Part.Fractional) + pixel4.B * sx.Part.Fractional)/65536 * sy.Part.Fractional 
						)/65536;					  
			sx.Value += stepX;			
			SetPixelColor(x, y, pixel5);
		}
		sy.Value += stepY;	
	}
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
