/*
 * Persistence Library / Basic types / TString
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

#ifndef TSTRING___H
#define TSTRING___H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 *  TString is a class for handling strings with maximum length 64kB.
 *  Embedded systems rarely need to handle strings bigger that 64kB,
 *  therefore such limit was implemented.
 *  
 *  Class uses dynamic string allocation, however it always allocates
 *  a bit more memory (e.g. instead of 3 bytes is allocated 16 bytes to 
 *  prevent more additional allocations and deleting character causes no
 *  memory reallocation at all). This way class decreases memory 
 *  fragmentation.
 *
 */

class TString
{
protected:
    char*          PData;    
    unsigned short DataLen;
    unsigned short DataMax;
    bool           DataStatic;    

        
public:

    // Constructor
    TString();
    TString(const char* pChar, unsigned short pCharLen=0);
	TString(TString &s);

    // Destructor 
    ~TString();

	unsigned short GetBufferSize();
	bool  IsBufferStatic();

    unsigned short Length();

    //setters
    void Clear(bool dontReleaseMemory=false);    
    bool Fill(char c, unsigned short number);	
    bool CopyFrom (const char* AData, unsigned short length=0);    
    
    //string operations    
    bool Contains(const char* pChar);
    bool Contains(char c);
    int  IndexOf(const char* pChar, unsigned short startIndex=0);
    int  IndexOf(char c, unsigned short startIndex=0);    
	int  LastIndexOf(char c);
	int  LastIndexOf(const char* pChar);
	char FirstChar();
	char LastChar();
 
    operator char* ();
    operator const char*();
	const char* ToPChar();
  
    TString& Trim();
    TString& LowerCase();
    TString& UpperCase();
	TString& Append(char c);
	TString& Append(const char* c);
	TString& Append(TString& oString);
	TString& Insert(unsigned short index, char c);
	TString& Insert(unsigned short index, const char* s);
	TString& Insert(unsigned short index, TString& oString);
	TString& Delete(unsigned short index, unsigned short length=1);    
    TString& Replace(char oldChar, char newChar);
    unsigned short SetLength(unsigned short newLength, bool addSpaces=true);

	TString& operator = ( TString &s);
    TString& operator = ( const char* pChar);
    TString  operator + ( const char* pChar);
    TString& operator += ( const char* pChar);
    TString& operator += ( const char c);

    //operators
    bool operator > (const char* pChar);
    bool operator < (const char* pChar);
    bool operator >= (const char* pChar);
    bool operator <= (const char* pChar);
    bool operator == (const char* pChar);
    bool operator != (const char* pChar);
    char operator [] (unsigned short index);
    char operator [] (int index);

};

#endif

