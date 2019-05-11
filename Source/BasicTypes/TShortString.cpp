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

#include "TShortString.h"

TShortString::TShortString()
{
    Data[0] = 0;
	PData = (char*)&Data;
	DataLen = 0;
	DataMax = SHORTSTRINGLENGTH+1;
	DataStatic = true;
	Clear();
}

TShortString::TShortString(const char* pChar, unsigned short pCharLen)
	:TString()
{
	PData = (char*)&Data;
	DataLen = 0;
	DataMax = SHORTSTRINGLENGTH+1;
	DataStatic = true;
	if (pChar)
	{
		if (pCharLen==0)
		{
			pCharLen = (unsigned short)strlen(pChar);
		}
		CopyFrom(pChar, pCharLen);    
	}
}

TShortString::TShortString(const TShortString& s)
	:TString()
{
    PData = (char*)&Data;
    DataLen = 0;
    DataMax = SHORTSTRINGLENGTH+1;
    DataStatic = true;
    CopyFrom(s.ToPChar(), s.Length());    
}

TShortString& TShortString::operator = (const TShortString& oString )
{
    CopyFrom(oString.ToPChar());
    return *this;
}

TShortString& TShortString::operator = (const TString& oString )
{
	CopyFrom(oString.ToPChar());
	return *this;
}

TShortString& TShortString::operator = ( const char* pChar)
{
	CopyFrom(pChar);
	return *this;
}

bool TShortString::operator == (const TShortString& s)
{
	if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return true;
	if (ToPChar()==NULL) return false;
	if (s.ToPChar()==NULL) return false;
	return strcmp(ToPChar(), s.ToPChar()) == 0;
}

bool TShortString::operator == (const char* pChar)
{
    if ((ToPChar()==NULL) && (pChar==NULL)) return true;
    if ((ToPChar()==NULL) && (*pChar==0)) return true;
	if (ToPChar()==NULL) return false;
	if (pChar==NULL) return false;
    return strcmp(ToPChar(), pChar) == 0;
}

bool TShortString::operator != (const TShortString& s)
{
    if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return false;
    if (ToPChar()==NULL) return true;
    if (s.ToPChar()==NULL) return true;
    return strcmp(ToPChar(), s.ToPChar()) != 0;
}

bool TShortString::operator != (const char* pChar)
{
    if ((ToPChar()==NULL) && (pChar==NULL)) return false;
    if (ToPChar()==NULL) return true;
    if (pChar==NULL) return true;
    return strcmp(ToPChar(), pChar) != 0;
}


int TShortString::MaxLength()
{
    return SHORTSTRINGLENGTH;
}
