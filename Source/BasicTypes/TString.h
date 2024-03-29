/*
 * Persistence Library / Basic types / TString
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

#ifndef TSTRING___H
#define TSTRING___H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 *  TString is a class for handling strings with both dynamic and static
 *  memory allocation (it depends on constructor arguments).
 *
 *  Both static and dynamic strings can have maximum length 64kB
 *  (embedded systems rarely need to handle strings bigger that 64kB)
 *  
 *  Dynamic allocation always allocates a bit more memory to avoid memory 
 *  fragmentation (e.g. instead of 3 bytes is allocated 16 bytes to prevent 
 *  more additional allocations). 
 *
 *  TString intentionally does not use any virtual methods to avoid
 *  problems when it is used with Atmel AVR.
 *
 */

class TString
{
protected:
    char*          PData;    
    unsigned short DataLen;
    unsigned short DataMax;
    bool           DataStatic;  
	bool           ReadOnly;
        
public:

    // Constructor
    TString();
	TString(const char* pChar, unsigned short pCharLen=0, bool useExistingReference=false, bool readOnly=false);
	TString(const TString &s);

    // Destructor 
    ~TString();

	unsigned short GetBufferSize() const;
	bool  IsBufferStatic() const;

    unsigned short Length() const;

    //setters
    void Clear(bool dontReleaseMemory=false);    
    bool Fill(char c, unsigned short number);	
	bool CopyFrom (const char* AData);
    bool CopyFrom (const char* AData, unsigned short length);
    
    //string operations    
    bool Contains(const char* pChar) const;
    bool Contains(char c) const;
    int  IndexOf(const char* pChar, unsigned short startIndex=0) const;
    int  IndexOf(char c, unsigned short startIndex=0) const;    
	int  LastIndexOf(char c) const;
	int  LastIndexOf(const char* pChar) const;
	char FirstChar() const;
	char LastChar() const;
 
    operator char* ();
    operator const char*() const;
	const char* ToPChar() const;
  
    TString& Trim();
    TString& LowerCase();
    TString& UpperCase();
    TString& Append(char c);
	TString& Append(const char* c);
	TString& Append(const TString& oString);
	TString& Insert(unsigned short index, char c);
	TString& Insert(unsigned short index, const char* s);
	TString& Insert(unsigned short index, const char* s, int length);
	TString& Insert(unsigned short index, const TString& oString);
	TString& Delete(unsigned short index, unsigned short length=1);    
    TString& Replace(char oldChar, char newChar);
    bool SetLength(unsigned short newLength, bool addSpaces=true);
	bool SetCapacity(unsigned short capacity);

	TString& operator = ( const TString& s);
    TString& operator = ( const char* pChar);
    TString  operator + ( const char* pChar);
    TString& operator += ( const char* pChar);
    TString& operator += ( const char c);

    //operators
    bool operator > (const char* pChar) const;
    bool operator < (const char* pChar) const;
    bool operator >= (const char* pChar) const;
    bool operator <= (const char* pChar) const;
    bool operator == (const char* pChar) const;
	bool operator == (const TString& s)  const;
    bool operator != (const char* pChar) const;
    char& operator [] (unsigned short index);
    char& operator [] (int index);

#ifdef STL_STYLE
    typedef char* iterator;
	char* begin();
	char* end();

	int size() const;
	int length() const;
	int max_size() const;
	int resize(int n, char c);
	int capacity() const;
	int reserve(int n);
	void clear();
	bool empty() const;
	char& at(int i);
	char& back();
	char& front();
 	TString& append(const char* str);
    TString& append(const TString& str);
	void push_back(char c);
	TString& assign(const TString& str);
	TString& assign(const TString& str, int position, int length);
	TString& assign(const char* str);
	TString& assign(const char* str, int n);
	TString& assign(int n, char c);
	TString& insert(int position, const TString& str);
	TString& insert(int position, const TString& str, int subpos, int sublen);
	TString& insert(int position, const char* str);
	TString& insert(int position, const char* str, int length);
	TString& insert(int position, int n, char c);
	TString& remove(int position, int length);
	void swap(TString& str);
	void pop_back();
	const char* c_str();
	const char* data();
	int find(const TString& str, int pos=0) const;
	int find(const char* str, int pos=0) const;

#endif

};

#endif

