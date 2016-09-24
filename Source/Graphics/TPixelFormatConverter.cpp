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

#include "TPixelFormatConverter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TColorRGB.h"
#include "TColorYUV.h"
#include "TDxtBlockCreator.h"

unsigned char* TPixelFormatConverter::srcPalette = NULL;

TPixelFormatConverter::TPixelFormatConverter()
{

}

TPixelFormatConverter::~TPixelFormatConverter()
{

}

void TPixelFormatConverter::BGRPalette2toRGB565  (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{	
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x80?1:0)));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x40)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x20)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x10)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x08)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x04)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x02)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)&0x01)?1:0));
		*targetData++  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );
		*targetData++  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3);
		pixelCount--;
		if (pixelCount==0) break;

		sourceData++;
	}
}

void TPixelFormatConverter::BGRPalette4toRGB565  (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{	
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3*(((*sourceData)>>6)&3)); 
		*targetData  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );   targetData++;
		*targetData  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3); targetData++;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)>>4)&3));
		*targetData  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );   targetData++;
		*targetData  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3); targetData++;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)>>2)&3)); 
		*targetData  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );   targetData++;
		*targetData  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3); targetData++;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData))&3));
		*targetData  = (unsigned char)(color[1]>>5)        + ( color[2] & 0xF8 );   targetData++;
		*targetData  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3); targetData++;
		pixelCount--;
		if (pixelCount==0) break;

		sourceData++;		
	}
}

void TPixelFormatConverter::BGRPalette16toRGB565  (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{	
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3*((*sourceData)>>4)); 
		*targetData  = (unsigned char)(color[2]&0xF8)      + ( color[1] >> 5 );      targetData++;
		*targetData  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3); targetData++;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&15));
		*targetData  = (unsigned char)(color[2]&0xF8)      + ( color[1] >> 5 );      targetData++;
		*targetData  = (unsigned char)((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3); targetData++;
		pixelCount--;
		if (pixelCount==0) break;

		sourceData++;		
	}
}

void TPixelFormatConverter::BGRPalette256toRGB565 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	while(pixelCount>0)
	{
		unsigned char* color = (unsigned char*)(srcPalette + 3**sourceData);
		*targetData  = (unsigned char)( (color[1]>>5)       + ( color[2] & 0xF8 ));   targetData++;
		*targetData  = (unsigned char)(((color[1]<<3)&0xE0) + ((color[0] & 0xF8)>>3)); targetData++;
		pixelCount--;
	}
}

void TPixelFormatConverter::RGB565toRGB565     (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	memcpy(targetData, sourceData, pixelCount*2);
}

void TPixelFormatConverter::RGB888toRGB565     (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	while(pixelCount>0)
	{
		*targetData++  = (unsigned char)((sourceData[0] & 0xF8)   + (sourceData[1]>>5));   
		*targetData++  = (unsigned char)((sourceData[1]<<3)&0xE0) + ((sourceData[2] & 0xF8)>>3); 
		sourceData+=3;
		pixelCount--;
	}
}

void TPixelFormatConverter::RGBX8888toRGB565   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	while(pixelCount>0)
	{
		*targetData  = (unsigned char)( sourceData[1]>>5)        + (sourceData[0] & 0xE0 );   targetData++;
		*targetData  = (unsigned char)((sourceData[1]<<3)&0xE0) + ((sourceData[2] & 0xE0)/8); targetData++;
		sourceData+=4;
		pixelCount--;
	}
}

void TPixelFormatConverter::RGBA8888toRGB565   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	unsigned char a,b1,b2;
	unsigned short c1,c2;
	while(pixelCount>0)
	{
		b1  = (unsigned char)( sourceData[1]>>5)        + (sourceData[0] & 0xE0 );   targetData++;
		b2  = (unsigned char)((sourceData[1]<<3)&0xE0) + ((sourceData[2] & 0xE0)/8); targetData++;
		a   = sourceData[3];
		c1 = (b1*a + *targetData*(256-a)); c1>>=8;
		c2 = (b1*a + *targetData*(256-a)); c2>>=8;
		*targetData = (unsigned char)c1; targetData++;
		*targetData = (unsigned char)c2; targetData++;
		sourceData+=4;
		pixelCount--;
	}
}

