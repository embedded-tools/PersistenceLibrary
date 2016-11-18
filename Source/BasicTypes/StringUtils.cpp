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

#include "stringutils.h"
#include "tshortstring.h"

unsigned short StrLen(const char* s)
{
	if (s==0) return 0;

	unsigned short result = 0;	
	while(*s!=0)
	{	
		s++;
		result++;		
	}
	return result;
}

short StrPos (const char* substring, const char* string, short startIndex)
{
	bool match;
	short n=-1;

	unsigned short len = StrLen(string);
	unsigned short sublen = StrLen(substring);

	if ((len==0) || (sublen==0)) return -1;
	if (sublen>len) return -1;

	if (startIndex<0) startIndex = 0;

	for (int i=startIndex; i<(len-sublen+1); i++) 
	{
		match=true;
		for (int j=0; j<sublen; j++) 
		{
			if (substring[j]!=string[i+j]) { match=false; break; }
		}
 		if (match==true) 
		{
			n=i;
			break;
		}
	}
	return n;
};

short StrPos (char c, const char* string, short startIndex)
{
	char buf[2];
	buf[0] = c;
	buf[1] = 0;
	char* p = (char*) &buf;
	return StrPos(p, string, startIndex);
}



bool ByteToStr (char Value, char* buffer, unsigned short bufferSize)
{
    if (bufferSize<1)
    {
        return false;
    }
    char* pp = buffer;
    if (Value==-128)
    {
        if (bufferSize<5) return false;
        *pp='-'; pp++;
        *pp='1'; pp++;
        *pp='2'; pp++;
        *pp='8'; pp++;
        *pp=0;
        return true;
    }
    if (Value<0) 
    {
        Value=-Value;
        *pp = '-';
        pp++;
        bufferSize--;
        if (bufferSize==0) return false;
    };
    bool leadingSpaces = true;
    for (short xx=100; xx>0; xx=xx/10)
    {
        if (xx==1) leadingSpaces=false;
        char n=Value/xx; 
        Value-=n*xx;
        if (n==0) 
        {
            if (!leadingSpaces) 
            {                
                *pp = '0';
                pp++;
                bufferSize--;
                if (bufferSize==0) return false;
            }
        } else {
            *pp = (n + '0');
            pp++;
            bufferSize--;
            if (bufferSize==0) return false;

            leadingSpaces = false;
        }
    }
    *pp = 0;
    return true;
}

bool UByteToStr (unsigned char Value, char* buffer, unsigned short bufferSize)
{
    if (bufferSize<1)
    {
        return false;
    }
    char* pp = buffer;
    if (Value==-128)
    {
        if (bufferSize<5) return false;
        *pp='-'; pp++;
        *pp='1'; pp++;
        *pp='2'; pp++;
        *pp='8'; pp++;
        *pp=0;
        return true;
    }
    if (Value<0) 
    {
        Value=-Value;
        *pp = '-';
        pp++;
        bufferSize--;
        if (bufferSize==0) return false;
    };
    bool leadingSpaces = true;
    for (short xx=100; xx>0; xx=xx/10)
    {
        if (xx==1) leadingSpaces=false;
        char n=Value/xx; 
        Value-=n*xx;
        if (n==0) 
        {
            if (!leadingSpaces) 
            {                
                *pp = '0';
                pp++;
                bufferSize--;
                if (bufferSize==0) return false;
            }
        } else {
            *pp = (n + '0');
            pp++;
            bufferSize--;
            if (bufferSize==0) return false;

            leadingSpaces = false;
        }
    }
    *pp = 0;
    return true;
}

bool ShortIntToStr (short Value, char* buffer, unsigned short bufferSize)
{    
	if (bufferSize<1)
	{
		return false;
	}
	char* pp = buffer;
    if (Value==-32768)
    {
        if (bufferSize<7) return false;
        *pp='-'; pp++;
        *pp='3'; pp++;
        *pp='2'; pp++;
        *pp='7'; pp++;
        *pp='6'; pp++;
        *pp='8'; pp++;
        *pp=0;
        return true;
    }
	if (Value<0) 
	{
		Value=-Value;
		*pp = '-';
		pp++;
		bufferSize--;
		if (bufferSize==0) return false;
	};
	bool leadingSpaces = true;
	for (short xx=10000; xx>0; xx=xx/10)
	{
		if (xx==1) leadingSpaces=false;
		char n=Value/xx; 
		Value-=n*xx;
		if (n==0) 
		{
			if (!leadingSpaces) 
			{                
				*pp = '0';
				pp++;
				bufferSize--;
				if (bufferSize==0) return false;
			}
		} else {
			*pp = (n + '0');
			pp++;
			bufferSize--;
			if (bufferSize==0) return false;

			leadingSpaces = false;
		}
	}
	*pp = 0;
	return true;
};


