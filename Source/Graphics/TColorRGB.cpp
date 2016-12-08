/*
 * Persistence Library / Graphics / TColorRGB
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

#include "TColorRGB.h"
#include "TColorYUV.h"

TColorRGB::TColorRGB()
{
	this->R = 0;
	this->G = 0;
	this->B = 0;
	this->A = 255;
}

TColorRGB::TColorRGB(short r, short g, short b)
{
	if (r>255) r = 255;
	if (r<0)   r = 0;
	if (g>255) g = 255;
	if (g<0)   g = 0;
	if (b>255) b = 255;
	if (b<0)   b = 0;

	this->R = (unsigned char)r;
	this->G = (unsigned char)g;
	this->B = (unsigned char)b;
}

TColorRGB::TColorRGB(unsigned long rgb)
{
	this->R = (unsigned char)(rgb >> 16);
	this->G = (unsigned char)(rgb >> 8);
	this->B = (unsigned char) rgb;
}



unsigned short TColorRGB::ConvertToRGB565()
{
	unsigned char l = (R & 0xF8)    + (G>>5);
	unsigned char h = ((G<<3)&0xE0) + (B>>3);
	return l + (h<<8);
}

TColorRGB TColorRGB::operator = (TColorRGB color)
{
	this->R = color.R;
	this->G = color.G;
	this->B = color.B;
	this->A = color.A;
	return *this;
}

TColorRGB TColorRGB::operator = (unsigned long color)
{
	this->R = (unsigned char)(color>>16);
	this->G = (unsigned char)(color>>8);
	this->B = (unsigned char)(color);
	this->A = (unsigned char)(color>>24);
	return *this;
}

bool TColorRGB::operator == (TColorRGB& color)
{
	return ((color.R == R) && (color.G==G) && (color.B==B));
}

