/*
 * Persistence Library / Graphics / TColorYUV
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

#include "TColorYUV.h"
#include "TColorRGB.h"

TColorYUV::TColorYUV()
{
	this->Y = 0;
	this->U = 0;
	this->V = 0;
}

TColorYUV::TColorYUV(const TColorYUV& color)
{
	this->Y = color.Y;
	this->U = color.U;
	this->V = color.V;
}

TColorYUV::TColorYUV(short y, short u, short v)
{
	if (y>255) y = 255;
	if (y<0) y =0;
	if (u>127)  u = 127;
	if (u<-127) u = -127;
	if (v>127)  v = 127;
	if (v<-127) v = -127;
	this->Y = (unsigned char)y;
	this->U = (unsigned char)u;
	this->V = (unsigned char)v;
}

TColorRGB TColorYUV::ConvertToRGB()
{
	TColorRGB result((255*Y + 358*V + 128)/255,
		             (255*Y - 88*U - 182*V)/255,
					 (255*Y + 451*U + 128)/255
		            );
	return result;
}

TColorYUV& TColorYUV::operator = (const TColorYUV& color)
{
	this->Y = color.Y;
	this->U = color.U;
	this->V = color.V;
	return *this;
}

bool TColorYUV::operator == (TColorYUV& color)
{
	return (color.Y==Y) && (color.U == U) && (color.V == V);
}