void TPixelFormatConverter::DXT1toRGB565 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	short cx = addParam & 3;
	bool pixel1 = cx<1;
	bool pixel2 = cx<2;
	bool pixel3 = cx<3;
	bool pixel4 = cx<4;
	while(pixelCount>0)
	{		
		unsigned char color[12];
		color[0] =  (sourceData[4]&0xF8);
		color[1] = ((sourceData[4]&0x07)<<5) + ((sourceData[5]&0xE0)>>3);
		color[2] =  (sourceData[5]<<3);
		color[3] =  (sourceData[6]&0xF8);
		color[4] = ((sourceData[6]&0x07)<<5) + ((sourceData[7]&0xE0)>>3);
		color[5] =  (sourceData[7]<<3);

		short colors = 4;

		if (sourceData[5]<sourceData[7]) colors=3;	
		if (sourceData[5]==sourceData[7])
		{
			if (sourceData[4]<sourceData[6]) colors =3;
		}
		if (colors==3)
		{
			color[6]  = (color[0] + color[3]) / 2;
			color[7]  = (color[1] + color[4]) / 2;
			color[8]  = (color[2] + color[5]) / 2;
			color[9]  = 0;
			color[10] = 0;
			color[11] = 0;
		} else {
			color[6]  = (color[0] * 2 + color[3]) / 3;
			color[7]  = (color[1] * 2 + color[4]) / 3;
			color[8]  = (color[2] * 2 + color[5]) / 3;
			color[9]  = (color[0] + color[3]  * 2) / 3;
			color[10] = (color[1] + color[4] * 2) / 3;
			color[11] = (color[2] + color[5] * 2) / 3;
		}

		unsigned char b,i;

		short cx = addParam  & 3;
		short cy = addParam >> 2;

		b = sourceData[cy];

		if (pixel1)
		{
			i = ((b&0xC0)>>6)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=2;
			} else {
				*targetData++ = (color[i]&0xF8) + (color[i+1]>>5);
				*targetData++ = ((color[i+1]<<3)&0xE0) + (color[i+2]>>3);
			}
			pixelCount--;
		}
		if (pixel2)
		{
			i = ((b&0x30)>>4)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=2;
			} else {
				*targetData++ = (color[i]&0xF8) + (color[i+1]>>5);
				*targetData++ = ((color[i+1]<<3)&0xE0) + (color[i+2]>>3);
			}
			pixelCount--;
		}
		if (pixel3)
		{
			i = ((b&0x0C)>>2)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=2;
			} else {
				*targetData++ = (color[i]&0xF8) + (color[i+1]>>5);
				*targetData++ = ((color[i+1]<<3)&0xE0) + (color[i+2]>>3);
			}
			pixelCount--;
		}
		if (pixel4)
		{
			i = (b & 3)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=2;
			} else {
				*targetData++ = (color[i]&0xF8) + (color[i+1]>>5);
				*targetData++ = ((color[i+1]<<3)&0xE0) + (color[i+2]>>3);
			}
			pixelCount--;
		}
		sourceData+=8;

		pixel1 = pixelCount>=1;
		pixel2 = pixelCount>=2;
		pixel3 = pixelCount>=3;
		pixel4 = pixelCount>=4;

	}

}

void TPixelFormatConverter::BGRPalette2toRGB888 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x80?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x40?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x20?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x10?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x08?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x04?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x02?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&0x01?1:0));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;

		sourceData++;
	}

}

void TPixelFormatConverter::BGRPalette4toRGB888 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3*(((*sourceData)>>6)&3));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)>>4)&3));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData)>>2)&3));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*(((*sourceData))&3));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;

		sourceData++;
	}

}


void TPixelFormatConverter::BGRPalette16toRGB888  (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3*((*sourceData)>>4));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
		if (pixelCount==0) break;

		color = (unsigned char*)(srcPalette + 3*((*sourceData)&15));
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;

		sourceData++;
		pixelCount--;
	}
}

