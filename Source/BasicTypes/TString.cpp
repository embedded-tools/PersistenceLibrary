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

#include "tstring.h"

TString::TString()
    : PData(NULL), DataLen(0), DataMax(0), DataStatic(false)
{

}

TString::TString (const char* pChar, unsigned short pCharLen)
    : PData(NULL), DataLen(0), DataMax(0), DataStatic(false)
{
	CopyFrom(pChar, 0, pCharLen);
}

TString::TString(TString &s)
	: PData(NULL), DataLen(0), DataMax(0), DataStatic(false)
{
	CopyFrom(s.ToPChar(), 0, s.Length());
}

TString::~TString()
{
    if (!DataStatic)
    {
        if (PData)
        {
            free(PData);
        }
    }
	PData=NULL;
}

unsigned short TString::GetBufferSize()
{
	return DataMax;
}

bool TString::IsBufferStatic()
{
	return DataStatic;
}

unsigned short TString::Length()
{
    return DataLen;
}

void TString::Clear(bool dontReleaseMemory)
{
    if (dontReleaseMemory)
    {
        PData = NULL;
        DataLen = 0;
        DataMax = 0;
        DataStatic = false;
    } else {
        SetLength(0);
    }
}

bool TString::Fill(char c, unsigned short number)
{
    SetLength(number);

    if (number!=DataLen)
    {
        return false;
    }
    
    char* dst = PData;
    for(unsigned short i = DataLen; i>0; i--)
    {
        *dst = c;
        dst++;
    }
    *dst = 0;
    return true;
}

bool TString::CopyFrom (const char* s, unsigned short startIndex, unsigned short length)
{    
    if (s==NULL) 
    {
        Clear();
        return false;
    }
       
    unsigned short slen = strlen(s);
    if (startIndex>=slen)
    {
        Clear();
        return false;
    }
    if (length==0)
    {
        length = slen-startIndex;
        if (length==0)
        {
            Clear();
            return false;
        }
    }    
    if (startIndex+length>slen)
    {
        length = slen-startIndex;
    }

    SetLength(length);

    char* src = (char*)s + startIndex;
    char* dst = PData;
    for(unsigned short k = DataLen; k>0; k--)
    {
        *dst = *src; dst++; src++;
    }
    *dst = 0;
    return true;
}

const char* TString::ToPChar()
{
    return (const char*)PData;
}

bool TString::Contains(char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    char* pChar = (char*) &buf;
    return Contains(pChar);
}

bool TString::Contains(const char* pChar)
{
    if (PData==NULL)
    {
        return false;
    }
    int len = Length();
    if (len==0)
    {
        return false;
    }
    int pCharLen = 0;
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = strlen(pChar);
        }
    }
    
    bool found = false;
    for(int i = 0; i<len-pCharLen+1; i++)
    {
        found = true;
        for (int j = 0; j<pCharLen; j++)
        {
            if (PData[j+i]!=pChar[j])
            {
                found=false;
                break;
            }
        }
        if (found)
        {
            break;
        }
    }
    return found;
}

int TString::IndexOf(char c, unsigned short startIndex)
{
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    char* pChar = (char*) &buf;
    return IndexOf(pChar, startIndex);
}

int TString::IndexOf(const char* pChar, unsigned short startIndex)
{
    unsigned short len = Length();
    unsigned char pCharLen = 0;
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = strlen(pChar);
        }
    }

    if (startIndex<0)
    {
        return -1;
    }
    if (startIndex>=pCharLen)
    {
        return -1;
    }
    
    int result= -1;
    bool found = false;
    for(int i = startIndex; i<len-pCharLen; i++)
    {
        found = true;
        for (int j = 0; j<pCharLen; j++)
        {
            if (PData[i+j]!=pChar[j])
            {
                found=false;
                break;
            }
        }
        if (found)
        {
            result = i;
            break;
        }
    }
    return result;
}

TString::operator char*()
{
    return (char*)PData;
}

TString::operator const char*()
{
    return (char*)PData;
}

