/*
 * Persistence Library / Basic types / TWideString
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

#ifndef TWIDESTRING_H
#define TWIDESTRING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tstring.h"

/**
 *  TString is a class for handling unicode strings with maximum 
 *  length 65535 characters. Embedded systems rarely need to handle strings 
 *  bigger that 65535 characters, therefore such limit was implemented.
 *  
 *  Class uses dynamic string allocation, however it always allocates
 *  a bit more memory (e.g. instead of 3 bytes is allocated 16 bytes to 
 *  prevent more additional allocations and deleting character causes no
 *  memory reallocation at all). This way class decreases memory 
 *  fragmentation.
 *
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
	TWideString(TWideString& oString );

	unsigned short GetDataMax();
	unsigned short GetDataStatic();

	// Destructor 
	~TWideString();

	//setters
	void Set(const wchar_t* pChar, unsigned short pCharLen=0);
	unsigned short SetLength(unsigned short len);
	void Clear(bool dontReleaseMemory=false);
	unsigned short Length();

	void CopyFrom (const wchar_t* AData, unsigned short startIndex, unsigned short length);


	//string operations    
	bool Contains(const wchar_t* pChar);
	bool Contains(wchar_t c);
	int IndexOf(const wchar_t* pChar, unsigned short startIndex=0);
	int IndexOf(wchar_t c, unsigned short startIndex=0);

	virtual TWideString Clone();
	TWideString ToLower();
	TWideString ToUpper();
	TWideString TrimStart();
	TWideString Trim();
	TWideString TrimEnd();    
	TWideString SubString(unsigned short startIndex, unsigned short size = 65535);
	void Replace(wchar_t oldChar, wchar_t newChar);
	TWideString Replace(const wchar_t* oldString, const wchar_t* newString);

	operator wchar_t* ();
	operator const wchar_t*();
	const wchar_t* ToPWChar();

	TWideString& Add(wchar_t c);
	TWideString& Add(const wchar_t* c);
	TWideString& Add(TWideString& oString);
	TWideString& Insert(unsigned short index, wchar_t c);
	TWideString& Insert(unsigned short index, const wchar_t* s);
	TWideString& Insert(unsigned short index, TWideString& oString);
	TWideString& Delete(unsigned short index, unsigned short length=1);

	TWideString& operator = ( TString& oString );
    TWideString& operator = ( const char* pChar );
	TWideString& operator = ( TWideString& oString );
	TWideString& operator = ( const wchar_t* pChar);      
	TWideString operator  + ( TWideString& oString);
	TWideString operator  + ( const wchar_t* pChar);
	TWideString& operator  += ( TWideString& oString);
	TWideString& operator  += ( const wchar_t* pChar);
	TWideString& operator  += ( const wchar_t c);

	//operators
	bool operator > (const wchar_t* pChar);
	bool operator < (const wchar_t* pChar);
	bool operator >= (TWideString& s);
	bool operator >= (const wchar_t* pChar);
	bool operator <= (TWideString& s);
	bool operator <= (const wchar_t* pChar);
	bool operator == (TWideString& s);
	bool operator == (const wchar_t* pChar);
	bool operator != (TWideString& s);
	bool operator != (const wchar_t* pChar);
	wchar_t& operator [] (int index);

};

#endif

