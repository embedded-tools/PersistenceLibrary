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

#ifndef TENCODING___H
#define TENCODING___H

/**
 *  Conversion from UTF8 (most common 8bit encoding) to Unicode and vice versa. 
 *  Warning - method UnicodeToASCII should not be used until you are sure why you 
 *  need it (it can cause some information loss)
 */

bool UnicodeToASCII(const short* unicodeText, char* pbASCIIbuffer, unsigned short& cbASCIIbuffer);
bool UnicodeToUTF8(const short* unicodeText, char* utf8buffer, unsigned short& utf8BufferSize);
bool ASCIIToUnicode(const char* asciiText, short* unicodeBuffer, unsigned short& unicodeBufferSize);
bool UTF8ToUnicode(const char* utf8text, short* unicodeBuffer, unsigned short& unicodeBufferSize);

#endif