/*
 * Persistence Library / Graphics / TFontCharacter
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

#include "TFontCharacter.h"

TFontCharacter::TFontCharacter()
{

}

TFontCharacter::~TFontCharacter()
{

}

void TFontCharacter::GetPixelColor(short x, short y)
{

}

TRectangle TFontCharacter::GetSize()
{
    static TRectangle invalidValue(0,0,0,0);

	return invalidValue;
}