void TPixelFormatConverter::BGRPalette256toRGB888 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	unsigned char* color;
	while(pixelCount>0)
	{		
		color = (unsigned char*)(srcPalette + 3**sourceData);
		*targetData  = color[2];  targetData++;
		*targetData  = color[1];  targetData++;
		*targetData  = color[0];  targetData++;
		color+=3;
		pixelCount--;
	}
}

void TPixelFormatConverter::RGB565toRGB888     (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	while(pixelCount>0)
	{
		*targetData++ = ( sourceData[0]&0xF8);
		*targetData++ = ((sourceData[0]&0x07)<<5) + ((sourceData[1]&0xE0)>>3);
		*targetData++ = ( sourceData[1]&0x1F)<<3;
		sourceData+=2;
		pixelCount--;
	}
}

void TPixelFormatConverter::RGB888toRGB888     (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	memcpy(targetData, sourceData, pixelCount*3);
}

void TPixelFormatConverter::RGBX8888toRGB888   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	while(pixelCount>0)
	{
		*targetData++ = *sourceData++;
		*targetData++ = *sourceData++;
		*targetData++ = *sourceData++;
		sourceData++;
		pixelCount--;
	}
}

void TPixelFormatConverter::RGBA8888toRGB888   (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	while(pixelCount>0)
	{
		unsigned char a = sourceData[3];
		*targetData++ = a*(*sourceData++) + (256-a)*(*targetData);
		*targetData++ = a*(*sourceData++) + (256-a)*(*targetData);
		*targetData++ = a*(*sourceData++) + (256-a)*(*targetData);
		sourceData++;
		pixelCount--;
	}	
}

void TPixelFormatConverter::DXT1toRGB888 (unsigned char* targetData, unsigned char* sourceData, short pixelCount, short addParam)
{
	short cx = addParam & 3;
	bool pixel1 = cx<1;
	bool pixel2 = cx<2;
	bool pixel3 = cx<3;
	bool pixel4 = cx<4;
	while(pixelCount>0)
	{		
		
		unsigned char color[12];
		color[0] =  (sourceData[4]&0xF8);
		color[1] = ((sourceData[4]&0x07)<<5) + ((sourceData[5]&0xE0)>>3);
		color[2] =  (sourceData[5]<<3);
		color[3] =  (sourceData[6]&0xF8);
		color[4] = ((sourceData[6]&0x07)<<5) + ((sourceData[7]&0xE0)>>3);
		color[5] =  (sourceData[7]<<3);

		short colors = 4;
		
		if (sourceData[5]<sourceData[7]) colors=3;	
		if (sourceData[5]==sourceData[7])
		{
			if (sourceData[4]<sourceData[6]) colors =3;
		}
		if (colors==3)
		{
			color[6]  = (color[0] + color[3]) / 2;
			color[7]  = (color[1] + color[4]) / 2;
			color[8]  = (color[2] + color[5]) / 2;
			color[9]  = 0;
			color[10] = 0;
			color[11] = 0;
		} else {
			color[6]  = (color[0] * 2 + color[3]) / 3;
			color[7]  = (color[1] * 2 + color[4]) / 3;
			color[8]  = (color[2] * 2 + color[5]) / 3;
			color[9]  = (color[0] + color[3]  * 2) / 3;
			color[10] = (color[1] + color[4] * 2) / 3;
			color[11] = (color[2] + color[5] * 2) / 3;
		}

		unsigned char b,i;

		short cx = addParam  & 3;
		short cy = addParam >> 2;

		b = sourceData[cy];

		if (pixel1)
		{
			i = ((b&0xC0)>>6)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=3;
			} else {
				*targetData++ = color[i++];
				*targetData++ = color[i++];
				*targetData++ = color[i++];
			}
			pixelCount--;
		}
		if (pixel2)
		{
			i = ((b&0x30)>>4)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=3;
			} else {
				*targetData++ = color[i++];
				*targetData++ = color[i++];
				*targetData++ = color[i++];
			}
			pixelCount--;
		}
		if (pixel3)
		{
			i = ((b&0x0C)>>2)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=3;
			} else {
				*targetData++ = color[i++];
				*targetData++ = color[i++];
				*targetData++ = color[i++];
			}
			pixelCount--;
		}
		if (pixel4)
		{
			i = (b & 3)*3;
			if ((colors==3) && (i==9))
			{
				targetData+=3;
			} else {
				*targetData++ = color[i++];
				*targetData++ = color[i++];
				*targetData++ = color[i++];
			}
			pixelCount--;
		}
		sourceData+=8;
		
		pixel1 = pixelCount>=1;
		pixel2 = pixelCount>=2;
		pixel3 = pixelCount>=3;
		pixel4 = pixelCount>=4;
		
	}
}


