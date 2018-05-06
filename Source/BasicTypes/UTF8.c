#include "UTF8.h"
#include <string.h>


int UTF8TextToUnicodeText(const char* utf8text, wchar_t* unicodeBuffer, unsigned long unicodeMaxCharacterCount)
{
	unsigned int   utf8textLength = strlen(utf8text);
	unsigned long  unicodeCharacterCount = 0;
	unsigned char* pChar = (unsigned char*)utf8text;
    unsigned char  c;
    wchar_t        uc;
    int            i, nBytes;
	while(1)
	{
		c   = *pChar;
		if (c==0) break;

		uc = c;
		nBytes = 1;
		if (c>=0xF0) nBytes=4; else
		if (c>=0xE0) nBytes=3; else
		if (c>=0xC0) nBytes=2;
		if (nBytes>1)
		{		
			uc = 0;
			for(i = 0; i<nBytes; i++)
			{
				uc<<=6;
				c = *pChar; pChar++;
				if (c==0) break;
				uc+= c & 0x3F;								
			}
		} else {
			pChar++;
		}
		unicodeBuffer[unicodeCharacterCount]=uc;
		unicodeCharacterCount++;
		if (unicodeCharacterCount==unicodeMaxCharacterCount) return 0;
	}
	unicodeBuffer[unicodeCharacterCount]=0;
	return 1;
}

int UnicodeTextToUTF8Text(const wchar_t* unicodeText, char* utf8buffer, unsigned long utf8BufferByteSize)
{
	unsigned long unicodeTextLength = wcslen(unicodeText);
	unsigned long utf8Length = 0;
    unsigned long i;

	for(i = 0; i<unicodeTextLength; i++)
	{
		wchar_t wChar = unicodeText[i];
		if (wChar>=0x800)
		{
			utf8buffer[utf8Length] = 0xE0+(wChar / 4096);
			utf8Length++; if (utf8Length==utf8BufferByteSize) return 0;
			utf8buffer[utf8Length] = 0x80+(wChar / 64 % 64);
			utf8Length++; if (utf8Length==utf8BufferByteSize) return 0;
			utf8buffer[utf8Length] = 0x80+(wChar % 64);
			utf8Length++; if (utf8Length==utf8BufferByteSize) return 0;
		} else 
		if (wChar>=0xC0)
		{
			utf8buffer[utf8Length] = 0xC0+(wChar / 64 % 64);
			utf8Length++; if (utf8Length==utf8BufferByteSize) return 0;
			utf8buffer[utf8Length] = 0x80+(wChar % 64);
			utf8Length++; if (utf8Length==utf8BufferByteSize) return 0;
		} else {
			utf8buffer[utf8Length] = 0xC0+(wChar / 64 % 64);
			utf8Length++; if (utf8Length==utf8BufferByteSize) return 0;
		}
	}
	utf8buffer[utf8Length] = 0;
	return 1;
}