bool UShortIntToStr (unsigned short Value, char* buffer, unsigned short bufferSize)
{    
    if (bufferSize<1)
    {
        return false;
    }
    char* pp = buffer;
    bool leadingSpaces = true;
    for (unsigned short xx=10000; xx>0; xx=xx/10)
    {
        if (xx==1) leadingSpaces=false;
        char n=Value/xx; 
        Value-=n*xx;
        if (n==0) 
        {
            if (!leadingSpaces) 
            {                
                *pp = '0';
                pp++;
                bufferSize--;
                if (bufferSize==0) return false;
            }
        } else {
            *pp = (n + '0');
            pp++;
            bufferSize--;
            if (bufferSize==0) return false;

            leadingSpaces = false;
        }
    }
    *pp = 0;
    return true;
};


bool LongIntToStr (long Value, char* buffer, unsigned short bufferSize)
{    
    if (bufferSize<1)
    {
        return false;
    }
    char* pp = buffer;
    if (Value==(-2147483647-1))
    {
        if (bufferSize<12) return false;
        *pp='-'; pp++;
        *pp='2'; pp++;
        *pp='1'; pp++;
        *pp='4'; pp++;
        *pp='7'; pp++;
        *pp='4'; pp++;
        *pp='8'; pp++;
        *pp='3'; pp++;
        *pp='6'; pp++;
        *pp='4'; pp++;
        *pp='8'; pp++;
        *pp=0;
        return true;
    }
    if (Value<0) 
    {
        Value=-Value;
        *pp = '-';
        pp++;
        bufferSize--;
        if (bufferSize==0) return false;
    };
    bool leadingSpaces = true;
    for (long xx=1000000000; xx>0; xx=xx/10)
    {
        if (xx==1) leadingSpaces=false;
        char n=(char)(Value/xx); 
        Value=Value-n*xx;
        if (n==0) 
        {
            if (!leadingSpaces) 
            {                
                *pp = '0';
                pp++;
                bufferSize--;
                if (bufferSize==0) return false;
            }
        } else {
            *pp = (n + '0');
            pp++;
            bufferSize--;
            if (bufferSize==0) return false;

            leadingSpaces = false;
        }
    }
    *pp = 0;
    return true;
};

bool ULongIntToStr (unsigned long Value, char* buffer, unsigned short bufferSize)
{    
    if (bufferSize<1)
    {
        return false;
    }
    char* pp = buffer;
    bool leadingSpaces = true;
    for (unsigned long xx=1000000000; xx>0; xx=xx/10)
    {
        if (xx==1) leadingSpaces=false;
        char n=(char)(Value/xx); 
        Value=Value-n*xx;
        if (n==0) 
        {
            if (!leadingSpaces) 
            {                
                *pp = '0';
                pp++;
                bufferSize--;
                if (bufferSize==0) return false;
            }
        } else {
            *pp = (n + '0');
            pp++;
            bufferSize--;
            if (bufferSize==0) return false;

            leadingSpaces = false;
        }
    }
    *pp = 0;
    return true;
};

bool UByteToHex (unsigned short Value, char* buffer, unsigned short bufferSize)
{    
	if (bufferSize<3)
	{
		return false;
	}
	buffer[0] = ((Value >> 4) & 15) + '0'; if (buffer[0]>'9') buffer[0]+=7;
	buffer[1] = ((Value)      & 15) + '0'; if (buffer[1]>'9') buffer[1]+=7;
	buffer[2] = 0;
	return true;
};


bool UShortIntToHex (unsigned short Value, char* buffer, unsigned short bufferSize)
{    
    if (bufferSize<5)
    {
        return false;
    }
	buffer[0] = ((Value >> 12) & 15) + '0'; if (buffer[0]>'9') buffer[0]+=7;
	buffer[1] = ((Value >> 8)  & 15) + '0'; if (buffer[1]>'9') buffer[1]+=7;
	buffer[2] = ((Value >> 4)  & 15) + '0'; if (buffer[2]>'9') buffer[2]+=7;
	buffer[3] = ((Value)       & 15) + '0'; if (buffer[3]>'9') buffer[3]+=7;
	buffer[4] = 0;
    return true;
};

