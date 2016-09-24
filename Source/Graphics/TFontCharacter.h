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

#ifndef TFONTCHARACTER___H
#define TFONTCHARACTER___H

#include <stdlib.h>
#include "TRectangle.h"

class TFontCharacter
{

public:
	TFontCharacter();
	virtual ~TFontCharacter();

	void GetPixelColor(short x, short y);
	TRectangle GetSize();

};

#endif