TString& TString::Trim()
{
    unsigned short len = Length();
    if (len==0)
    {
        return *this;
    }

    unsigned short begin = 0;
    for(unsigned short i = 0;  i<len; i++)
    {
        begin = i;
        if (PData[i]!=32) break;
    }
    unsigned short end = len;
    if (len!=0)
    {
        for(unsigned short j = len-1; j>0; j--)
        {
            end = j;
            if (PData[j]!=32) break;
        }        
    }
    if (begin>end)
    {
        Clear();
        return *this;
    }
    unsigned short newLength = end - begin + 1;
    char* src = PData + begin;
    char* dst = PData;
    for(unsigned short k = newLength; k>0; k--)
    {
        *dst = *src;
        dst++; src++;
    }
    SetLength(newLength);
    return *this;
}

TString& TString::LowerCase()
{
    if (PData==NULL)
    {
        return *this;
    }
    char* pp = (char*)PData;
    for(int i = DataLen; i!=0; i--)
    {
        char c = *pp;
        if ((c>='A') && (c<='Z'))
        {
            *pp = c + 32;
        }
        pp++;
    }
    return *this;
}

TString& TString::UpperCase()
{
    if (PData==NULL)
    {
        return *this;
    }
    char* pp = (char*)PData;
    for(int i = DataLen; i!=0; i--)
    {
        char c = *pp;
        if ((c>='a') && (c<='z'))
        {
            *pp = c - 32;
        }
        pp++;
    }
    return *this;
}

TString& TString::Append(char c)
{
	int oldLength = Length();
	SetLength(oldLength+1);
	if (Length()!=oldLength)
	{
		PData[oldLength] = c;
	}
	return *this;
}

TString& TString::Append(const char* s)
{
	int oldLength = Length();
	int sLength = 0;
	if (s!=NULL) sLength = strlen(s);

	SetLength(oldLength+sLength);
	if (Length()!=oldLength)
	{
		memcpy(PData+oldLength, s, sLength);
	}
	return *this;
}

TString& TString::Append(TString& oString)
{
	int oldLength = Length();
	SetLength(oldLength+oString.Length());
	if (Length()!=oldLength)
	{
		memcpy(PData+oldLength, oString.ToPChar(), oString.Length());
	}
	return *this;
}

TString& TString::Insert(unsigned short index, char c)
{
	if (index>Length()) return *this;
	
	int oldLength = Length();
	int newLength = SetLength(oldLength+1);

	for(int i = newLength; i>index; i--)
	{
		PData[i] = PData[i-1];
	}
	PData[index] = c;	
	return *this;
}

TString& TString::Insert(unsigned short index, const char* s)
{
	if (index>Length()) return *this;

	int sLength = 0;
	if (s!=NULL) sLength=strlen(s);
	
	int oldLength = Length();
	int newLength = SetLength(oldLength+1);

	for(int i = newLength; i>index; i--)
	{
		PData[i] = PData[i-sLength];
	}
	memcpy(PData+index, s, sLength);
	return *this;
}

TString& TString::Insert(unsigned short index, TString& oString)
{
	return *this;
}

TString& TString::Delete(unsigned short index, unsigned short length)
{
	if (length<1) return *this;

	int oldLength = Length();
	if ((index+length)>oldLength)
	{
		length = oldLength - index;
	}
	for(int i = index; i<oldLength-length; i--)
	{
		PData[i] = PData[i];
	}		
	return *this;
}

TString& TString::Replace(char oldChar, char newChar)
{
    int len = Length();
    if (len==0) return *this;

    for(unsigned short i = 0; i<len; i++)
    {
        if (PData[i] == oldChar)
        {
            PData[i] = newChar;
        }
    }
    return *this;
}

unsigned short TString::SetLength(unsigned short len, bool addSpaces)
{
    if (len==65535) len=65534;
    
    if (!DataStatic)
    {
        if (len==0)
        {
            if (PData!=NULL)
            {
                free(PData);
            }
            PData=NULL;
            DataLen = 0;
            DataMax = 0;
            return 0;
        }    
        if (DataMax<=len)
        {
            //allocated memory is not so big,
            //therefore it must be reallocated    
            unsigned short newDataMax = DataMax;
            while (newDataMax<=len)
            {            
                if (newDataMax>=64000)
                {
                    newDataMax = 65535;
                    break;
                } else
                    if (newDataMax>=1024)
                    {
                        newDataMax += 1024;
                    } else 
                        if (newDataMax>=128)
                        {
                            newDataMax += 128;
                        } else {
                            newDataMax += 16;
                        }				
            }            
            DataMax = newDataMax;
            
            if (PData==NULL)
            {
                PData = (char*) malloc(DataMax);
                if (PData==NULL)
                {
                    DataLen = 0;
                    DataMax = 0;
                    return 0;
                }
                memset((void*)PData, 0, DataMax);
            } else {
                PData = (char*) realloc(PData,DataMax);
                if (PData==NULL)
                {
                    DataLen = 0;
                    DataMax = 0;
                    return 0;
                }
            }
        }
    } else {
        //no reallocation if string is preallocated statically.
        //in this case just sets new string length
        if (len>=DataMax)
        {
            len = DataMax-1;
        }
    }
    if (addSpaces)
    {
        for(int i = DataLen; i<len; i++)
        {
            //adds spaces at the string end
            PData[i] = ' ';
        }
    }

    PData[len]=0;   
    DataLen = len;
    return len;
}



