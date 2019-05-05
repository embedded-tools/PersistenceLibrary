/*
 * Persistence Library / Basic types / TWideString
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

#ifndef TWIDESTRING_H
#define TWIDESTRING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TString.h"

/**
 *  TWideString is a class for handling 16 bit unicode strings with maximum 
 *  length 65535 characters. 
 *  
 *  Class can use both static or dynamic memory allocation depending
 *  on constructor arguments.
 *
 *  Dynamic allocation always allocates a bit more memory to avoid memory 
 *  fragmentation (e.g. instead of 3 bytes is allocated 16 bytes to prevent 
 *  more additional allocations). 
 */

class TWideString
{
protected:
	wchar_t*       PData;    
	unsigned short DataLen;
	unsigned short DataMax;
	bool           DataStatic;    

public:

	// Constructor
	TWideString();
	TWideString(const wchar_t* pChar, unsigned short pCharLen=0);
	TWideString(const TWideString& oString );


	// Destructor 
	~TWideString();

    unsigned short GetBufferSize() const;
    bool  IsBufferStatic() const;

    unsigned short Length() const;

	//setters
    void Clear(bool dontReleaseMemory=false);    
    bool Fill(wchar_t c, unsigned short number);	
    bool CopyFrom (const wchar_t* AData, unsigned short length=0); 

	//string operations    
    //string operations    
    bool Contains(const wchar_t* pChar) const;
    bool Contains(wchar_t c) const;
    int  IndexOf(wchar_t c, unsigned short startIndex=0) const;    
    int  IndexOf(const wchar_t* pChar, unsigned short startIndex=0) const;    
    int  LastIndexOf(wchar_t c) const;
    int  LastIndexOf(const wchar_t* pChar) const;
    wchar_t FirstChar() const;
    wchar_t LastChar() const;

    operator wchar_t* ();
    operator const wchar_t*();
    const wchar_t* ToPWChar() const;

    TWideString& Trim();
	TWideString& LowerCase();
	TWideString& UpperCase();	
  
    TWideString& Append(wchar_t c);
    TWideString& Append(const wchar_t* c);
    TWideString& Append(TWideString& oString);
    TWideString& Insert(unsigned short index, wchar_t c);
    TWideString& Insert(unsigned short index, const wchar_t* s);
    TWideString& Insert(unsigned short index, TWideString& oString);
    TWideString& Delete(unsigned short index, unsigned short length=1);    
    TWideString& Replace(wchar_t oldChar, wchar_t newChar);
    bool         SetLength(unsigned short newLength, bool addSpaces=true);

	TWideString& operator = ( const TString oString );
    TWideString& operator = ( const char* pChar );
	TWideString& operator = ( const TWideString oString );
	TWideString& operator = ( const wchar_t* pChar);      
	TWideString  operator + ( TWideString& oString);
	TWideString  operator + ( const wchar_t* pChar);
	TWideString& operator  += ( TWideString& oString);
	TWideString& operator  += ( const wchar_t* pChar);
	TWideString& operator  += ( const wchar_t c);

	//operators
	bool operator > (const wchar_t* pChar) const;
	bool operator < (const wchar_t* pChar) const;
	bool operator >= (TWideString& s) const;
	bool operator >= (const wchar_t* pChar) const;
	bool operator <= (TWideString& s) const;
	bool operator <= (const wchar_t* pChar) const;
	bool operator == (TWideString& s) const;
	bool operator == (const wchar_t* pChar) const;
	bool operator != (TWideString& s) const;
	bool operator != (const wchar_t* pChar) const;
	wchar_t& operator [] (int index);

};

#endif

