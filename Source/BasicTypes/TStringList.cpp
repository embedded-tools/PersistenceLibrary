/*
 * Persistence Library / FileUtils / TStringList
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

#include <stdlib.h>
#include "tstringlist.h"

TStringList::TStringList (const TStringList &list)
{
    m_sorted = false;
    m_stringDataCapacity = 64;
    m_stringData = (char*)malloc(m_stringDataCapacity);            
    m_stringDataSize = 0;
    memset(m_stringData, 0, m_stringDataCapacity);

    m_substringCapacity = 16;
    m_substringOffset = (int*)malloc(m_substringCapacity*sizeof(int));        
    m_substringCount = 0;
    memset(m_substringOffset, 0, m_substringCapacity*sizeof(int));

	for(int i = 0; i<list.Count(); i++)
	{
		Add(list[i]);
	}
}

TStringList::TStringList (bool sorted)
{   
    m_sorted = sorted;
    m_stringDataCapacity = 64;
    m_stringData = (char*)malloc(m_stringDataCapacity);            
    m_stringDataSize = 0;

    m_substringCapacity = 64;
    m_substringOffset = (int*)malloc(m_substringCapacity*sizeof(int));        
    m_substringCount = 0;
    memset(m_substringOffset, 0, m_substringCapacity*sizeof(int));
}

TStringList::~TStringList ()
{
	if (m_stringData) free(m_stringData);
	m_stringData = NULL;
	m_stringDataSize = 0;
	m_stringDataCapacity = 0;

	if (m_substringOffset) free(m_substringOffset);
	m_substringOffset = NULL;
	m_substringCount = 0;
	m_substringCapacity = 0;
}

void TStringList::Clear ()
{
    if (m_stringDataCapacity>64)
    {
        if (m_stringData)
        {
            free(m_stringData);
        }        
        m_stringDataCapacity = 64;
        m_stringData = (char*)malloc(m_stringDataCapacity);            
    }
    if (m_stringData)
    {
        memset(m_stringData, 0, m_stringDataCapacity);
    }
    m_stringDataSize = 0;

    if (m_substringCount>16)
    {
        if (m_substringOffset)
        {
            free(m_substringOffset);
        }        
        m_substringCapacity = 16;
        m_substringOffset = (int*)malloc(m_substringCapacity*sizeof(int));        
    }
    m_substringCount = 0;
    if (m_substringOffset)
    {
        memset(m_substringOffset, 0, m_substringCapacity);
    }    
}

void TStringList::Add (const char* s, int stringLength)
{
    if (s==NULL) return;
    if (stringLength==-1)
    {
        stringLength = (int)strlen(s);
    }
    Insert (m_substringCount,s,stringLength);
    if (m_sorted) Sort();
}

int TStringList::Count () const
{
    return (m_substringCount);
}

int TStringList::IndexOf (const char* s) const
{
    if (!s) return -1;

	const char* substring;
    for(int i=0; i<m_substringCount; i++)
    {
		substring = GetString(i);
		if (substring)
		{			
			if ( strcmp(substring,s) == 0 )
            return i;
		}
    }
    return -1;
}

void TStringList::Insert (int i, const char* s)
{
    Insert (0, s, 0);
}


void TStringList::Insert (int i, const char* s, int sLength)
{
	if (i<0) i = 0;
	if (i>=m_substringCount) i = m_substringCount;

	m_substringCount++;
	if (m_substringCount>=m_substringCapacity)
	{
		if (m_substringCapacity == 0) m_substringCapacity = 2;
		m_substringCapacity+=m_substringCapacity;
        m_substringCapacity+=m_substringCapacity;

		if (m_substringOffset == NULL)
		{
			m_substringOffset = (int*)malloc(m_substringCapacity*sizeof(int));
		} else {
			m_substringOffset = (int*)realloc(m_substringOffset, m_substringCapacity*sizeof(int));
		}
	}
	m_substringOffset[m_substringCount-1] = m_stringDataSize;
	m_stringDataSize+=sLength+1;	    
    m_substringOffset[m_substringCount] = m_stringDataSize;
    

	if (m_stringDataSize>(m_stringDataCapacity-NUMBER_OF_TERMINAL_ZEROS)) //2 = number of terminal zeros at array end
	{
		if (m_stringDataCapacity == 0)
		{
			m_stringDataCapacity = 16;
		}
		while(m_stringDataSize>(m_stringDataCapacity-NUMBER_OF_TERMINAL_ZEROS))
		{
			m_stringDataCapacity += m_stringDataCapacity;
            m_stringDataCapacity += m_stringDataCapacity;
		}
		if (m_stringData==NULL)
		{
			m_stringData = (char*)malloc(m_stringDataCapacity);
		} else {
			m_stringData = (char*)realloc(m_stringData, m_stringDataCapacity);
		}
	}   

    const char* source;
    char* target;
    int   length;
	if (i<m_substringCount-1)
	{
		source = m_stringData + m_substringOffset[i];
        target = m_stringData + m_substringOffset[i+1];
        length = m_substringCapacity - m_substringOffset[i+1];
		memcpy(target, source, length);
        for(int j = m_substringCount-1; j>i; j--)
        {
            m_substringOffset[j] = m_substringOffset[j-1];            
        }
	}
    source = s;
    target = m_stringData+m_substringOffset[i];
    length = sLength + 1;
    memcpy(target, source, length);
    m_stringData[m_stringDataSize]=0;
}

void TStringList::Delete (int i)
{
    if (i>=m_substringCount) return;
    if (i<0) return;

    char* target;
    char* source;
    int   length;
    
    if (i<m_substringCount-1)
    {
        source = m_stringData+m_substringOffset[i+1];
        target = m_stringData+m_substringOffset[i];
        length = m_stringDataCapacity - m_substringOffset[i+1];
        if (length>0)
        {
            memcpy(target, source, length);
        }
    }    
    length = m_substringOffset[i+1] - m_substringOffset[i];
    m_stringDataSize -= length;
    m_stringData[m_stringDataSize]=0;

    for(int j = i; j<m_substringCount; j++)
    {
        m_substringOffset[j] = m_substringOffset[j+1]-length;
    }
    m_substringOffset[m_substringCount] = 0;
    m_substringCount--;
}


void TStringList::Exchange (int i1, int i2)
{
    if (i1>=m_substringCount) return;
    if (i2>=m_substringCount) return;
    if (i1<0) return;
    if (i2<0) return;
    if (i1==i2) return;
    if (m_stringData==NULL) return;
    if (m_substringOffset == NULL) return;

    TString s1 (m_stringData+m_substringOffset[i1]);
    TString s2 (m_stringData+m_substringOffset[i2]); 
    Delete (i2); Delete (i1);
    Insert (i1,s2);
    Insert (i2,s1);
}

bool TStringList::Compare (const char *s1, int len1, const char *s2, int len2)
{
    int len=len1; if (len2<len1) len=len2;
    int c;

    if ((s1==NULL) && (s2==NULL))
    {
        return true;
    }
    if ((s1==NULL) || (s2==NULL))
    {
        return false;
    }

    bool lower=false;
    bool greater=false;

    for (c=0; c<len; c++) {
        if (s1[c]==s2[c]) continue;
        if (s1[c]<s2[c]) lower=true; else greater=true;
        break;
    };
    if (lower)     return true;
    if (greater)   return false;
    if (len2>len1) return true; 

    return false;
};

void TStringList::Sort (bool ascending)
{
    int c,d,e,x; 
    int mini;

    int  *ss;
    int  *len;
    char *pp1,*pp2;

    if (m_substringCount<2) return;

    ss	= (int*) malloc (m_substringCount*4); for (c=0; c<m_substringCount; c++) ss [c]=c;
    len	= (int*) malloc (m_substringCount*4); for (c=0; c<m_substringCount; c++) len[c]=m_substringOffset[c+1]-m_substringOffset[c];

    for (c=0; c<m_substringCount; c++) 
    {
        mini=c;
        for (d=c+1; d<m_substringCount; d++) 
        {
            pp1=(char*)(m_stringData+m_substringOffset[ss[d]]); 
            pp2=(char*)(m_stringData+m_substringOffset[ss[mini]]);
            if (ascending) 
            {
                for (e=1024; e>0; e--) 
                {
                    if (*pp1<*pp2) { mini=d; break; };
                    if (*pp1>*pp2) break;
                    if (*pp1==0) break;
                    if (*pp2==0) break;
                    pp1++;
                    pp2++;
                };
            } else {
                for (e=1024; e>0; e--) 
                {
                    if (*pp1>*pp2) { mini=d; break; };
                    if (*pp1<*pp2) break;
                    if (*pp1==0)   break;
                    if (*pp2==0)   break;
                    pp1++;
                    pp2++;
                };
            };
        };
        if (c!=mini) 
        { 
            x=ss[c];   ss[c]=ss[mini];    ss[mini]=x;  
            x=len[c];  len[c]=len[mini];  len[mini]=x;
        };
    };

    char *new_stringData      = (char*)malloc (m_stringDataCapacity);
    int  *new_substringOffset = (int *)malloc (m_substringCapacity*4);
    char *pp=new_stringData;
    int	  offset = 0;
    for (c=0; c<m_substringCount; c++) 
    {
        memcpy ((char*)pp, (char*)(m_stringData+m_substringOffset[ss[c]]), len[c]); 
        pp+=len[c];
        new_substringOffset[c] = offset;
        offset += len[c];
    };
    new_substringOffset[m_substringCount] = offset;
    
    free (m_stringData);	  m_stringData = new_stringData;
    free (m_substringOffset); m_substringOffset = new_substringOffset;
    free (ss);
    free (len);

};


void TStringList::SetText(const char* val)
{
    char   *pp,*pp2;

    int  lineLengthCounter;
    bool crlf=false;

    Clear ();    
    lineLengthCounter=0;
    pp= (char*) val; 
    pp2=(char*) val;

    while(true)
    {
        switch(*pp)
        {
            case 13:
                {
                    //CR+LF is supported
                    crlf=true;
                    Add (pp2,lineLengthCounter); 
                    pp2=pp; 
                    pp2++;
                }
                break;
            case 10:
                {
                    if (crlf==false)
                    {
                        //LF without CR is supported also
                        Add(pp2, lineLengthCounter); 
                        pp2=pp; 
                        pp2++;
                        lineLengthCounter = 0;
                    } else
                    {
                        //LF after CR is ignored
                        crlf=false;
                        pp2=pp; pp2++;
                        lineLengthCounter = 0;
                    }
                }
                break;
            case 0:
                {
                    //zero means string end
                    if (*pp2!=0) Add (pp2,lineLengthCounter);
                }
                break;
            default:
                {
                    //counts line length
                    lineLengthCounter++;
                }
        }
        if (*pp==0) break;

        pp++;
    }
    return;
}

const char* TStringList::operator[] (int i) const
{
    return GetString(i);
}

const char* TStringList::GetString (int i) const
{
    if (m_stringData==NULL) return NULL;
    if (m_substringOffset==NULL) return NULL;

    if (i>=m_substringCount) i=m_substringCount;
    return ( (char*)(m_stringData+m_substringOffset[i]) );
}

const TString TStringList::GetStringAsObject (int i) const
{
    static TString tmp;
    tmp = GetString(i);
    return tmp;
}

const char* TStringList::GetStringArray() const
{
    return (m_stringData);
};

int TStringList::GetLength() const
{
    return m_substringOffset[m_substringCount];
}

int TStringList::GetCapacity() const
{
    return m_stringDataCapacity;
}

bool TStringList::Copy(TStringList* stl)
{
    if (stl==NULL) return false;

    this->Clear();
    this->m_sorted = stl->m_sorted;
    this->m_substringCount = stl->m_substringCount;
    this->m_substringCapacity = stl->m_substringCapacity;    
    this->m_substringOffset = (int*)malloc(stl->m_substringCapacity*sizeof(int));
    if (this->m_substringOffset==NULL)
    {
        this->Clear();
        return false;
    }
    if (this->m_substringOffset)
    {
        memcpy(this->m_substringOffset,stl->m_substringOffset,stl->m_substringCount*sizeof(int));
    }    
    this->m_stringDataCapacity = stl->m_stringDataCapacity;
    this->m_stringDataSize = stl->m_stringDataSize;
    this->m_stringData = (char*)malloc(this->m_stringDataCapacity);
    if(this->m_stringData==NULL)
    {
        this->Clear();
        return false;
    }
    memcpy(this->m_stringData, stl->m_stringData, this->m_stringDataCapacity);
    return true;
}