bool TPixelFormatConverter::CopyRect(TGraphicsData& target, TPosition targetPosition, TGraphicsData& source)
{
	TRectangle sourceArea(0, source.GetWidth(), 0, source.GetHeight());
	return CopyRect(target, targetPosition, source, sourceArea);
}

bool TPixelFormatConverter::CopyRect(TGraphicsData& target, TPosition targetPosition, TGraphicsData& source, TRectangle sourceArea)
{
	srcPalette = source.GetPalette();

	ePixelFormat tgtFmt = target.GetPixelFormat();
	if ((tgtFmt==pfDXT1) || (tgtFmt==pfDXT3))// || (tgtFmt==pfDXT5))
	{ 
		if ((targetPosition.X==0) && (targetPosition.Y==0))
		{
			return ConvertBMPtoDXT(target, source);
		}		
	}

	FormatConverterFunction callback = GetFormatConverter(target.GetPixelFormat(), source.GetPixelFormat());
	if (callback==NULL)
	{
		return false;
	}

	ePixelFormat srcFmt = source.GetPixelFormat();
	bool dxt = (srcFmt==pfDXT1) || (srcFmt==pfDXT3);// || (srcFmt==pfDXT5);

	short addParam = 0;
	unsigned char* tgt = NULL;
	unsigned char* src = NULL;

	if (sourceArea.Left>source.GetWidth()) return true;
	if (sourceArea.Top>source.GetHeight()) return true;
	if (sourceArea.Right>source.GetWidth()) sourceArea.Right = source.GetWidth();
	if (sourceArea.Bottom>source.GetHeight()) sourceArea.Bottom = source.GetHeight();

	TRectangle targetArea(targetPosition.X, targetPosition.X+sourceArea.Width(),
		                  targetPosition.Y, targetPosition.Y+sourceArea.Height());
	if (targetArea.Left>target.GetWidth()) return true;
	if (targetArea.Top>target.GetHeight()) return true;
	if (targetArea.Right>target.GetWidth())   targetArea.Right = target.GetWidth();
	if (targetArea.Bottom>target.GetHeight()) targetArea.Bottom = target.GetHeight();
	short cx    = sourceArea.Left;	
	short width = targetArea.Width();
	for(short cy = 0; cy<targetArea.Height(); cy++)
	{
		tgt = target.ScanLine(cy+targetPosition.Y);
		if (tgt==NULL) continue;
		tgt += (targetPosition.X*target.GetBitsPerPixel())/8;

		if (dxt)
		{
			addParam = (cy & 3)*4 + (cx & 3);
			src  = source.ScanLine((cy+sourceArea.Top)/4);
			if (src==NULL) continue;
			src += ((sourceArea.Left&0xFFFC) * 8);
		} else {
			src  = source.ScanLine(cy+sourceArea.Top);
			if (src==NULL) continue;
			src += (sourceArea.Left * source.GetBitsPerPixel())/8;
		}		
		callback(tgt, src, width, addParam);
	}
	return true;
}

bool TPixelFormatConverter::CopyRect(TGraphicsData& target, TRectangle targetArea, TGraphicsData& source, TRectangle sourceArea)
{
	//scaling
	for(int cy = targetArea.Top; cy<targetArea.Height(); cy++)
	{
		for (int cx = targetArea.Left; cx<targetArea.Width(); cx++)
		{
			sourceArea.Left = 0;

		}
	}
	return true;
}


