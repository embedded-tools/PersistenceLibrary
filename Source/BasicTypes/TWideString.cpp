/*
 * Persistence Library / Basic types / TWideString
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

	CopyFrom(pChar, 0, pCharLen);
}

TWideString::TWideString (TWideString& oString )
{
	PData = NULL;
	DataLen = 0;
	DataMax = 0;
	DataStatic = false;

	CopyFrom(oString.ToPWChar(), 0, oString.Length());
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

unsigned short TWideString::GetDataMax()
{
	return DataMax;
}

unsigned short TWideString::GetDataStatic()
{
	return DataStatic;
}

void TWideString::CopyFrom (const wchar_t* pChar, unsigned short startIndex, unsigned short length)
{    
	if (pChar==NULL)
	{
		Clear();
	} else {
		unsigned short pCharLen = wcslen(pChar);
		if (startIndex>pCharLen)
		{
			return;
		}            
		if (length==0)
		{
			if (pChar!=NULL)
			{
				length = pCharLen - startIndex;
			}
		}
		unsigned short newLength = SetLength(length);
		if (newLength>0)
		{
			memcpy(PData,pChar+startIndex,newLength*2);
			PData[newLength] = 0;
		}
		DataLen = newLength;            
	}
}


void TWideString::Set(const wchar_t* pChar, unsigned short pCharLen)
{
	CopyFrom(pChar, 0, pCharLen);
}

unsigned short TWideString::SetLength(unsigned short len)
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
				PData = (wchar_t*) malloc(DataMax*2);
				if (PData==NULL)
				{
					DataLen = 0;
					DataMax = 0;
					return 0;
				}
				memset((void*)PData, 0, DataMax);
			} else {
				PData = (wchar_t*) realloc(PData,DataMax*2);
				if (PData==NULL)
				{
					DataLen = 0;
					DataMax = 0;
					return 0;
				}
			}
		}
	} else {
		if (len>=DataMax)
		{
			len = DataMax-1;
		}
	}
	PData[len]=0;   
	DataLen = len;
	return len;
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

unsigned short TWideString::Length()
{
	return DataLen;
}

const wchar_t* TWideString::ToPWChar()
{
	return (const wchar_t*)PData;
}

bool TWideString::Contains(wchar_t c)
{
	wchar_t buf[2];
	buf[0] = c;
	buf[1] = 0;
	wchar_t* pChar = (wchar_t*) &buf;
	return Contains(pChar);
}

bool TWideString::Contains(const wchar_t* pChar)
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
			pCharLen = wcslen(pChar);
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

int TWideString::IndexOf(wchar_t c, unsigned short startIndex)
{
	wchar_t buf[2];
	buf[0] = c;
	buf[1] = 0;
	wchar_t* pChar = (wchar_t*) &buf;
	return IndexOf(pChar, startIndex);
}

int TWideString::IndexOf(const wchar_t* pChar, unsigned short startIndex)
{
	unsigned short len = Length();
	unsigned short pCharLen = 0;
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = wcslen(pChar);
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

TWideString TWideString::Clone()
{
	TWideString result;
	result.Set(PData);
	return result;
}

TWideString TWideString::ToLower()
{
	TWideString result = Clone();

	wchar_t* pp = (wchar_t*) result.ToPWChar();
	int   len = result.Length();
	for(int i = 0; i<len; i++)
	{
		wchar_t c = *pp;
		if ((c>='A') && (c<='Z'))
		{
			*pp = c + 32;            
		}
		pp++;
	}
	return result;
}

TWideString TWideString::ToUpper()
{
	TWideString result = Clone();

	wchar_t* pp = (wchar_t*)result.ToPWChar();
	int   len = result.Length();
	for(int i = 0; i<len; i++)
	{
		wchar_t c = *pp;
		if ((c>='a') && (c<='z'))
		{
			*pp = c - 32;
		}
		pp++;
	}
	return result;
}

TWideString TWideString::Trim()
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
		return L"";
	}
	return SubString(begin, end-begin+1);    
}

TWideString TWideString::TrimStart()
{
	int len = Length();

	int begin = 0;
	for(int i = 0;  i<len; i++)
	{
		begin = i;
		if (PData[i]!=32) break;
	}

	return SubString(begin);
}

TWideString TWideString::TrimEnd()
{
	int len = Length();

	int end = 0;
	if (len!=0)
	{
		for(int i = len-1;  i>0; i--)
		{
			end = i;
			if (PData[i]!=32) break;
		}        
	}
	return SubString(0, end+1);
}

TWideString TWideString::SubString(unsigned short begin, unsigned short size)
{
	unsigned short len = Length();
	if (size==65535)
	{
		size = len-begin;
	}
	TWideString result;
	if (size<1)
	{
		return result;
	}
	if (begin>=len)
	{
		return result;
	}
	unsigned short newLength = result.SetLength(size);
	if (newLength>0)
	{
		result.CopyFrom(ToPWChar(), begin, newLength);
	}
	return result;    
}

void TWideString::Replace(wchar_t oldChar, wchar_t newChar)
{
	int len = Length();
	if (len==0) return;
	for(int i = 0; i<len; i++)
	{
		if (PData[i] == oldChar)
		{
			PData[i] = newChar;
		}
	}
}

TWideString TWideString::Replace(const wchar_t* oldString, const wchar_t* newString)
{
	int oldLength = 0;
	if (oldString!=NULL)
	{        
		oldLength = 0;
		if (oldString!=NULL)
		{
			oldLength = wcslen(oldString);
		}
	}
	int newLength = 0;
	if (newString!=NULL)
	{       
		newLength = 0;
		if (newLength!=NULL)
		{
			newLength = wcslen(newString);
		}
	}

	TWideString result;   
	int pos = 0;
	int index = 0;        
	while(true)
	{
		index = IndexOf(oldString, pos);
		if (index>0)
		{
			result += SubString(pos, index-pos);
			result += newString;
			pos += newLength;
		} else {
			result += SubString(pos);
			break;
		}
	}
	return result;    
}

TWideString::operator wchar_t*()
{
	return (wchar_t*)PData;
}

TWideString::operator const wchar_t*()
{
	return (wchar_t*)PData;
}


TWideString& TWideString::Add(wchar_t c)
{
	int oldLength = Length();
	SetLength(oldLength+1);
	if (Length()!=oldLength)
	{
		PData[oldLength] = c;
	}
	return *this;
}

TWideString& TWideString::Add(const wchar_t* s)
{
	int oldLength = Length();
	int sLength = 0;
	if (s!=NULL) sLength = wcslen(s);

	SetLength(oldLength+sLength);
	if (Length()!=oldLength)
	{
		memcpy(PData+oldLength, s, sLength*2);
	}
	return *this;
}

TWideString& TWideString::Add(TWideString& oString)
{
	int oldLength = Length();
	SetLength(oldLength+oString.Length());
	if (Length()!=oldLength)
	{
		memcpy(PData+oldLength, oString.ToPWChar(), oString.Length()*2);
	}
	return *this;
}

TWideString& TWideString::Insert(unsigned short index, wchar_t c)
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

TWideString& TWideString::Insert(unsigned short index, const wchar_t* s)
{
	if (index>Length()) return *this;

	int sLength = 0;
	if (s!=NULL) sLength=wcslen(s);

	int oldLength = Length();
	int newLength = SetLength(oldLength+1);

	for(int i = newLength; i>index; i--)
	{
		PData[i] = PData[i-sLength];
	}
	memcpy(PData+index, s, sLength*2);
	return *this;
}

TWideString& TWideString::Insert(unsigned short index, TWideString& oString)
{
	return *this;
}

TWideString& TWideString::Delete(unsigned short index, unsigned short length)
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

TWideString& TWideString::operator = (TWideString& oString )
{
	Set(oString.ToPWChar());
	return *this;
}    

TWideString& TWideString::operator = (const wchar_t* pChar)
{
	Set(pChar);
	return *this;
}    


TWideString TWideString::operator + ( const wchar_t* pChar )
{
	TWideString result = this->Clone();
	result+=pChar;
	return result;
}

TWideString TWideString::operator + (TWideString& oString )
{
	TWideString result = this->Clone();
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
			pCharLen = wcslen(pChar);    
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
		memcpy(&PData[oldLength], pChar, (newLength-oldLength)*2);
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
			pCharLen = wcslen(pChar);    
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
		memcpy(&PData[oldLength], pChar, (newLength-oldLength)*2);
	}
	return *this;
}


TWideString& TWideString::operator += ( const wchar_t AChar )
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

bool TWideString::operator > (const wchar_t* pChar)
{
	if (Length()==0)   return (false);
	if (pChar==NULL)   return (true);

	int pCharLen = 0;
	if (pChar!=NULL)
	{
		pCharLen = 0;
		if (pChar!=NULL)
		{
			pCharLen = wcslen(pChar);
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

bool TWideString::operator < (const wchar_t* pChar)
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
			pCharLen = wcslen(pChar);
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

bool TWideString::operator >= (TWideString& s)
{
	if (*this>s) return (true);
	if (*this==s) return (true);
	return false;
}

bool TWideString::operator >= (const wchar_t* pChar)
{
	if (*this>pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}


bool TWideString::operator <= (TWideString &s)
{        
	if (*this<s) return (true);
	if (*this==s) return (true);
	return false;
}

bool TWideString::operator <= (const wchar_t* pChar)
{        
	if (*this<pChar) return (true);
	if (*this==pChar) return (true);
	return false;
}

bool TWideString::operator == (TWideString& s)
{
	if ((ToPWChar()==NULL) && (s.ToPWChar()==NULL)) return true;
	if (ToPWChar()==NULL) return false;
	if (s.ToPWChar()==NULL) return false;
	return wcscmp(ToPWChar(), s.ToPWChar()) == 0;
}

bool TWideString::operator == (const wchar_t* pChar)
{
	if ((ToPWChar()==NULL) && (pChar==NULL)) return true;
	if ((ToPWChar()==NULL) && (*pChar==0)) return true;
	if (ToPWChar()==NULL) return false;
	if (pChar==NULL) return false;
	return wcscmp(ToPWChar(), pChar) == 0;
}

bool TWideString::operator != (TWideString& s)
{
	if ((ToPWChar()==NULL) && (s.ToPWChar()==NULL)) return false;
	if (ToPWChar()==NULL) return true;
	if (s.ToPWChar()==NULL) return true;
	return wcscmp(ToPWChar(), s.ToPWChar()) != 0;
}

bool TWideString::operator != (const wchar_t* pChar)
{
	if ((ToPWChar()==NULL) && (pChar==NULL)) return false;
	if (ToPWChar()==NULL) return true;
	if (pChar==NULL) return true;
	return wcscmp(ToPWChar(), pChar) != 0;
}

wchar_t TWideString::operator[] (int index)
{
	if (index<0) return 0;
	if (index>=Length()) return 0;
	return PData[index];
}
