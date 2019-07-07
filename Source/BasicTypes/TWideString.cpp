/*
 * Persistence Library / Basic types / TWideString
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

#include "TWideString.h"
extern "C"
{
    #include "UTF8.h"
};


TWideString::TWideString()
{
	PData = NULL;
	DataLen = 0;
	DataMax = 0;
	DataStatic = false;
}

TWideString::TWideString (const wchar_t* pChar, unsigned short pCharLen)
{
	PData = NULL;
	DataLen = 0;
	DataMax = 0;
	DataStatic = false;

	CopyFrom(pChar, pCharLen);
}

TWideString::TWideString (const TWideString& oString )
{
	PData = NULL;
	DataLen = 0;
	DataMax = 0;
	DataStatic = false;

	CopyFrom(oString.ToPWChar(), oString.Length());
}


TWideString::~TWideString()
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

unsigned short TWideString::GetBufferSize() const
{
    return DataMax;
}

bool TWideString::IsBufferStatic() const
{
    return DataStatic;
}

void TWideString::Clear(bool dontReleaseMemory)
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

bool TWideString::Fill(wchar_t c, unsigned short number)
{
    if (!SetLength(number, false))
    {
        return false;
    }

    wchar_t* dst = PData;
    for(unsigned short i = DataLen; i>0; i--)
    {
        *dst = c;
        dst++;
    }
    *dst = 0;
    return true;
}

bool TWideString::CopyFrom (const wchar_t* pChar, unsigned short length)
{    
	if (pChar==NULL)
	{
		Clear();
	} else {
        if (length==0)
        {
            length = (unsigned short)wcslen(pChar);
            if (length==0)
            {
                Clear();
                return true;
            }
        }
		if (SetLength(length))
		{
			memcpy(PData,pChar,DataLen*sizeof(wchar_t));
			PData[DataLen] = 0;
            return true;
		}		
        return false;
	}
    return true;
}

bool TWideString::SetLength(unsigned short len, bool addSpaces)
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
			return false;
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
				PData = (wchar_t*) malloc(DataMax*sizeof(wchar_t));
				if (PData==NULL)
				{
					DataLen = 0;
					DataMax = 0;
					return false;
				}
				memset((void*)PData, 0, DataMax*sizeof(wchar_t));
			} else {
				PData = (wchar_t*) realloc(PData,DataMax*sizeof(wchar_t));
				if (PData==NULL)
				{
					DataLen = 0;
					DataMax = 0;
					return false;
				}
			}
		}
	} else {
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

unsigned short TWideString::Length() const
{
	return DataLen;
}

const wchar_t* TWideString::ToPWChar() const
{
	return (const wchar_t*)PData;
}

bool TWideString::Contains(wchar_t c) const
{
	wchar_t buf[2];    
	buf[0] = c;
	buf[1] = 0;
	wchar_t* pChar = (wchar_t*) &buf;
	return Contains(pChar);
}

bool TWideString::Contains(const wchar_t* pChar) const
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
			pCharLen = (int)wcslen(pChar);
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

int TWideString::IndexOf(wchar_t c, unsigned short startIndex) const
{
	wchar_t buf[2];    
	buf[0] = c;
	buf[1] = 0;
    
	wchar_t* pChar = (wchar_t*) &buf;
	return IndexOf(pChar, startIndex);
}

int TWideString::IndexOf(const wchar_t* pChar, unsigned short startIndex) const
{
	unsigned short len = Length();
	unsigned short pCharLen = 0;
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = (unsigned short)wcslen(pChar);
		}
	}

	if (startIndex>=1)
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

int TWideString::LastIndexOf(wchar_t c) const
{
	wchar_t buf[2];    
	buf[0] = c;
	buf[1] = 0;
    
    wchar_t* pChar = (wchar_t*) &buf;
    return IndexOf(pChar);
}

int TWideString::LastIndexOf(const wchar_t* pChar) const
{
    unsigned short len = Length();
    unsigned short pCharLen = 0;
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = (unsigned short)wcslen(pChar);
        }
    }
    int result= -1;
    bool found = false;
    for(int i = len-pCharLen; i>0; i--)
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


TWideString& TWideString::LowerCase()
{
	wchar_t* pp = (wchar_t*) PData;
	for(int i = 0; i<DataLen; i++)
	{
		wchar_t c = *pp;
		if ((c>='A') && (c<='Z'))
		{
			*pp = c + 32;            
		}
		pp++;
	}
	return *this;
}

TWideString& TWideString::UpperCase()
{
	wchar_t* pp = PData;
	for(int i = 0; i<DataLen; i++)
	{
		wchar_t c = *pp;
		if ((c>='a') && (c<='z'))
		{
			*pp = c - 32;
		}
		pp++;
	}
	return *this;
}

TWideString& TWideString::Trim()
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
    wchar_t* src = PData + begin;
    wchar_t* dst = PData;
    for(unsigned short k = newLength; k>0; k--)
    {
        *dst = *src;
        dst++; src++;
    }
    SetLength(newLength, false);
    return *this;
}

TWideString& TWideString::Replace(wchar_t oldChar, wchar_t newChar)
{
	for(int i = 0; i<DataLen; i++)
	{
		if (PData[i] == oldChar)
		{
			PData[i] = newChar;
		}
	}
    return *this;
}

TWideString::operator wchar_t*()
{
	return (wchar_t*)PData;
}

TWideString::operator const wchar_t*()
{
	return (wchar_t*)PData;
}

TWideString& TWideString::Append(wchar_t c)
{
	int oldLength = Length();	
	if (SetLength(oldLength+1))
	{
		PData[oldLength] = c;
	}
	return *this;
}

TWideString& TWideString::Append(const wchar_t* s)
{
	int oldLength = Length();
	int sLength = 0;
	if (s!=NULL) sLength = (int)wcslen(s);
	
	if (SetLength(oldLength+sLength))
	{
		sLength = DataLen - oldLength;
		memcpy(PData+oldLength, s, sLength*sizeof(wchar_t));
	}
	return *this;
}

TWideString& TWideString::Append(TWideString& oString)
{
	int oldLength = Length();
	int sLength = oString.Length();
	if (SetLength(oldLength+oString.Length()))
	{
		sLength = DataLen - oldLength;
		memcpy(PData+oldLength, oString.ToPWChar(), sLength*sizeof(wchar_t));
	}
	return *this;
}

TWideString& TWideString::Insert(unsigned short index, wchar_t c)
{
	if (index>Length()) return *this;

	int oldLength = Length();
	if (SetLength(oldLength+1))
	{
		for(int i = DataLen; i>index; i--)
		{
			PData[i] = PData[i-1];
		}
		PData[index] = c;	
	}
	return *this;
}

TWideString& TWideString::Insert(unsigned short index, const wchar_t* s)
{
	if (index>Length()) return *this;

	int sLength = 0;
	if (s!=NULL) sLength=(int)wcslen(s);

	int oldLength = Length();
	if (SetLength(oldLength+1))
	{
		sLength = DataLen - oldLength;
		for(int i = DataLen; i>index; i--)
		{
			PData[i] = PData[i-sLength];
		}
		memcpy(PData+index, s, sLength*sizeof(wchar_t));
	}
	return *this;
}

TWideString& TWideString::Insert(unsigned short index, TWideString& oString)
{
	return Insert(index, oString.ToPWChar());
}

TWideString& TWideString::Delete(unsigned short index, unsigned short length)
{
    if (((short)index<0) || (index>=Length()))
    {
        return *this;
    }	 
	if ((index+length)>=DataLen)
	{
		length = DataLen - index;
	}
    for(int i = 0; i<DataLen-index; i++)
    {
        PData[index+i] = PData[index+length+i];
    }
	DataLen-=length;
    return *this;
}

TWideString& TWideString::operator = (const TString oString )
{
    if (SetLength(oString.Length()))
	{
		UTF8TextToUnicodeText(oString.ToPChar(), PData, DataLen);    
	}
    return *this;
}

TWideString& TWideString::operator = ( const char* pChar )
{
    if (pChar==NULL)
    {
        Clear();
    } else {
	    if (SetLength((unsigned short)strlen(pChar)+1))
		{
			UTF8TextToUnicodeText(pChar, PData, DataLen);
		}
        DataLen = (unsigned short)wcslen(PData);
    }    
    return *this;
}

TWideString& TWideString::operator = (const TWideString oString )
{
	CopyFrom(oString.ToPWChar(), oString.Length());
	return *this;
}    

TWideString& TWideString::operator = (const wchar_t* pChar)
{
	CopyFrom(pChar);
	return *this;
}    


TWideString TWideString::operator + ( const wchar_t* pChar )
{
	static TWideString result;
    result.CopyFrom(PData, DataLen);
	result+=pChar;
	return result;
}

TWideString TWideString::operator + (TWideString& oString )
{
    static TWideString result;
    result.CopyFrom(PData, DataLen);
	result+=oString;
	return result;
}

TWideString& TWideString::operator += ( const wchar_t* pChar )
{
	int pCharLen = 0;
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = (int)wcslen(pChar);    
		}
	}
	if (pCharLen==0) return *this;        
	unsigned short oldLength = Length();
	unsigned short newLength = oldLength + pCharLen;
	if (newLength<oldLength) newLength=65534;
	if (newLength==65535) newLength=65534;
	if (SetLength(newLength))
	{
		memcpy(&PData[oldLength], pChar, (DataLen-oldLength)*sizeof(wchar_t));
	}
	return *this;
}


TWideString& TWideString::operator += ( TWideString& oString )
{
	const wchar_t* pChar = oString.ToPWChar();
	int pCharLen = 0;
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = (int)wcslen(pChar);    
		}
	}
	if (pCharLen==0) return *this;        
	unsigned short oldLength = Length();
	unsigned short newLength = oldLength + pCharLen;
	if (newLength<oldLength) newLength=65534;
	if (newLength==65535) newLength=65534;
	if (SetLength(newLength))
	{
		memcpy(&PData[oldLength], pChar, (DataLen-oldLength)*sizeof(wchar_t));
	}
	return *this;
}


TWideString& TWideString::operator += ( const wchar_t AChar )
{    
	int length = Length();
	if (SetLength(length+1))
	{
		PData[length] = AChar;
	}
	return *this;
}

bool TWideString::operator > (const wchar_t* pChar) const
{
	if (Length()==0)   return (false);
	if (pChar==NULL)   return (true);

	int pCharLen = 0;
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = (int)wcslen(pChar);
		}
	}

	int minlength;
	if (Length()<pCharLen) minlength=Length(); else minlength=pCharLen;

	int i;
	wchar_t* pp = (wchar_t*)ToPWChar();
	wchar_t* ss = (wchar_t*)pChar; 

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

bool TWideString::operator < (const wchar_t* pChar) const
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
			pCharLen = (int)wcslen(pChar);
		}
	}
	if (Length()<pCharLen) minlength=Length(); else minlength=pCharLen;
	int i;

	wchar_t* pp = (wchar_t*)ToPWChar();
	wchar_t* ss = (wchar_t*)pChar; 

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

bool TWideString::operator >= (TWideString& s) const
{
	if (*this>s) return (true);
	if (*this==s) return (true);
	return false;
}

bool TWideString::operator >= (const wchar_t* pChar) const
{
	if (*this>pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}


bool TWideString::operator <= (TWideString &s) const
{        
	if (*this<s) return (true);
	if (*this==s) return (true);
	return false;
}

bool TWideString::operator <= (const wchar_t* pChar) const
{        
	if (*this<pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}

bool TWideString::operator == (TWideString& s) const
{
	if ((ToPWChar()==NULL) && (s.ToPWChar()==NULL)) return true;
	if (ToPWChar()==NULL) return false;
	if (s.ToPWChar()==NULL) return false;
	return wcscmp(ToPWChar(), s.ToPWChar()) == 0;
}

bool TWideString::operator == (const wchar_t* pChar) const
{
	if ((ToPWChar()==NULL) && (pChar==NULL)) return true;
	if ((ToPWChar()==NULL) && (*pChar==0)) return true;
	if (ToPWChar()==NULL) return false;
	if (pChar==NULL) return false;
	return wcscmp(ToPWChar(), pChar) == 0;
}

bool TWideString::operator != (TWideString& s) const
{
	if ((ToPWChar()==NULL) && (s.ToPWChar()==NULL)) return false;
	if (ToPWChar()==NULL) return true;
	if (s.ToPWChar()==NULL) return true;
	return wcscmp(ToPWChar(), s.ToPWChar()) != 0;
}

bool TWideString::operator != (const wchar_t* pChar) const
{
	if ((ToPWChar()==NULL) && (pChar==NULL)) return false;
	if (ToPWChar()==NULL) return true;
	if (pChar==NULL) return true;
	return wcscmp(ToPWChar(), pChar) != 0;
}

wchar_t& TWideString::operator[] (int index)
{
    static wchar_t tmp = 0;
	if (index<0) return tmp;
	if (index>=Length()) return tmp;
	return PData[index];
}
