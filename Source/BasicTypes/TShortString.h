/*
 * Persistence Library / Basic types / TShortString 
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

#ifndef TSHORTSTRING___H
#define TSHORTSTRING___H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tstring.h"

#define SHORTSTRINGLENGTH 40
//40 characters is usually enough to store any name, surname, address, city, etc.

/**
 *  TShortString a TCustomString with predefined length (similar to TShortString
 *  in borland delphi).
 *  
 *  TShortString prevents memory fragmentation, all string data are held in static
 *  char array with predefined size.
 */

class TShortString : public TString
{
private:
    unsigned char  Data[SHORTSTRINGLENGTH+1];  //one more byte for terminal zero

public:
    TShortString()
	{
		PData = (char*)&Data;
		DataLen = 0;
		DataMax = SHORTSTRINGLENGTH+1;
		DataStatic = true;
		Clear();
	}

    TShortString(const char* pChar, unsigned short pCharLen=0)
		:TString()
	{
		PData = (char*)&Data;
		DataLen = 0;
		DataMax = SHORTSTRINGLENGTH+1;
		DataStatic = true;
    	CopyFrom(pChar, pCharLen);    
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

    TShortString& operator = (const TShortString& oString )
    {
        CopyFrom(oString.ToPChar());
        return *this;
    }

    TShortString& operator = (const TString& oString )
	{
		CopyFrom(oString.ToPChar());
		return *this;
	}

    TShortString& operator = ( const char* pChar)
	{
		CopyFrom(pChar);
		return *this;
	}

	bool operator == (TShortString& s)
	{
		if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return true;
		if (ToPChar()==NULL) return false;
		if (s.ToPChar()==NULL) return false;
		return strcmp(ToPChar(), s.ToPChar()) == 0;
	}

	bool operator == (const char* pChar)
	{
	    if ((ToPChar()==NULL) && (pChar==NULL)) return true;
	    if ((ToPChar()==NULL) && (*pChar==0)) return true;
		if (ToPChar()==NULL) return false;
		if (pChar==NULL) return false;
	    return strcmp(ToPChar(), pChar) == 0;
	}

    bool operator != (TShortString& s)
    {
        if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return false;
	    if (ToPChar()==NULL) return true;
	    if (s.ToPChar()==NULL) return true;
        return strcmp(ToPChar(), s.ToPChar()) != 0;
    }

    bool operator != (const char* pChar)
    {
        if ((ToPChar()==NULL) && (pChar==NULL)) return false;
	    if (ToPChar()==NULL) return true;
	    if (pChar==NULL) return true;
        return strcmp(ToPChar(), pChar) != 0;
    }

   
    int MaxLength()
    {
        return SHORTSTRINGLENGTH-1;
    }

};

#endif