bool ULongIntToHex (unsigned long Value, char* buffer, unsigned short bufferSize)
{    
    if (bufferSize<9)
    {
        return false;
    }
	buffer[0] = (char)(((Value >> 28) & 15) + '0'); if (buffer[0]>'9') buffer[0]+=7;
	buffer[1] = (char)(((Value >> 24) & 15) + '0'); if (buffer[1]>'9') buffer[1]+=7;
	buffer[2] = (char)(((Value >> 20) & 15) + '0'); if (buffer[2]>'9') buffer[2]+=7;
	buffer[3] = (char)(((Value >> 16) & 15) + '0'); if (buffer[3]>'9') buffer[3]+=7;
	buffer[4] = (char)(((Value >> 12) & 15) + '0'); if (buffer[4]>'9') buffer[4]+=7;
	buffer[5] = (char)(((Value >> 8)  & 15) + '0'); if (buffer[5]>'9') buffer[5]+=7;
	buffer[6] = (char)(((Value >> 4)  & 15) + '0'); if (buffer[6]>'9') buffer[6]+=7;
	buffer[7] = (char)(((Value)       & 15) + '0'); if (buffer[7]>'9') buffer[7]+=7;
	buffer[8] = 0;

    return true;
};

short StrToShortInt (const char* pChar, unsigned short pCharLength)
{
    return StrToShortIntDef(pChar, 0, pCharLength);
}

short StrToShortIntDef (const char* pChar, short default_value, unsigned short pCharLength)
{
    short temp=0;
    bool minus=false;

    if (pCharLength==0) pCharLength = StrLen(pChar);
    for (unsigned short i=0; i<pCharLength; i++) 
    {
        char c = pChar[i];
        if (c==' ') continue;
        if (c=='-') { minus=true; continue; }
        if ((c<'0')||(c>'9')) 
        { 
            return default_value;
        }
        temp *= 10;

        c &= 15;
        if (c==0) continue;

        if (temp>=0)
            temp += c;
        else
            temp -= c;

        if (minus)
        {
            temp = -temp;
            minus = false;
        }

    }
    return temp;
}

unsigned short StrToUShortInt (const char* pChar, unsigned short pCharLength)
{
    return StrToUShortIntDef(pChar, 0, pCharLength);
}

unsigned short StrToUShortIntDef (const char* pChar, unsigned short default_value, unsigned short pCharLength)
{
    unsigned short temp=0;

    if (pCharLength==0) pCharLength = StrLen(pChar);
    for (unsigned char i=0; i<(unsigned char)pCharLength; i++)
    {
        char c = pChar[i];
        if (c==' ') continue;
        if (c=='-') return default_value;
        if ((c<'0')||(c>'9')) 
        { 
            return default_value;
        }
        temp *= 10;

        c &= 15;
        temp += c;
    }
    return temp;
}

unsigned short HexToUShortInt(const char* pChar, unsigned short pCharLength)
{
    return HexToUShortIntDef(pChar, 0, pCharLength);
}

unsigned short HexToUShortIntDef(const char* pChar, unsigned short def, unsigned short pCharLength)
{
    unsigned short res = def;
    char c;
    if (pChar==NULL)
    {
        return def;
    }
	if (pCharLength==0) pCharLength = StrLen(pChar);
	if (pChar[1]=='x') 
	{
		pChar+=2;
		pCharLength-=2;
	}    
    for(unsigned char i=0; i<(unsigned char)pCharLength; i++)
    {
        c = pChar[i];
        if ( (c>='a') && (c<='z') ) c-=32;
        if (c<'0') return def;
        c-='0';
        if (c>9) c-=7;
        if (c>15)
        { 
            return def;
        }
        res *= 16;
        res += c;
    }
    return res;

}


long StrToLongInt (const char* pChar, unsigned short pCharLength)
{
    return (StrToLongIntDef (pChar, 0, pCharLength));
};

long StrToLongIntDef (const char* pChar, long default_value, unsigned short pCharLength)
{
    long temp=0;
    bool minus=false;

    if (pCharLength==0) pCharLength = StrLen(pChar);
    for (unsigned char i=0; i<(unsigned char)pCharLength; i++)
    {
        char c = pChar[i];
        if (c==' ') continue;
        if (c=='-') { minus=true; continue; }
        if ((c<'0')||(c>'9')) 
        { 
            return default_value;
        }
        temp *= 10;

        c &= 15;
        if (c==0) continue;
          
        if (temp>=0)
            temp += c;
        else
            temp -= c;

        if (minus)
        {
            temp = -temp;
            minus = false;
        }

    }
    return temp;
};

unsigned long StrToULongInt (const char* pChar, unsigned short pCharLength)
{
    return (StrToULongIntDef (pChar, 0, pCharLength));
};