TString& TString::operator = (TString& oString )
{
    CopyFrom(oString.ToPChar());
    return *this;
}    

TString& TString::operator = (const char* pChar)
{
    CopyFrom(pChar);
    return *this;
}    


TString TString::operator + ( const char* pChar )
{
    TString result = *this;
    result+=pChar;
    return result;
}

TString& TString::operator += ( const char* pChar )
{
    int pCharLen = 0;
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = strlen(pChar);    
        }
    }
    if (pCharLen==0) return *this;        
    unsigned short oldLength = Length();
    unsigned short newLength = oldLength + pCharLen;
    if (newLength<oldLength) newLength=65534;
    if (newLength==65535) newLength=65534;
    newLength = SetLength(newLength);        
    if (newLength>oldLength)
	{
		memcpy(&PData[oldLength], pChar, newLength-oldLength);
	}
    return *this;
}

TString& TString::operator += ( const char AChar )
{    
    int length = Length();
    int newLength = SetLength(length+1);
    if (newLength>0)
    {
        if (length!=newLength)
        {
            PData[length] = AChar;
            PData[newLength] = 0;
            DataLen = newLength;
        }
    }
    return *this;
}

bool TString::operator > (const char* pChar)
{
    if (Length()==0)   return (false);
    if (pChar==NULL)   return (true);

    int pCharLen = 0;
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = strlen(pChar);
        }
    }

    int minlength;
    if (Length()<pCharLen) minlength=Length(); else minlength=pCharLen;

    int i;
    char* pp = (char*)ToPChar();
    char* ss = (char*)pChar; 
		
    for (i=0; i<minlength; i++) 
    {
	    if (*pp>*ss) return (true);
	    if (*pp<*ss) return (false);
	    pp++; ss++;
    }	
    if (Length()==pCharLen) return (false);
    if (Length()>pCharLen)  return (true);
    return (false);
}

bool TString::operator < (const char* pChar)
{
	int minlength;
	if (Length()==0)   return (true);
	if (pChar==NULL)   return (false);
	
    int pCharLen=0;
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = strlen(pChar);
        }
    }
	if (Length()<pCharLen) minlength=Length(); else minlength=pCharLen;
	int i;

	char* pp = (char*)ToPChar();
	char* ss = (char*)pChar; 
		
	for (i=0; i<minlength; i++) 
    {
		if (*pp<*ss) return (true);
		if (*pp>*ss) return (false);
		pp++; ss++;
	}	
	if (Length()==pCharLen) return (false);
	if (Length()<pCharLen)  return (true);
	return (false);

}

bool TString::operator >= (const char* pChar)
{
	if (*this>pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}

bool TString::operator <= (const char* pChar)
{        
	if (*this<pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}

bool TString::operator == (const char* pChar)
{
    if ((ToPChar()==NULL) && (pChar==NULL)) return true;
    if ((ToPChar()==NULL) && (*pChar==0)) return true;
	if (ToPChar()==NULL) return false;
	if (pChar==NULL) return false;
    return strcmp(ToPChar(), pChar) == 0;
}

bool TString::operator != (const char* pChar)
{
    if ((ToPChar()==NULL) && (pChar==NULL)) return false;
	if (ToPChar()==NULL) return true;
	if (pChar==NULL) return true;
    return strcmp(ToPChar(), pChar) != 0;
}

char TString::operator[] (unsigned short index)
{
      if (index<0) return 0;
      if (index>=Length()) return 0;
      return PData[index];
}

char TString::operator[] (int index)
{
    if (index<0) return 0;
    if (index>=Length()) return 0;
    return PData[index];
}
