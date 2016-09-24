/*
 * Persistence Library / Graphics / TFont
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

#ifndef TFONT___H
#define TFONT___H

#include <stdlib.h>
#include "TFontCharacter.h"

class TFont
{

public:
	TFont(const unsigned char* fontData, unsigned long fontDataSize);
	virtual ~TFont();
	
	TFontCharacter GetCharacter(char character);

};

#endif