unsigned long StrToULongIntDef (const char* pChar, unsigned long default_value, unsigned short pCharLength)
{
    unsigned long temp=0;

    if (pCharLength==0) pCharLength = StrLen(pChar);
    for (unsigned char i=0; i<(unsigned char)pCharLength; i++) 
    {
        char c = pChar[i];
        if (c==' ') continue;
        if (c=='-') return default_value;
        if ((c<'0')||(c>'9')) 
        { 
            return default_value;
        }
        temp *= 10;

        c &= 15;
        temp += c;
    }
    return temp;
};


unsigned long HexToULongInt(const char* pChar, unsigned short pCharLength)
{
    return HexToULongIntDef(pChar, 0, pCharLength);
}

unsigned long HexToULongIntDef(const char* pChar, unsigned long def, unsigned short pCharLength)
{
    unsigned long res = def;
    char c;
    if (pChar==NULL)
    {
        return def;    
	}
	if (pCharLength==0) pCharLength = StrLen(pChar);
	if (pChar[1]=='x') 
	{
		pChar+=2;
		pCharLength-=2;
	}    
    for(unsigned char i=0; i<(unsigned char)pCharLength; i++)
    {
        c = pChar[i];
        if ( (c>='a') && (c<='z') ) c-=32;
        if (c<'0') return def;
        c-='0';
        if (c>9) c-=7;
        if (c>15)
        { 
            return def;
        }
        res *= 16;
        res += c;
    }
    return res;
}

bool ByteArrayToStr (char* byteArray, unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator)
{
    for(unsigned short i = 1; i<=byteArrayLength; i++)
    {
        bool res = ByteToStr(*byteArray, buffer, bufferSize);
		if (!res) return false;
        byteArray++;

        while(*buffer!=0)
        {
            buffer++;
            bufferSize--;
            if (bufferSize==0) break;
        }
        if (bufferSize==0) return false;

        if (i!=byteArrayLength)
        {
            buffer[0]=separator;
            buffer++;
            bufferSize--;
            if (bufferSize==0) return false;
        }
    }
    return true;
}

bool UByteArrayToStr (unsigned char* byteArray, unsigned short byteArrayLength, char* buffer, unsigned short bufferSize, char separator)
{
    for(unsigned short i = 1; i<=byteArrayLength; i++)
    {
        bool res = UByteToStr(*byteArray, buffer, bufferSize);
		if (!res) return false;
        byteArray++;
        while(*buffer!=0)
        {
            buffer++;
            bufferSize--;
            if (bufferSize==0) break;
        }
        if (bufferSize==0) return false;

        if (i!=byteArrayLength)
        {
            buffer[0]=separator;
            buffer++;
            bufferSize--;
            if (bufferSize==0) return false;
        }
    }
    return true;
}

bool ShortIntArrayToStr (short* shortArray, unsigned short shortArrayLength, char* buffer, unsigned short bufferSize, char separator)
{
    for(unsigned short i = 1; i<=shortArrayLength; i++)
    {
        bool res = ShortIntToStr(*shortArray, buffer, bufferSize);
		if (!res) return false;
        shortArray++;
        while(*buffer!=0)
        {
            buffer++;
            bufferSize--;
            if (bufferSize==0) break;
        }
        if (bufferSize==0) return false;

        if (i!=shortArrayLength)
        {
            buffer[0]=separator;
            buffer++;
            bufferSize--;
            if (bufferSize==0) return false;
        }
    }
    return true;
}

bool UShortIntArrayToStr(unsigned short* ushortArray,unsigned short ushortArrayLength, char* buffer, unsigned short bufferSize, char separator)
{
    for(unsigned short i = 1; i<=ushortArrayLength; i++)
    {
        bool res = UShortIntToStr(*ushortArray, buffer, bufferSize);
		if (!res) return false;
        ushortArray++;
        while(*buffer!=0)
        {
            buffer++;
            bufferSize--;
            if (bufferSize==0) break;
        }
        if (bufferSize==0) return false;

        if (i!=ushortArrayLength)
        {
            buffer[0]=separator;
            buffer++;
            bufferSize--;
            if (bufferSize==0) return false;
        }        
    }
    return true;
}

bool LongIntArrayToStr  (long* longArray,  unsigned short longArrayLength, char* buffer, unsigned short bufferSize, char separator)
{
    for(unsigned short i = 1; i<=longArrayLength; i++)
    {
        bool res = LongIntToStr(*longArray, buffer, bufferSize);
		if (!res) return false;
        longArray++;
        while(*buffer!=0)
        {
            buffer++;
            bufferSize--;
            if (bufferSize==0) break;
        }
        if (bufferSize==0) return false;

        if (i!=longArrayLength)
        {
            buffer[0]=separator;
            buffer++;
            bufferSize--;
            if (bufferSize==0) return false;
        }
    }
    return true;
}

