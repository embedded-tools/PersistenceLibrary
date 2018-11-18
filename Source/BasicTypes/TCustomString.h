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
 *  Memory is allocated statically as a part of class instance memory.
 *
 *  There is no way how to increase preallocated memory size.
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

    TCustomString(const char* pChar)
        :TString()
	{
		PData = (char*)&Data;
		DataMax = LENGTH+1;
		DataStatic = true;
		CopyFrom(pChar);
	}


    TCustomString(const char* pChar, unsigned short pCharLen)
        :TString()
	{
		PData = (char*)&Data;
		DataMax = LENGTH+1;
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

	TCustomString(const TString& s)
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

    TCustomString<LENGTH>& operator = (const TCustomString<LENGTH>& oString )
	{
		CopyFrom(oString.ToPChar(), oString.Length());
		return *this;    
	}

    TCustomString<LENGTH>& operator = (const TString& oString )
	{
		CopyFrom(oString.ToPChar(), oString.Length());
		return *this;
	}

    TCustomString<LENGTH>& operator = ( const char* pChar)
	{
		CopyFrom(pChar);
		return *this;
	}

	bool operator == (const TCustomString<LENGTH>& s)
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

    bool operator != (const TCustomString<LENGTH>& s)
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

