/*
 * Persistence Library / Graphics / TRectangle
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

#include "TRectangle.h"

TRectangle::TRectangle()
{
	Left   = 0;
	Right  = 0;
	Top    = 0;
	Bottom = 0;
}

TRectangle::TRectangle(const TRectangle &src)
{
	this->Left = src.Left;
	this->Right = src.Right;
	this->Top  = src.Top;
	this->Bottom = src.Bottom;
}

TRectangle::TRectangle(short left, short right, short top, short bottom)
{
	Left   = left;
	Right  = right;
	Top    = top;
	Bottom = bottom;
}

short TRectangle::Width()
{
	return Right-Left;
}

short TRectangle::Height()
{
	return Bottom-Top;
}

TRectangle& TRectangle::operator = (TRectangle &src)
{
	this->Left = src.Left;
	this->Right = src.Right;
	this->Top = src.Top;
	this->Bottom = src.Bottom;
	return *this;
}



