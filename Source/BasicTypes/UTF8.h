#ifndef UNICODE2UTF8___H
#define UNICODE2UTF8___H

#include <tchar.h>

int UnicodeTextToUTF8Text(const wchar_t* unicodeText, char* utf8buffer, unsigned long utf8BufferSize);
int UTF8TextToUnicodeText(const char* utf8text, wchar_t* unicodeBuffer, unsigned long unicodeBufferSize);

#endif