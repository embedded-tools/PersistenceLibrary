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

#include "tstring.h"

TString::TString()
: PData(NULL), 
  DataLen(0), 
  DataMax(0), 
  DataStatic(false),
  ReadOnly(false)
{

}
         
TString::TString(const char* pChar, unsigned short pCharLen, bool useExistingReference)
: PData(NULL), 
  DataLen(0), 
  DataMax(0), 
  DataStatic(false),
  ReadOnly(false)
{
	if (useExistingReference)
	{
		PData = (char*)pChar;
		DataLen = pCharLen;
		DataMax = pCharLen;
		DataStatic = true;
	} else {
		if (pChar)
		{
			if (pCharLen==0)
			{
				pCharLen = strlen(pChar);
			}
			CopyFrom(pChar, pCharLen);	
		}
	}	
}

TString::TString(const TString &s)
: PData(NULL), 
  DataLen(0), 
  DataMax(0), 
  DataStatic(false),
  ReadOnly(false)
{
	CopyFrom(s.ToPChar(), s.Length());
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

unsigned short TString::GetBufferSize() const
{
	return DataMax;
}

bool TString::IsBufferStatic() const
{
	return DataStatic;
}

unsigned short TString::Length() const
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
        SetLength(0, false);
    }
}

bool TString::Fill(char c, unsigned short number)
{
    if (!SetLength(number, false))
	{
		return false;
	}
	if (number==0) 
	{
		return true;
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

bool TString::CopyFrom (const char* s)
{
	if (s==NULL) return false;

	return CopyFrom(s, strlen(s));
}

bool TString::CopyFrom (const char* s, unsigned short length)
{    
    if (s==NULL) 
    {
        Clear();
        return false;
    }
        
    if (!SetLength(length, false))
	{
		return false;
	}
	
	if (PData)
	{
		char* src = (char*)s;
		char* dst = PData;
		memcpy(PData, s, DataLen);
		PData[DataLen] = 0;
	}
    return true;
}

const char* TString::ToPChar() const
{
    return (const char*)PData;
}

bool TString::Contains(char c) const
{
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    char* pChar = (char*) &buf;
    return Contains(pChar);
}

bool TString::Contains(const char* pChar) const
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

int TString::IndexOf(char c, unsigned short startIndex) const
{
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    char* pChar = (char*) &buf;
    return IndexOf(pChar, startIndex);
}

int TString::IndexOf(const char* pChar, unsigned short startIndex) const
{
    unsigned short len = Length();
    unsigned char pCharLen = 0;
	if (pChar==NULL)
	{
		return -1;
	}
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
        startIndex = 0;
    }
    if (startIndex>=len)
    {
        return -1;
    }
    
    int result= -1;
    bool found = false;
    for(int i = startIndex; i<=len-pCharLen; i++)
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

int TString::LastIndexOf(char c) const
{
	char buf[2];
	buf[0] = c;
	buf[1] = 0;
	char* pChar = (char*) &buf;
	return LastIndexOf(pChar);
}

int TString::LastIndexOf(const char* pChar) const
{
	unsigned short len = Length();
	unsigned char pCharLen = 0;

	if (pChar==NULL)
	{
		return -1;
	}
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = strlen(pChar);
		}
	}

	int result= -1;
	bool found = false;
	for(int i = 0; i<=len-pCharLen; i++)
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
		}
	}
	return result;
}

char TString::FirstChar() const
{
	if (PData==NULL) return 0;
	if (DataLen==0) return 0;
	return PData[0];
}

