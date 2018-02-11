/*
 * Persistence Library / Graphics / TPosition
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

#include "TPosition.h"
#include "TVector.h"

TPosition::TPosition()
{
	this->X = 0;
	this->Y = 0;
}

TPosition::TPosition(const TPosition& src)
{
	this->X = src.X;
	this->Y = src.Y;
}

TPosition::TPosition(short x, short y)
{
    this->X = x;
    this->Y = y;
}

TPosition& TPosition::operator=(const TPosition &pos)
{
	this->X = pos.X;
	this->Y = pos.Y;
	return *this;
}

TPosition  TPosition::operator+(const TVector   &pos)
{
	TPosition result;
	result.X = X + pos.DirectionX;
	result.Y = Y + pos.DirectionY;
	return result;
}

TPosition  TPosition::operator-(const TVector   &pos)
{
	TPosition result;
	result.X = X - pos.DirectionX;
	result.Y = Y - pos.DirectionY;
	return result;
}


TVector    TPosition::operator-(const TPosition &pos)
{
	TVector result;
	result.DirectionX = X - pos.X;
	result.DirectionY = Y - pos.Y;
	return result;
}