FormatConverterFunction TPixelFormatConverter::GetFormatConverter(ePixelFormat targetFormat, ePixelFormat sourceFormat)
{
	if (targetFormat==pfRGB565)
	{
		switch(sourceFormat)
		{
			case pfBGR2ColorsPalette: return BGRPalette2toRGB565;
			case pfBGR4ColorsPalette: return BGRPalette4toRGB565;
			case pfBGR16ColorsPalette: return BGRPalette16toRGB565;
			case pfBGR256ColorsPalette: return BGRPalette256toRGB565;
			case pfRGB565: return RGB565toRGB565;
			case pfRGB888: return RGB888toRGB565;			
			case pfRGBA8888: return RGBA8888toRGB565;
			case pfRGBX8888: return RGBX8888toRGB565;
			case pfDXT1: return DXT1toRGB565;
		}
	}
	if (targetFormat==pfRGB888)
	{
		switch(sourceFormat)
		{
			case pfBGR2ColorsPalette:  return BGRPalette2toRGB888;
			case pfBGR4ColorsPalette: return BGRPalette4toRGB888;
			case pfBGR16ColorsPalette:  return BGRPalette16toRGB888;
			case pfBGR256ColorsPalette: return BGRPalette256toRGB888;
			case pfRGB565:   return RGB565toRGB888;
			case pfRGB888:   return RGB888toRGB888;
			case pfRGBX8888: return RGBX8888toRGB888;
			case pfRGBA8888: return RGBA8888toRGB888;
			case pfDXT1: return DXT1toRGB888;
		}
	}	
	return NULL;
}