char TString::LastChar() const
{
	if (PData==NULL) return 0;
	if (DataLen==0) return 0;
	return PData[DataLen-1];
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
	if (ReadOnly)
	{
		return *this;
	}
    unsigned short len = Length();
    if (len==0)
    {
        return *this;
    }

    unsigned short begin = 0;
    for(unsigned short i = 0;  i<len; i++)
    {
        begin = i;
        if (PData[i]>' ') break;
    }
    unsigned short end = len;
    if (len!=0)
    {
        for(unsigned short j = len-1; j>0; j--)
        {
            end = j;
            if (PData[j]>' ') break;
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
    SetLength(newLength, false);
    return *this;
}

TString& TString::LowerCase()
{
    if (PData==NULL)
    {
        return *this;
    }
	if (ReadOnly)
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
	if (ReadOnly)
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
	if (SetLength(oldLength+1, false))
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
	
	if (SetLength(oldLength+sLength, false))
	{
		memcpy(PData+oldLength, s, DataLen - oldLength);
	}
	return *this;
}

TString& TString::Append(const TString& oString)
{
	int oldLength = Length();	
	if (SetLength(oldLength+oString.Length(), false))
	{
		memcpy(PData+oldLength, oString.ToPChar(), DataLen - oldLength);
	}
	return *this;
}

TString& TString::Insert(unsigned short index, char c)
{
	if (index>Length()) return *this;
	
	int oldLength = Length();
	if (SetLength(oldLength+1, false))
	{
		for(int i = DataLen; i>index; i--)
		{
			PData[i] = PData[i-1];
		}
		PData[index] = c;	
	}
	return *this;
}

TString& TString::Insert(unsigned short index, const char* s)
{
	if (index>Length()) return *this;

	int sLength = 0;
	if (s!=NULL) sLength=strlen(s);
	
	int oldLength = Length();
	int newLength = oldLength+sLength;	
	if (SetLength(newLength, false))
	{		
		sLength = DataLen - oldLength;
		for(int i = DataLen; i>index; i--)
		{
			PData[i] = PData[i-sLength];
		}
		memcpy(PData+index, s, sLength);
	}
	return *this;
}

TString& TString::Insert(unsigned short index, const char* s, int length)
{
	if (index>Length()) return *this;
	
	int oldLength = Length();
	int newLength = oldLength+length;
	if (SetLength(newLength, false))
	{
		length = DataLen - oldLength;
		for(int i = newLength; i>index; i--)
		{
			PData[i] = PData[i-length];
		}
		memcpy(PData+index, s, length);
	}
	return *this;
}


TString& TString::Insert(unsigned short index, const TString& oString)
{
	return *this;
}

TString& TString::Delete(unsigned short index, unsigned short length)
{
	if (length<1) return *this;
	if (PData==NULL) return *this;

	if ((index+length)>DataLen)
	{
		length = DataLen - index;
	}
	memcpy(PData+index, PData+index+length, DataLen - index - length + 1);
	DataLen-=length;

	return *this;
}

TString& TString::Replace(char oldChar, char newChar)
{
    int len = Length();
    if (len==0) return *this;
	if (PData==NULL) return *this;

    for(unsigned short i = 0; i<len; i++)
    {
        if (PData[i] == oldChar)
        {
            PData[i] = newChar;
        }
    }
    return *this;
}

bool TString::SetLength(unsigned short len, bool addSpaces)
{
    if (len==65535) len=65534;
    
    if (!DataStatic)
    {
        if (len==0)
        {
			SetCapacity(0);
			return false;
        }    
        if (DataMax<=len)
        {
            //allocated memory is not so big,
            //therefore it must be reallocated    
            unsigned short newDataMax = DataMax;
            while (newDataMax<=len)
            {            
                if (newDataMax>=61440)
                {
                    newDataMax = 65535;
                    break;
                } else
                if (newDataMax>=4096)
                {
                    newDataMax += 4096;
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
			SetCapacity(newDataMax);
			if (PData==NULL)
			{
				return false;
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
    return true;
}

bool TString::SetCapacity(unsigned short capacity)
{
    if (PData==NULL)
    {
		if (capacity==0) 
		{
			return false;
		}
        PData = (char*) malloc(capacity);
        if (PData==NULL)
        {
            DataLen = 0;
            DataMax = 0;
            return false;
        }
		DataMax = capacity;
		return true;
    } 
	if (capacity==0)
	{
		free(PData);
		PData = NULL;
        DataLen = 0;
        DataMax = 0;
        return false;
	}
    PData = (char*) realloc(PData,capacity);
    if (PData==NULL)
    {
        DataLen = 0;
        DataMax = 0;
        return false;
    }
	if (DataLen>DataMax)
	{
		DataLen=DataMax-1;
	}
	DataMax = capacity;
	return true;
}

TString& TString::operator = (const TString& oString )
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
    if (SetLength(newLength, false))
	{
		memcpy(&PData[oldLength], pChar, DataLen-oldLength);
	}
    return *this;
}

TString& TString::operator += ( const char AChar )
{    
    int length = Length();
	int newLength = length + 1;
    if (SetLength(newLength, false))
    {
        PData[length] = AChar;
        PData[newLength] = 0;
        DataLen = newLength;
    }
    return *this;
}

bool TString::operator > (const char* pChar) const
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

bool TString::operator < (const char* pChar) const
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

bool TString::operator >= (const char* pChar) const
{
	if (*this>pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}

bool TString::operator <= (const char* pChar) const
{        
	if (*this<pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}

bool TString::operator == (const char* pChar) const
{
    if ((ToPChar()==NULL) && (pChar==NULL)) return true;
    if ((ToPChar()==NULL) && (*pChar==0)) return true;
	if (ToPChar()==NULL) return false;
	if (pChar==NULL) return false;
    return strcmp(ToPChar(), pChar) == 0;
}

bool TString::operator != (const char* pChar) const
{
    if ((ToPChar()==NULL) && (pChar==NULL)) return false;
	if (ToPChar()==NULL) return true;
	if (pChar==NULL) return true;
    return strcmp(ToPChar(), pChar) != 0;
}

char& TString::operator[] (unsigned short index)
{
	  static char tmp = 0;
      if (index<0) return tmp;
      if (index>=Length()) return tmp;
      return PData[index];
}

char& TString::operator[] (int index)
{
	static char tmp = 0;
    if (index<0) return tmp;
    if (index>=Length()) return tmp;
    return PData[index];
}

#ifdef STL_STYLE

char* TString::begin()
{
	if (PData==NULL)
	{	
		SetCapacity(16);
	}
	return PData;
}
char* TString::end()
{
	return PData + DataLen;
}

int TString::size() const
{
	return DataLen;
}

int TString::length() const
{
	return DataLen;
}

int TString::max_size() const
{
	return 65535;
}

int TString::resize(int n, char c)
{
	Fill(c, n);
    return DataLen;
}

int TString::capacity() const
{
	return DataMax;
}

int TString::reserve(int n)
{
	return SetCapacity(n);
}

void TString::clear()
{
	SetLength(0);
}

bool TString::empty() const
{
	return DataLen==0;
}

char& TString::at(int i)
{
	static char c = 0;
	if (i<0) return c;
	if (i>=DataLen) return c;
	return PData[i];
}

char& TString::back()
{
	static char c = 0;
	if (DataLen<1) return c;
	return PData[DataLen-1];
}

char& TString::front()
{
	static char c = 0;
	if (DataLen<1) return c;
	return PData[0];
}

TString& TString::append(const char* str)
{
	return Append(str);
}

TString& TString::append(const TString& str)
{
	return Append(str);
}

void TString::push_back(char c)
{
	int newLength = DataLen+1;
	if (SetLength(newLength))
	{
		PData[DataLen-1] = c;
	}
}

TString& TString::assign(const TString& str)
{
	CopyFrom(str.PData, str.Length());
	return *this; 
}

TString& TString::assign(const TString& str, int position, int length)
{
	CopyFrom(str.PData + position, length);
	return *this; 
}

TString& TString::assign(const char* str)
{
	if (str)
	{
		CopyFrom(str, strlen(str));
	} else {
		SetLength(0);
	}
	return *this; 
}

TString& TString::assign(const char* str, int n)
{
	CopyFrom(str, n);
	return *this; 
}

TString& TString::assign(int n, char c)
{
	Fill(c, n);
	return *this; 
}

TString& TString::insert(int position, const TString& str)
{
	Insert(position, str);
	return *this; 
}

TString& TString::insert(int position, const TString& str, int subpos, int sublen)
{
	Insert(position, str.PData + subpos, sublen);
	return *this; 
}

TString& TString::insert(int position, const char* str)
{
	Insert(position, str);
	return *this; 
}

TString& TString::insert(int position, const char* str, int length)
{
	Insert(position, str);
	return *this; 
}

TString& TString::remove(int position, int length)
{
	Delete(position, length);
	return *this;
}

void TString::swap(TString& str)
{
	TString tmp;
	tmp = str;
	str = *this;
	*this = tmp;
}

void TString::pop_back()
{
	if (DataLen>0)
	{
		SetLength(DataLen-1);
	}
}

const char* TString::c_str()
{
	return PData;
}

const char* TString::data()
{
	return PData;
}

int TString::find(const TString& str, int pos) const
{
	return IndexOf(str.PData, pos);
}

int TString::find(const char* str, int pos) const
{
	return IndexOf(str, pos);
}

#endif