/*
 * Persistence Library / Graphics / TDXTBlockCreator
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

#include "TDxtBlockCreator.h"
#include "TColorConverter.h"
#include <string.h>
#include <math.h>

TDxtBlockCreator::TDxtBlockCreator()
{
	Color0 = 0;
	Color1 = 0;	
	TransparentColor = 0;
	TransparentColorUsed = false;
	memset((void*)Pixel, 0, sizeof(Pixel) );
}

void TDxtBlockCreator::DoBlockCompression()
{
	short i;
	short colors = 4;

	short pixel_y[16];
	short pixel_u[16];
	short pixel_v[16];

	for(i = 0; i<16; i++)
	{	
		if ((TransparentColorUsed) && (Pixel[i].rgb==TransparentColor))
		{			
			pixel_y[i] = 0x7FFF;
			pixel_u[i] = 0x7FFF;
			pixel_v[i] = 0x7FFF;
			Pixel[i].colorIndex = 3;
			colors = 3;
		} else {	
			TColorYUV yuv = TColorConverter::RGBtoYUV(Pixel[i].rgb);
			Pixel[i].yuv = yuv;
			Pixel[i].colorIndex = 0;

			pixel_y[i] = yuv.Y;
			pixel_u[i] = yuv.U;
			pixel_v[i] = yuv.V;						
		}		
	}

	short bestMinY, bestMaxY;
	short bestMinU, bestMaxU;
	short bestMinV, bestMaxV;

	LeastSquare(pixel_y, bestMinY, bestMaxY, colors);
	LeastSquare(pixel_u, bestMinU, bestMaxU, colors);
	LeastSquare(pixel_v, bestMinV, bestMaxV, colors);

	TColorYUV yuv1, yuv2;
	TColorYUV bestYUV0, bestYUV1;
	long      disp;
	long      bestDisp = 2147483647;

	yuv1.Y = (unsigned char)bestMinY;
	yuv2.Y = (unsigned char)bestMaxY;

	for(i = 0; i<5; i++)
	{
		switch(i)
		{
			case 0: yuv1.U = (unsigned char)bestMinU; yuv2.U = (unsigned char)bestMaxU;
		     		yuv1.V = (unsigned char)bestMinV; yuv2.V = (unsigned char)bestMaxV; break;
			case 1: yuv1.U = (unsigned char)bestMinU; yuv2.U = (unsigned char)bestMaxU;
					yuv1.V = (unsigned char)bestMaxV; yuv2.V = (unsigned char)bestMinV; break;
			case 2: yuv1.U = (unsigned char)bestMaxU; yuv2.U = (unsigned char)bestMinU;
		     		yuv1.V = (unsigned char)bestMinV; yuv2.V = (unsigned char)bestMaxV; break;
			case 3: yuv1.U = (unsigned char)bestMaxU; yuv2.U = (unsigned char)bestMinU;
					yuv1.V = (unsigned char)bestMaxV; yuv2.V = (unsigned char)bestMinV; break;
			case 4: yuv1.U = 0;        yuv2.U = 0;
				    yuv1.V = 0;        yuv2.V = 0; break;

		}		
		disp = AssignColors(yuv1, yuv2, colors);
		if (disp<bestDisp)
		{
			bestDisp = disp;
			bestYUV0 = yuv1;
			bestYUV1 = yuv2;
		}
	}

    Color0 = bestYUV0.ConvertToRGB();
	Color1 = bestYUV1.ConvertToRGB();
	if (Color0.ConvertToRGB565()==Color1.ConvertToRGB565())
	{		
		if (Color0.B<128)
		{
			Color1.B+=8;
		} else {
			Color0.B-=8;
		}		
	}	
	if ((colors==3)^(Color0.ConvertToRGB565()<=Color1.ConvertToRGB565()))
	{
		TColorRGB tmp  = Color0;   Color0 = Color1;     Color1 = tmp;
		TColorYUV tmp2 = bestYUV0; bestYUV0 = bestYUV1; bestYUV1 = tmp2;
	}
	AssignColors(bestYUV0, bestYUV1, colors);
}

long TDxtBlockCreator::AssignColors(TColorYUV yuv1, TColorYUV yuv4, short numberOfColors)
{
	TColorYUV yuv2 ( (yuv1.Y*2+yuv4.Y)/3,
		             (yuv1.U*2+yuv4.U)/3,
					 (yuv1.V*2+yuv4.V)/3 );

	TColorYUV yuv3 ( (yuv1.Y+yuv4.Y*2)/3,
				     (yuv1.U+yuv4.U*2)/3,
		             (yuv1.V+yuv4.V*2)/3 );

	TColorYUV yuv2b ((yuv1.Y+yuv4.Y)/2,
					 (yuv1.U+yuv4.U)/2,
					 (yuv1.V+yuv4.V)/2 );


	long distsum = 0;
	long dist1, dist2, dist2b, dist3, dist4, dist;
	long y, u, v;
	for(short k = 0; k<16; k++)
	{
		if (numberOfColors==3)
		{
			if (Pixel[k].colorIndex == 3) continue;
		}
		y = (Pixel[k].yuv.Y - yuv1.Y); y*=y; y*=16;
		u = (Pixel[k].yuv.U - yuv1.U); u*=u;
		v = (Pixel[k].yuv.V - yuv1.V); v*=v;
		dist1 = y + u + v;

		y = (Pixel[k].yuv.Y - yuv4.Y); y*=y; y*=16; 
		u = (Pixel[k].yuv.U - yuv4.U); u*=u;
		v = (Pixel[k].yuv.V - yuv4.V); v*=v;
		dist4 = y + u + v;

		if (numberOfColors==3)
		{
			y = (Pixel[k].yuv.Y - yuv2b.Y); y*=y; y*=16;
			u = (Pixel[k].yuv.U - yuv2b.U); u*=u;
			v = (Pixel[k].yuv.V - yuv2b.V); v*=v;
			dist2b = y + u + v;

			dist = dist1; Pixel[k].colorIndex = 0;
			if (dist4<dist)  { dist = dist4;  Pixel[k].colorIndex = 1; }
			if (dist2b<dist) { dist = dist2b; Pixel[k].colorIndex = 2; }			
		} else {

			y = (Pixel[k].yuv.Y - yuv2.Y); y*=y;  y*=16;
			u = (Pixel[k].yuv.U - yuv2.U); u*=u;
			v = (Pixel[k].yuv.V - yuv2.V); v*=v;
			dist2 = y + u + v;

			y = (Pixel[k].yuv.Y - yuv3.Y); y*=y;  y*=16;
			u = (Pixel[k].yuv.U - yuv3.U); u*=u;
			v = (Pixel[k].yuv.V - yuv3.V); v*=v;
			dist3 = y + u + v;

			dist = dist1; Pixel[k].colorIndex = 0;
			if (dist2<dist) { dist = dist2; Pixel[k].colorIndex = 2; }
			if (dist3<dist) { dist = dist3; Pixel[k].colorIndex = 3; }
			if (dist4<dist) { dist = dist4; Pixel[k].colorIndex = 1; }
		}
		distsum += dist;
	}
	return distsum;
}

void TDxtBlockCreator::LeastSquare(short* valueArray, short& bestValue1, short &bestValue2, short numberOfColors)
{
	long  minDist = 2147483647;

	short valueMin =  32767;
	short valueMax = -32767;
	short valueStep = numberOfColors;
	if ((valueMax-valueMin)<=4)
	{
		valueStep = 1;
	}
	if ((valueMax-valueMin)>=128)
	{
		valueStep *= 2;
	}
	for(short k = 0; k<16; k++)
	{
		if (valueArray[k]==0x7FFF) continue;
		if (valueArray[k]<valueMin) valueMin = valueArray[k];
		if (valueArray[k]>valueMax) valueMax = valueArray[k];
	}
	if (valueMax<valueMin)
	{
		bestValue1 = 0x7FFF;
		bestValue2 = 0x7FFF;
		return;
	}
	if ((valueMax-valueMin)<8)
	{
		bestValue1 = valueMin;
		bestValue2 = valueMax;
		return;
	}

	short c1, c2, c3, c4;
	for(short i = valueMin; i<valueMax; i+=valueStep)
	{		
		for(short j=i+valueStep; j<valueMax; j+=valueStep)
		{						
 			if (numberOfColors==3)
			{
				c1 = i;
				c2 = (i + j)/2;
				c3 = j;
				short dist1, dist2, dist3, dist;
				long distsum = 0;
				for(short k = 0; k<16; k++)
				{
					if (valueArray[k]==0x7FFF)
					{
						//transparent pixels are skipped
						//from color interpolation
						continue;
					}
					dist1 = valueArray[k] - c1; if (abs(dist1)<180) dist1 *= dist1; else dist1 = 0x7FFF;
					dist2 = valueArray[k] - c2; if (abs(dist2)<180) dist2 *= dist2; else dist2 = 0x7FFF;
					dist3 = valueArray[k] - c2; if (abs(dist3)<180) dist3 *= dist3; else dist3 = 0x7FFF;
					dist = dist1;
					if (dist2<dist) dist = dist2;
					if (dist3<dist) dist = dist3;
					distsum += dist;
				}
				if (distsum<minDist)
				{
					minDist = distsum;
					bestValue1 = c1;
					bestValue2 = c3;
				}

			} else {
				c1 =  i;
				c2 = (i * 2 + j)/3;
				c3 = (i + j * 2)/3;
				c4 =  j-1;
				short dist1, dist2, dist3, dist4, dist;
				long distsum = 0;
				for(short k = 0; k<16; k++)
				{
					if (valueArray[k]==0x7FFF)
					{
						//transparent pixels are skipped
						//from color interpolation
						continue;
					}
					dist1 = valueArray[k] - c1; if (abs(dist1)<180) dist1 *= dist1; else dist1 = 0x7FFF;
					dist2 = valueArray[k] - c2; if (abs(dist2)<180) dist2 *= dist2; else dist2 = 0x7FFF;
					dist3 = valueArray[k] - c3; if (abs(dist3)<180) dist3 *= dist3; else dist3 = 0x7FFF;
					dist4 = valueArray[k] - c4; if (abs(dist4)<180) dist4 *= dist4; else dist4 = 0x7FFF;
					dist = dist1;
					if (dist2<dist) dist = dist2;
					if (dist3<dist) dist = dist3;
					if (dist4<dist) dist = dist4;
					distsum += dist;
				}
				if (distsum<minDist)
				{
					minDist = distsum;
					bestValue1 = c1;
					bestValue2 = c4;
				}
			}
		}
	}
}

