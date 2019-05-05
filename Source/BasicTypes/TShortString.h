/*
 * Persistence Library / Basic types / TShortString 
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

#ifndef TSHORTSTRING___H
#define TSHORTSTRING___H

#include "TString.h"

#define SHORTSTRINGLENGTH 40
//40 characters is usually enough to store any name, surname, address, city, etc.

/**
 *  TShortString is a TCustomString with predefined text length.
 *  Memory is always allocated statically in local class member.
 *
 */

class TShortString : public TString
{
private:
    unsigned char  Data[SHORTSTRINGLENGTH+1];  //one more byte for terminal zero

public:

	TShortString();
	TShortString(const char* pChar, unsigned short pCharLen=0);
	TShortString(const TShortString& s);
	TShortString& operator = (const TShortString& oString );
	TShortString& operator = (const TString& oString );
	TShortString& operator = (const char* pChar);
	bool operator == (const TShortString& s);
	bool operator == (const char* pChar);
	bool operator != (const TShortString& s);
	bool operator != (const char* pChar);
	int MaxLength();
};

#endif