bool TPixelFormatConverter::ConvertBMPtoDXT(TGraphicsData& target, TGraphicsData& source)
{
	short width  = source.GetWidth();
	short height = source.GetHeight();
	if (target.GetWidth()<width) return false;
	if (target.GetHeight()<height) return false;


	ePixelFormat srcFmt = source.GetPixelFormat();
	ePixelFormat tmpFmt = pfRGB888;

	FormatConverterFunction callback = GetFormatConverter(tmpFmt, srcFmt);

	short bytesPerLine = width;
	while(bytesPerLine & 3) bytesPerLine++;
	bytesPerLine *= 3;

	unsigned char* tmpBuffer1 = (unsigned char*)malloc(bytesPerLine);
	unsigned char* tmpBuffer2 = (unsigned char*)malloc(bytesPerLine);
	unsigned char* tmpBuffer3 = (unsigned char*)malloc(bytesPerLine);
	unsigned char* tmpBuffer4 = (unsigned char*)malloc(bytesPerLine);

	memset(tmpBuffer1, 0, bytesPerLine);
	memset(tmpBuffer2, 0, bytesPerLine);
	memset(tmpBuffer3, 0, bytesPerLine);
	memset(tmpBuffer4, 0, bytesPerLine);

	unsigned char* tgt = NULL;
	unsigned char* src = NULL;

	unsigned long  colorKey= 0x10000;
	short cy  = 0;
	while(cy<height)
	{
		tgt = target.ScanLine(cy/4);
		src = source.ScanLine(cy); callback((unsigned char*)tmpBuffer1, src, width, 0);
		cy++;
		if (cy<height)
		{
			src = source.ScanLine(cy); callback((unsigned char*)tmpBuffer2, src, width, 0);
			cy++;
		} else {
			memset(tmpBuffer2, 0, bytesPerLine);
		}
		if (cy<height)
		{
			src = source.ScanLine(cy); callback((unsigned char*)tmpBuffer3, src, width, 0);
			cy++;
		} else {
			memset(tmpBuffer3, 0, bytesPerLine);
		}
		if (cy<height)
		{
			src = source.ScanLine(cy); callback((unsigned char*)tmpBuffer4, src, width, 0);
			cy++;
		} else {
			memset(tmpBuffer4, 0, bytesPerLine);
		}
		unsigned char* p1 = tmpBuffer1;
		unsigned char* p2 = tmpBuffer2;
		unsigned char* p3 = tmpBuffer3;
		unsigned char* p4 = tmpBuffer4;

		TDxtBlockCreator dxtBlock;
		for(short cx=0; cx<width; cx+=4)
		{
			dxtBlock.Pixel[0].rgb  = TColorRGB(p1[0], p1[1], p1[2]); p1+=3;			
			dxtBlock.Pixel[1].rgb  = TColorRGB(p1[0], p1[1], p1[2]); p1+=3;
			dxtBlock.Pixel[2].rgb  = TColorRGB(p1[0], p1[1], p1[2]); p1+=3;
			dxtBlock.Pixel[3].rgb  = TColorRGB(p1[0], p1[1], p1[2]); p1+=3;
			dxtBlock.Pixel[4].rgb  = TColorRGB(p2[0], p2[1], p2[2]); p2+=3;
			dxtBlock.Pixel[5].rgb  = TColorRGB(p2[0], p2[1], p2[2]); p2+=3;
			dxtBlock.Pixel[6].rgb  = TColorRGB(p2[0], p2[1], p2[2]); p2+=3;
			dxtBlock.Pixel[7].rgb  = TColorRGB(p2[0], p2[1], p2[2]); p2+=3;
			dxtBlock.Pixel[8].rgb  = TColorRGB(p3[0], p3[1], p3[2]); p3+=3;
			dxtBlock.Pixel[9].rgb  = TColorRGB(p3[0], p3[1], p3[2]); p3+=3;
			dxtBlock.Pixel[10].rgb = TColorRGB(p3[0], p3[1], p3[2]); p3+=3;
			dxtBlock.Pixel[11].rgb = TColorRGB(p3[0], p3[1], p3[2]); p3+=3;
			dxtBlock.Pixel[12].rgb = TColorRGB(p4[0], p4[1], p4[2]); p4+=3;
			dxtBlock.Pixel[13].rgb = TColorRGB(p4[0], p4[1], p4[2]); p4+=3;
			dxtBlock.Pixel[14].rgb = TColorRGB(p4[0], p4[1], p4[2]); p4+=3;
			dxtBlock.Pixel[15].rgb = TColorRGB(p4[0], p4[1], p4[2]); p4+=3;
			dxtBlock.TransparentColor = target.GetTransparentColor();
			dxtBlock.TransparentColorUsed = target.IsTransparentColorUsed();

			dxtBlock.DoBlockCompression();

			*tgt++ = (dxtBlock.Pixel[0].colorIndex<<6) +
				     (dxtBlock.Pixel[1].colorIndex<<4) +
					 (dxtBlock.Pixel[2].colorIndex<<2) +
					  dxtBlock.Pixel[3].colorIndex;
			*tgt++ = (dxtBlock.Pixel[4].colorIndex<<6) +
					 (dxtBlock.Pixel[5].colorIndex<<4) +
				     (dxtBlock.Pixel[6].colorIndex<<2) +
				      dxtBlock.Pixel[7].colorIndex;
			*tgt++ = (dxtBlock.Pixel[8].colorIndex<<6) +
				     (dxtBlock.Pixel[9].colorIndex<<4) +
				     (dxtBlock.Pixel[10].colorIndex<<2) +
				      dxtBlock.Pixel[11].colorIndex;
			*tgt++ = (dxtBlock.Pixel[12].colorIndex<<6) +
				     (dxtBlock.Pixel[13].colorIndex<<4) +
				     (dxtBlock.Pixel[14].colorIndex<<2) +
				     dxtBlock.Pixel[15].colorIndex;
			unsigned short c1 = dxtBlock.Color0.ConvertToRGB565();
			*tgt++ = (unsigned char)c1;
			*tgt++ = (unsigned char)(c1>>8);
			unsigned short c2 = dxtBlock.Color1.ConvertToRGB565();
			*tgt++ = (unsigned char)c2;
			*tgt++ = (unsigned char)(c2>>8);
		}		
	}

	return true;
}



