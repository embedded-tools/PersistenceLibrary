/*
 * Persistence Library / Basic types / TFilePath
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

#include "TFilePath.h"

TFilePath::TFilePath()
:TCustomString<FILEPATH_MAXLENGTH>()
{
	m_separator = '\\';
}

TFilePath::TFilePath(const char* text, unsigned short textLength)
:TCustomString<FILEPATH_MAXLENGTH>(text, textLength)
{
	m_separator = '\\';
	if (PData!=NULL)
	{
		for(int i = 0; i<DataLen; i++)
		{
			if (PData[i]=='\\') m_separator='\\';
			if (PData[i]=='/') m_separator='/';
		}
	}
}

void TFilePath::ChangeSeparator(char separator)
{
	m_separator = separator;
}

const char* TFilePath::ExtractFileName(const char* filename)
{
	const char* result = NULL;
	int i = StrLen(filename)-1;
	while(i >= 0)
	{
		if ((filename[i]==92) || (filename[i]=='/')) break;
		i--;
	};
	return result;
};

const char* TFilePath::ExtractFileExt (const char* filename)
{
	const char* result = NULL;
	int i = StrLen(filename);
	while(i >= 0)
	{
		if (filename[i] == '.')	break;
		i--;
	};
	return result;
};

TFilePath TFilePath::ExtractFileDirectory(const char* filename)
{
	int i = StrLen(filename);
	while(i >= 0)
	{
		if (filename[i] == '\\') break;
		if (filename[i] == '/') break;
		i--;
	}
	TFilePath result(filename, i);
	return result;
};

TFilePath TFilePath::ExtractRelativePath (const char* dir, const char* basedir)
{
	int c,n;
	char* ss1;
	char* ss2;
	char* pp1;
	char* pp2;

	TFilePath result;

	pp1=(char*) dir;     ss1=pp1;
	pp2=(char*) basedir; ss2=pp2;
	n=0;
	for (c=1; c<=StrLen(dir); c++) 
	{
		//case insensitive string comparison
		if ((*ss1 & 0xDF) != (*ss2 & 0xDF)) break;
		if ((*ss1=='\\') || (*ss1=='/'))
        {
			pp1=ss1;  pp2=ss2; 
			pp1++;    pp2++;
		}
		ss1++; ss2++; n++;
	}
	if (n==StrLen(basedir)) 
	{ 
		result = (const char*)pp1;
	}
	else 
	{
		while (*pp2!=0) 
		{
			if ((*pp2=='\\') || (*pp2=='/')) 
			{
				result += "..";
				result += m_separator;
			}
			pp2++;
		}
		if ((basedir[StrLen(basedir)-1]!='\\') && (basedir[StrLen(basedir)-1]!='/')) 
		{
			result += "..";
			result += m_separator;
		}
		if (pp1==dir) result=(const char*)pp1;
		else result += (const char*)pp1;
	}
	return (result);  
}

bool TFilePath::ChangeFileExt (const char* ext)
{
	int pos = Length();
	for(int i = Length()-1; i>=0; i--)
	{
		if (PData[i] == '.') 
		{
			pos = i;
			break;
		}
		if ((PData[i]=='\\') || (PData[i]=='/'))
		{
			break;
		}
	}
	unsigned short extLen = StrLen(ext);
	if (Length()+extLen>=FILEPATH_MAXLENGTH)
	{
		return false;
	}
	int n = pos+extLen;
	SetLength(n);
	if (DataLen==n)
	{
		memcpy(PData+pos, ext, extLen+1);
	}
	return true;
};

bool TFilePath::ChangeFileName (const char* filename)
{
	int pos = Length();
	for(int i = Length()-1; i>=0; i--)
	{
		if ((PData[i]=='\\') || (PData[i]=='/'))
		{
			pos = i + 1;
			break;
		}
	}
	unsigned short extLen = StrLen(filename);
	if (Length()+extLen>=FILEPATH_MAXLENGTH)
	{
		return false;
	}
	int n = pos+extLen;
	SetLength(n);
	if (DataLen==n)
	{
		memcpy(PData+pos, filename, extLen+1);
	}
	return true;
};

void TFilePath::DeleteFileName()
{
	int i = LastIndexOf('\\');
	if (i==-1)
	{
		i = LastIndexOf('/');
	}
	if (i!=-1)
	{
		SetLength(i+1);
	}
}

void TFilePath::DeleteLastDir()
{
    unsigned short i;
	
	if (Length()==3)
	{
		if ( ((*this)[0]=='.')  && 
			 ((*this)[1]=='.')  && 
			 ( ((*this)[2]=='\\') || ((*this)[2]=='/') )
		   )   
		{
			SetLength(2);
			(*this)[1]=m_separator;
			return;
		}
	}
	bool slashFound = false;
    for (i = Length()-2; i>0; i--) 
    {
        if ( ((*this)[i]=='\\') || ((*this)[i]=='/') )
        {
            SetLength(i+1);
			slashFound = true;
            break;
        }
		if ((*this)[i]==':')
		{
			slashFound = true;
			break;
		}
    }
	if (!slashFound)
	{
		if (Length()==2)
		{
			if ((PData[0]=='.') && ( (PData[1]=='\\') || (PData[1]=='/')) )
			{
				return;
			}
		}
		Clear();
	}
}


TFilePath TFilePath::operator = ( TFilePath& oString )
{
	CopyFrom(oString.ToPChar());
	for(int i = 0; i<DataLen; i++)
	{
		if (i=='\\') m_separator='\\';
		if (i=='/') m_separator='/';
	}
	return *this;    
}

TFilePath& TFilePath::operator = ( TString& oString )
{
	CopyFrom(oString.ToPChar());
	for(int i = 0; i<DataLen; i++)
	{
		if (i=='\\') m_separator='\\';
		if (i=='/') m_separator='/';
	}
	return *this;
}

TFilePath& TFilePath::operator = ( const char* pChar)
{
	CopyFrom(pChar);
	return *this;
}

TFilePath TFilePath::operator + ( const char* pChar )
{
	TFilePath result = *this;
	result+=pChar;
	return result;
}

TFilePath& TFilePath::operator += ( const char* pChar )
{
	if (pChar==NULL)
	{
		return *this;
	}

    int pCharLen = 0;

	if (LastChar()>=' ')
	{
		if ((LastChar()!='\\') && (LastChar()!='/'))
		{
			SetLength(DataLen+1,false);
			if (PData!=NULL)
			{
				PData[DataLen-1] = m_separator;						
			}
		}
	}
	if (pChar!=0)
	{
        while (pChar[0]=='.')
        {
			DeleteFileName();
			pChar++;
            if ((pChar[0]=='.'))
            {
                pChar++;
				if ((pChar[0]=='\\') || (pChar[0]=='/'))
				{
					pChar++;
				}
                DeleteLastDir();
            } else
			if ((pChar[0]=='\\') || (pChar[0]=='/'))
            {
                pChar++;
            } else break;
        }
    };			
	if ((LastChar()=='\\') || (LastChar()=='/'))
	{
		if ((*pChar=='\\') || (*pChar=='/'))
		{
			pChar++;
		}
	}
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
		if (PData)
		{
			for(int i = 0; i<DataLen; i++)
			{
				if ((PData[i]=='\\') || (PData[i]=='/'))
				{
					if (PData[i]!=m_separator)
					{
						PData[i]=m_separator;
					}
				}
			}	
		}
	}
	return *this;
}

TFilePath&	TFilePath::operator += ( const char c)
{
	int oldLength = DataLen;
	SetLength(DataLen+1, false);
	if (DataLen>oldLength)
	{
		PData[DataLen-1] = c;
	}
	return *this;
}

bool TFilePath::operator == (TCustomString<FILEPATH_MAXLENGTH>& s)
{
	if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return true;
	if (ToPChar()==NULL) return false;
	if (s.ToPChar()==NULL) return false;
	return strcmp(ToPChar(), s.ToPChar()) == 0;
}

bool TFilePath::operator == (const char* pChar)
{
	if ((ToPChar()==NULL) && (pChar==NULL)) return true;
	if ((ToPChar()==NULL) && (*pChar==0)) return true;
	if (ToPChar()==NULL) return false;
	if (pChar==NULL) return false;
	return strcmp(ToPChar(), pChar) == 0;
}

bool TFilePath::operator != (TCustomString<FILEPATH_MAXLENGTH>& s)
{
	if ((ToPChar()==NULL) && (s.ToPChar()==NULL)) return false;
	if (ToPChar()==NULL) return true;
	if (s.ToPChar()==NULL) return true;
	return strcmp(ToPChar(), s.ToPChar()) != 0;
}

bool TFilePath::operator != (const char* pChar)
{
	if ((ToPChar()==NULL) && (pChar==NULL)) return false;
	if (ToPChar()==NULL) return true;
	if (pChar==NULL) return true;
	return strcmp(ToPChar(), pChar) != 0;
}