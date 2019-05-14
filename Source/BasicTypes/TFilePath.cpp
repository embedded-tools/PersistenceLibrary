/*
 * Persistence Library / Basic types / TFilePath
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
    for(int i = 0; i<DataLen; i++)
    {
        if ((PData[i]=='\\') || (PData[i]=='/') || (PData[i]==m_separator))
        {
            PData[i]=separator;
        }
    }	
    m_separator = separator;
}

const char* TFilePath::ExtractFileName()
{
    const char* result = NULL;
    int i = DataLen-1;
    while(i >= 0)
    {
        if ((Data[i]==92) || (Data[i]=='/')) break;
        result = (const char*)(Data + i);
        i--;
    };
    return result;
};

const char* TFilePath::ExtractFileExt ()
{
    const char* result = NULL;
    int i = DataLen-1;
    while(i >= 0)
    {
        if (Data[i] == '.')	
        {
            result = (const char*)(Data + i);
            break;
        }        
        i--;
    };
    return result;
};

TFilePath TFilePath::ExtractFileDirectory()
{
    int i = DataLen-1;
    while(i >= 0)
    {
        if (Data[i] == '\\') break;
        if (Data[i] == '/') break;
        i--;
    }
    TFilePath result((const char*)Data, i+1);
    return result;
};

//not debugged yet
TFilePath TFilePath::ExtractRelativePath (const char* basedir)
{
    int c,n;
    unsigned char* ss1;
    unsigned char* ss2;
    unsigned char* pp1;
    unsigned char* pp2;

    TFilePath result;

    pp1=Data;     ss1=pp1;
    pp2=(unsigned char*) basedir; ss2=pp2;
    n=0;
    for (c=1; c<=DataLen; c++) 
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
        if (pp1==Data) result=(char*)pp1;
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
    if (SetLength(n))
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
    if (SetLength(n))
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

bool TFilePath::DeleteLastDir()
{
    unsigned short i;

    if (Length()==2)
    {
        if ((PData[0]=='.') && ( (PData[1]=='\\') || (PData[1]=='/')) )
        {
            return false;
        }
    }

    if (Length()==3)
    {
        if ( ((*this)[0]=='.')  && 
            ((*this)[1]=='.')  && 
            ( ((*this)[2]=='\\') || ((*this)[2]=='/') )
            )   
        {
            if (SetLength(2))
            {
                (*this)[1]=m_separator;
                return true;
            }
            return false;
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
        Clear();
    }
    return true;
}

void TFilePath::DeleteDoubleSlash()
{
    for(int i = 1; i<DataLen; i++)
    {
        if ((PData[i-1]==m_separator) && (PData[i]==m_separator))
        {
            for(int j = i; j<DataLen; j++)
            {
                PData[j-1] = PData[j];
            }						
            DataLen--;
        }
    }
}



TFilePath TFilePath::operator = (const TFilePath& oString )
{
    CopyFrom(oString.ToPChar());
    for(int i = 0; i<DataLen; i++)
    {
        if (PData[i]=='\\') m_separator='\\';
        if (PData[i]=='/') m_separator='/';
    }
    return *this;    
}

TFilePath& TFilePath::operator = (const TString& oString )
{
    CopyFrom(oString.ToPChar());
    for(int i = 0; i<DataLen; i++)
    {
        if (PData[i]=='\\') m_separator='\\';
        if (PData[i]=='/') m_separator='/';
    }
    return *this;
}

TFilePath& TFilePath::operator = (const char* pChar)
{
    CopyFrom(pChar);
    for(int i = 0; i<DataLen; i++)
    {
        if (PData[i]=='\\') m_separator='\\';
        if (PData[i]=='/') m_separator='/';
    }
    return *this;
}

TFilePath TFilePath::operator + (const char* pChar )
{
    TFilePath result = *this;
    result.ChangeSeparator(m_separator);
    result+=pChar;
    return result;
}

TFilePath& TFilePath::operator += (const char* pChar )
{
    if (pChar==NULL)
    {
        return *this;
    }

    int pCharLen = 0;

    if (LastChar()>=' ')
    {
        bool fileFound = false;
        for(int i = DataLen; i>=0; i--)
        {
            if ((PData[i]=='\\') || (PData[i]=='/')) 
            {
                if (fileFound)
                {
                    SetLength(i+1);
                }
                break;
            }
            if (PData[i]=='.')
            {
                fileFound = true;
            }
        }
        if ((LastChar()!='\\') && (LastChar()!='/') && (!fileFound))
        {            
            if(SetLength(DataLen+1,false))
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
            if ((pChar[1]=='.'))
            {
                if ((pChar[2]=='\\') || (pChar[2]=='/'))
                {
                    if (!DeleteLastDir())
                    {
                        break;
                    }
                }
                pChar+=3;                
            } else
                if ((pChar[1]=='\\') || (pChar[1]=='/'))
                {
                    pChar+=2;
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
    if (DataLen==2)
    {
        if (PData && pChar)
        {
            if ( (pChar[0]=='.') && (PData[0]=='.') && ((PData[1]=='\\') || (PData[1]=='/')) )
            {
                Clear();
            }            
        }
    }
    if (pChar!=NULL)
    {
        pCharLen = 0;
        if (pChar!=NULL)
        {
            pCharLen = (int)strlen(pChar);    
        }
    }
    if (pCharLen>=3)
    {
        if ( ((pChar[0]>='A')&&(pChar[0]<='Z')) || ((pChar[0]>='a')&&(pChar[0]<='z')) )
        {
            if (pChar[1]==':')
            {
                Clear();
            }
        }
    }

    if (pCharLen==0) return *this;
    unsigned short oldLength = Length();
    unsigned short newLength = oldLength + pCharLen;
    if (newLength<oldLength) newLength=65534;
    if (newLength==65535) newLength=65534;
    if (SetLength(newLength))
    {
        memcpy(&PData[oldLength], pChar, DataLen-oldLength);
        if (PData)
        {
            ChangeSeparator(m_separator);
        }
    }
    return *this;
}

TFilePath&	TFilePath::operator += (const char c)
{
    char cc = c;
    if ((cc=='\\') || (cc=='/'))
    {
        cc = m_separator;
    }	
    if (SetLength(DataLen+1, false))
    {
        PData[DataLen-1] = cc;
    }
    return *this;
}

bool TFilePath::operator == (const TCustomString<FILEPATH_MAXLENGTH>& s)
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

bool TFilePath::operator != (const TCustomString<FILEPATH_MAXLENGTH>& s)
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
