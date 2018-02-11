/*
 * Persistence Library / Basic types / TCustomString
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

#ifndef TCUSTOMSTRING____H
#define TCUSTOMSTRING____H

#include "tstring.h"

/**
 *  TCustomString is a string that stores string data in static char array with predefined length.
 *  
 *  There are no memory reallocations, therefore it prevents memory fragmentation on systems with
 *  very small RAM (e.g. 32kB or less)
 *
 *  TCustomString intentionally does not use any virtual methods to avoid problems when it is used with Atmel AVR.
 */

template<int LENGTH>
class TCustomString : public TString
{
protected:
    unsigned char  Data[LENGTH+1];  

public:
    TCustomString()
        :TString()
	{
        Data[0] = 0;
        PData = (char*)&Data;
		DataMax = LENGTH+1;
		DataStatic = true;
	}

    TCustomString(const char* pChar, unsigned short pCharLen=0)
        :TString()
	{
		PData = (char*)&Data;
		DataMax = LENGTH+1;
		DataStatic = true;
		CopyFrom(pChar, pCharLen);
	}

	TCustomString(TString& s)
		:TString()
	{
		PData = (char*)&Data;
		DataMax = LENGTH+1;
		DataStatic = true;
		CopyFrom(s.ToPChar(), s.Length());
	}

    TCustomString(const TCustomString& s)
        :TString()
    {
        PData = (char*)&Data;
        DataMax = LENGTH+1;
        DataStatic = true;
        CopyFrom(s.ToPChar(), s.Length());
    }

    TCustomString<LENGTH>& operator = ( TCustomString<LENGTH>& oString )
	{
		CopyFrom(oString.ToPChar());
		return *this;    
	}

    TCustomString<LENGTH>& operator = ( TString& oString )
	{
		CopyFrom(oString.ToPChar());
		return *this;
	}

    TCustomString<LENGTH>& operator = ( const char* pChar)
	{
		CopyFrom(pChar);
		return *this;
	}

	bool operator == (TCustomString<LENGTH>& s)
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

    bool operator != (TCustomString<LENGTH>& s)
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
        return LENGTH;
    }

};

#endif

