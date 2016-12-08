/*
 * Persistence Library / Basic types /StringUtils
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

#ifndef STRINGUTILS___H
#define STRINGUTILS___H

/**
 *  String helper methods. All of them is possible to replace by printf syntaxes,
 *  but sometimes you can not afford to include printf functions (e.g. if ROM is not 
 *  big enough).
 *
 *  If you have plenty of RAM and ROM, always use printf or sprintf instead!!!
 */

unsigned short StrLen(const char* s);
short StrPos (const char* substring, const char* string, short startIndex=0);
short StrPos (char c, const char* string, short startIndex=0);

/*
void (*StringResultCallback)(const char* subString, unsigned short subStringLength);
void Split(const char* string, char separator, StringResultCallback callback);
void Trim (const char* string, bool trimLeft, bool trimRight, StringResultCallback callback);
*/


bool ByteToStr (short Value, char* buffer, unsigned short bufferSize);
bool UByteToStr (short Value, char* buffer, unsigned short bufferSize);
bool UShortIntToStr (unsigned short Value, char* buffer, unsigned short bufferSize);
bool ShortIntToStr (short Value, char* buffer, unsigned short bufferSize);
bool UShortIntToStr (unsigned short Value, char* buffer, unsigned short bufferSize);
bool LongIntToStr (long value, char* buffer, unsigned short bufferSize);
bool ULongIntToStr (unsigned long value, char* buffer, unsigned short bufferSize);
bool UByteToHex (unsigned short Value, char* buffer, unsigned short bufferSize);
bool UShortIntToHex (unsigned short Value, char* buffer, unsigned short bufferSize);
bool ULongIntToHex (unsigned long Value, char* buffer, unsigned short bufferSize);

short          StrToShortInt (const char* pChar, unsigned short pCharLength=0);
short          StrToShortIntDef (const char* pChar, short default_value, unsigned short pCharLength = 0);
unsigned short StrToUShortInt (const char* pChar, unsigned short pCharLength=0);
unsigned short StrToUShortIntDef (const char* pChar, unsigned short default_value, unsigned short pCharLength = 0);
unsigned short HexToUShortInt(const char* pChar, unsigned short pCharLength=0);
unsigned short HexToUShortIntDef(const char* pChar, unsigned short def, unsigned short pCharLength = 0);
long           StrToLongInt (const char* pChar, unsigned short pCharLength=0);
long           StrToLongIntDef (const char* pChar, long default_value, unsigned short pCharLength = 0);
unsigned long  StrToULongInt (const char* pChar, unsigned short pCharLength=0);
unsigned long  StrToULongIntDef (const char* pChar, unsigned long default_value, unsigned short pCharLength = 0);
unsigned long  HexToULongInt(const char* pChar, unsigned short pCharLength=0);
unsigned long  HexToULongIntDef(const char* pChar, unsigned long def, unsigned short pCharLength = 0);

bool ByteArrayToStr     (char* byteArray,  unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator=',');
bool UByteArrayToStr    (unsigned char* byteArray,  unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator=',');
bool ShortIntArrayToStr (short* shortArray, unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator=',');
bool UShortIntArrayToStr(unsigned short* ushortArray,unsigned short ushortArrayLength, char* buffer, unsigned short bufferSize, char separator=',');
bool LongIntArrayToStr  (long*  intArray,  unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator=',');
bool ULongIntArrayToStr (unsigned long* uintArray,  unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator=',');

unsigned short  StrToByteArray     (const char* string,  char* array,           unsigned short maxArraySize, char separator=',');
unsigned short  StrToUByteArray    (const char* string, unsigned char* array,  unsigned short maxArraySize, char separator=',');
unsigned short  StrToShortIntArray (const char* string, short* array,          unsigned short maxArraySize, char separator=',');
unsigned short  StrToUShortIntArray(const char* string, unsigned short* array, unsigned short maxArraySize, char separator=',');
unsigned short  StrToLongIntArray  (const char* string, long* array,           unsigned short maxArraySize, char separator=',');
unsigned short  StrToULongIntArray (const char* string, unsigned long* array,  unsigned short maxArraySize, char separator=',');

#endif