bool ULongIntArrayToStr (unsigned long* ulongArray,  unsigned short ulongArrayLength, char* buffer, unsigned short bufferSize, char separator)
{
    for(unsigned short i = 1; i<=ulongArrayLength; i++)
    {        
        bool res = ULongIntToStr(*ulongArray, buffer, bufferSize);
		if (!res) return false;
        ulongArray++;
        while(*buffer!=0)
        {
            buffer++;
            bufferSize--;
            if (bufferSize==0) break;
        }
        if (bufferSize==0) return false;

        if (i!=ulongArrayLength)
        {
            buffer[0]=separator;
            buffer++;
            bufferSize--;
            if (bufferSize==0) return false;
        }
    }
    return true;
}

unsigned short  StrToByteArray (char* string, char* array,  unsigned short maxArraySize, char separator)
{
	unsigned short arrayLength = 0;
	while(*string!=0)
	{
		unsigned short numberLength = 0;

		char* pp = string;
		while(*pp!=0)
		{
			if (*pp==separator) break;
			numberLength++;
			pp++;
		}
		*array = (unsigned char)StrToShortIntDef(string, 0, numberLength);
		array++;
		arrayLength++;
		if(arrayLength==maxArraySize) break;

		string+=numberLength;
		if (*string==separator) string++;        
	}    
	return arrayLength;    
}


unsigned short  StrToUByteArray (char* string, unsigned char* array,  unsigned short maxArraySize, char separator)
{
    unsigned short arrayLength = 0;
    while(*string!=0)
    {
        unsigned short numberLength = 0;

        char* pp = string;
        while(*pp!=0)
        {
            if (*pp==separator) break;
            numberLength++;
            pp++;
        }
        *array = (unsigned char)StrToUShortIntDef(string, 0, numberLength);
        array++;
        arrayLength++;
        if(arrayLength==maxArraySize) break;

        string+=numberLength;
        if (*string==separator) string++;        
    }    
    return arrayLength;    
}

unsigned short  StrToShortIntArray (char* string, short* array,          unsigned short maxArraySize, char separator)
{
    unsigned short arrayLength = 0;
    while(*string!=0)
    {
        unsigned short numberLength = 0;

        char* pp = string;
        while(*pp!=0)
        {
            if (*pp==separator) break;
            numberLength++;
            pp++;
        }
        *array = StrToShortIntDef(string, 0, numberLength);
        array++;
        arrayLength++;
        if(arrayLength==maxArraySize) break;

        string+=numberLength;
        if (*string==separator) string++;        
    }    
    return arrayLength;    
}

unsigned short  StrToUShortIntArray(char* string, unsigned short* array, unsigned short maxArraySize, char separator)
{
    unsigned short arrayLength = 0;
    while(*string!=0)
    {
        unsigned short numberLength = 0;

        char* pp = string;
        while(*pp!=0)
        {
            if (*pp==separator) break;
            numberLength++;
            pp++;
        }
        *array = StrToUShortIntDef(string, 0, numberLength);
        array++;
        arrayLength++;
        if(arrayLength==maxArraySize) break;

        string+=numberLength;
        if (*string==separator) string++;        
    }    
    return arrayLength;    
}

unsigned short  StrToLongIntArray  (char* string, long* array, unsigned short maxArraySize, char separator)
{
    unsigned short arrayLength = 0;
    while(*string!=0)
    {
        unsigned short numberLength = 0;

        char* pp = string;
        while(*pp!=0)
        {
            if (*pp==separator) break;
            numberLength++;
            pp++;
        }
        *array = StrToLongIntDef(string, 0, numberLength);
        array++;
        arrayLength++;
        if(arrayLength==maxArraySize) break;

        string+=numberLength;
        if (*string==separator) string++;        
    }    
    return arrayLength;    
}

unsigned short StrToULongIntArray (char* string, unsigned long* array,  unsigned short maxArraySize, char separator)
{
    unsigned short arrayLength = 0;
    while(*string!=0)
    {
        unsigned short numberLength = 0;

        char* pp = string;
        while(*pp!=0)
        {
            if (*pp==separator) break;
            numberLength++;
            pp++;
        }
        *array = StrToULongIntDef(string, 0, numberLength);
        array++;
        arrayLength++;
        if(arrayLength==maxArraySize) break;

        string+=numberLength;
        if (*string==separator) string++;        
    }    
    return arrayLength;    
}

