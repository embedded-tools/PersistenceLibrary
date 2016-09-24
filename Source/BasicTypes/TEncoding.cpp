/*
 * Persistence Library / Basic types / TEncoding
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

#include "TWideString.h"

bool UnicodeToASCII(const short* unicodeText, char* pbASCIIbuffer, unsigned short& cbASCIIbuffer)
{
	char* target = pbASCIIbuffer;

	for(const short* src = unicodeText; *src!=0; src++)
	{
		wchar_t cc = *src;
		if (cc>0xFF) cc = '?';

		if (cbASCIIbuffer==0) return false;
		*target = (unsigned char)cc;
		target++;

		cbASCIIbuffer--;
	}
	if (cbASCIIbuffer==0) return false;
	*target = 0;
    return true;
}

bool UnicodeToUTF8(const short* unicodeText, char* utf8buffer, unsigned short& utf8BufferSize)
{
    unsigned long utf8Length = 0;
	unsigned char* dst = (unsigned char*)utf8buffer;
    for(const short* src = unicodeText; *src!=0; src++)
    {
        short wChar = *src;
        if (wChar>=0x800)
        {
            *dst = 0xE0+(wChar / 4096); dst++;
            utf8Length++; if (utf8Length==utf8BufferSize) return false;
            *dst = 0x80+(wChar / 64 % 64); dst++;
            utf8Length++; if (utf8Length==utf8BufferSize) return false;
            *dst = 0x80+(wChar % 64); dst++;
            utf8Length++; if (utf8Length==utf8BufferSize) return false;
        } else 
            if (wChar>=0xC0)
            {
                *dst = 0xC0+(wChar / 64 % 64); dst++;
                utf8Length++; if (utf8Length==utf8BufferSize) return false;
                *dst = 0x80+(wChar % 64); dst++;
                utf8Length++; if (utf8Length==utf8BufferSize) return false;
            } else {
                *dst = (unsigned char)wChar; dst++;
                utf8Length++; if (utf8Length==utf8BufferSize) return false;
            }
    }
    *dst = 0;
    return true;
}

bool ASCIIToUnicode(const char* asciiText, short* unicodeBuffer, unsigned short& unicodeBufferSize)
{
	short* target = unicodeBuffer;

	for(const char* src = asciiText; *src!=0; src++)
	{
		unsigned char cc = *src;

		if (unicodeBufferSize==0) return false;
		*target = cc;
		target++;

		unicodeBufferSize--;
	}

	if (unicodeBufferSize==0) return false;
	*target = 0;
	return true;
}

bool UTF8ToUnicode(const char* utf8text, short* unicodeBuffer, unsigned short& unicodeBufferSize)
{
    unsigned int utf8textLength = strlen(utf8text);

    unsigned long unicodeLength = 0;
    unsigned char* pChar = (unsigned char*)utf8text;
    while(true)
    {
        unsigned char c   = *pChar;
        if (c==0) break;

        short uc = c;
        int nBytes = 1;
        if (c>=0xF0) nBytes=4; else
            if (c>=0xE0) nBytes=3; else
                if (c>=0xC0) nBytes=2;
        if (nBytes>1)
        {		
            uc = 0;
            for(int i = 0; i<nBytes; i++)
            {
                uc<<=6;
                c = *pChar; pChar++;
                if (c==0) break;
                uc+= c & 0x3F;								
            }
        } else {
            pChar++;
        }
        unicodeBuffer[unicodeLength]=uc;
        unicodeLength++;
        if (unicodeLength==unicodeBufferSize) return false;
    }
    unicodeBuffer[unicodeLength]=0;
    return true;
}
