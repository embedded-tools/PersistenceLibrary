/*
 * Persistence Library / FileUtils / TStringList
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

#include <stdlib.h>
#include "tstringlist.h"

TStringList::TStringList (TStringList &list)
{
    fsorted=false;
    item_num=0;	
    item_capacity=8;  
    obj =(int* )malloc (item_capacity*4);
    pstr=(int* )malloc (item_capacity*4);
     
    buf_size=0;
    buf_capacity=64;
    buf =(char*) malloc (buf_capacity);
	memset(buf, 0, buf_capacity);
    pstr[0]=0;

    int len = list.Count();
    for(int i = 0; i<len; i++)
    {
        AddObject(list.Strings(i), list.Objects(i));
    }
}

TStringList::TStringList (bool sorted)
{   
    fsorted=sorted;
    item_num=0;	
    item_capacity=8;  
    obj =(int* )malloc (item_capacity*4);
    pstr=(int* )malloc (item_capacity*4);
     
    buf_size=0;
    buf_capacity=64;
    buf =(char*) malloc (buf_capacity);
	memset(buf, 0, buf_capacity);
    pstr[0]=0;

}

TStringList::~TStringList ()
{
    if (buf)  free (buf);  buf = NULL;
    if (pstr) free (pstr); pstr= NULL;
    if (obj)  free (obj);  obj = NULL;
}

void TStringList::UpdateBufSizes ()
{
    realloc ((int*) obj,  item_capacity*4);
    realloc ((int*) pstr, item_capacity*4);
    realloc ((char*)buf,  buf_capacity);
}

void TStringList::Clear ()
{
    item_num=0; item_capacity=8;
    buf_size=0; buf_capacity=64;
    UpdateBufSizes ();
}

void TStringList::Add (const char* s)
{
    InsertObject (item_num,s,NULL);
    if (fsorted==true) Sort();
}

void TStringList::AddObject (const char* s, int sLength, void* AObject)
{
    InsertObject (item_num,s,sLength, AObject);	
    if (fsorted==true) Sort();
}

void TStringList::AddObject (const char* s, void* AObject)
{
    InsertObject (item_num,s,AObject);	
    if (fsorted==true) Sort();
}

int TStringList::Count ()
{
    return (item_num);
}

int TStringList::IndexOf (const char* s)
{
    if (!s) return -1;
    for(int i=0; i<item_num; i++)
    {
        if ( strcmp(this->Strings(i),s) == 0 )
            return i;
    }
    return -1;
}

void TStringList::Insert (int i, const char* s)
{
    InsertObject (0,s,NULL);
}

void TStringList::InsertBytes (int i, int len)
{
    char *pp;
    char *ss;
    int  c;
    int  memlen;

    ss=buf+pstr[i];
    pp=buf+pstr[i]+len;
    memlen=(int)(pstr[item_num-1]-pstr[i]);
	if (memlen>0) memcpy(pp,ss,memlen);

    for (c=item_num-1; c>=i; c--)
    {
        pstr[c+1]=pstr[c]+len;
        obj [c+1]=obj [c];
    }
    obj[item_num]=NULL;

}

void TStringList::Delete (int i)
{
    char *pp;
    char *ss;
    int  c;
    int  len;
    int  memlen;

    if (i>=item_num) return;
    if (i<0) return;

    ss=buf+pstr[i];
    pp=buf+pstr[i+1];
    len=pstr[i+1]-pstr[i];
    memlen=(int)(pstr[item_num]-pstr[i]);
    if (i<item_num-1)
    {
        if (memlen>0) memcpy (ss,pp,memlen);        
    }
    
    for (c=i; c<item_num; c++)
    {
        pstr[c]=pstr[c+1]-len;
        obj [c]=obj [c+1];
    }
    obj[item_num]=NULL;
    item_num--;
    buf_size=buf_size-len;
    buf[buf_size] = 0;

}

void TStringList::InsertObject (int i, const char* s, void* AObject)
{
    InsertObject(i, s, StrLen(s), AObject);
}

void TStringList::InsertObject (int i, const char* s, int sLength, void* AObject)
{
    if (i>=item_num) i = item_num;

    item_num++;
    if (item_num>=item_capacity) 
    {
        item_capacity=item_capacity+item_capacity;

        obj  =(int*)  realloc ((char*)  obj, item_capacity*4);
        pstr =(int*)  realloc ((char*)  pstr, item_capacity*4);

    }
    buf_size=buf_size+sLength+1;
  
    if (buf_size>=buf_capacity) 
    {
        while (buf_capacity<=buf_size) buf_capacity=buf_capacity+buf_capacity;

        buf = (char *) realloc ((char*)buf,buf_capacity);
    }    
    buf[buf_size]=0;

    
    char* ss;

    InsertBytes (i,sLength+1);
    ss=buf+pstr[i];

    memcpy (ss,(char*)s,sLength);
    ss[sLength]=0;
    obj[i]=(int)AObject;

}



void TStringList::Exchange (int i1, int i2)
{
    int ii;
    if (i1>=item_num) i1=item_num-1;
    if (i2>=item_num) i2=item_num-1;
    if (i1<0) return;
    if (i2<0) return;
    if (i1==i2) return;

    if (i1>i2) {ii=i1; i1=i2; i2=ii; }
    TString s1 (buf+pstr[i1]); int obj1=obj[i1];
    TString s2 (buf+pstr[i2]); int obj2=obj[i2];
    Delete (i2); Delete (i1);

    InsertObject (i1,s2,(void*)obj2);
    InsertObject (i2,s1,(void*)obj1);
}

bool TStringList::Compare (const char *s1, int len1, const char *s2, int len2)
{
    int len=len1; if (len2<len1) len=len2;
    int c;

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

    if (item_num<2) return;

    ss	= (int*) malloc (item_num*4); for (c=0; c<item_num; c++) ss [c]=c;
    len	= (int*) malloc (item_num*4); for (c=0; c<item_num; c++) len[c]=pstr[c+1]-pstr[c]-1;

    for (c=0; c<item_num; c++) {
        mini=c;
        for (d=c+1; d<item_num; d++) {
            pp1=(char*)(buf+pstr[ss[d]]); pp2=(char*)(buf+pstr[ss[mini]]);
            if (ascending) 
            {
                //max string length = 512
                for (e=512; e>0; e--) 
                {
                    if (*pp1<*pp2) { mini=d; break; };
                    if (*pp1>*pp2) break;
                    if (*pp1==0) break;
                    if (*pp2==0) break;
                    pp1++;
                    pp2++;
                };
            } else {
                for (e=512; e>0; e--) 
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
            x=obj[c];  obj[c]=obj[mini];  obj[mini]=x;
        };
    };

    char *newbuf  = (char*)malloc (buf_capacity);
    int  *newpstr = (int *)malloc (item_capacity*4);
    char *pp=newbuf;
    int		nn=0;
    for (c=0; c<item_num; c++) 
    {
        memcpy ((char*)pp, (char*)(buf+pstr[ss[c]]), len[c]+1); pp+=len[c]+1;
        newpstr[c]=nn; nn+=len[c]+1;
    };
    newpstr[item_num]=nn;
    free (buf);	 buf=newbuf;
    free (pstr); pstr=newpstr;
    free (ss);
    free (len);

};


bool TStringList::LoadFromStream (TStream *stream)
{
    char* pp;
	char* pp2;

    int     gc,c,nn;
    int     size;
	char    tmpBuf[2049];
    char*   tmp = (char*) &tmpBuf;

    Clear ();

    int totalsize=stream->GetSize();

    #ifdef __EXCEPTIONS
    try
    {
		#endif
        nn=0;
        while (stream->GetPosition()<(totalsize)) 
        {
            size=(int)stream->ReadBuffer(tmp+nn,2048-nn)+nn;  
            tmp[size]=0;
            pp= (char*) &tmp[0]; 
            pp2=(char*) &tmp[0];
            for (c=0; c<size; c++) 
            {
                if (*pp==13) {
                    *pp=0;
                    Add(pp2);
                    nn=size-c-1;
                    pp2=pp; 
                    pp2++;
                } else
                    if (*pp==10) {
                        nn=size-c-1;
                        pp2=pp; pp2++;
                    } else
                        if (c==(size-1)) 
                        {
                            gc++;
                            if (stream->GetPosition()==totalsize) Add (pp2);
                            else memcpy (tmp,pp2,nn);
                        }
                        pp++;
            }
        }
		#ifdef __EXCEPTIONS
    }
    catch (...)
    {
        return (false);
    }
		#endif

    return (true);

};

bool TStringList::LoadFromFile (const char* filename)
{    
    TFileStream *fs = new TFileStream(filename, efmOpenRead);
	if (fs==NULL) 
	{
		return false;
	}
    bool result = LoadFromStream (fs);
    delete fs;
    return result;
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
                    AddObject (pp2,lineLengthCounter, 0); 
                    pp2=pp; 
                    pp2++;
                }
                break;
            case 10:
                {
                    if (crlf==false)
                    {
                        //LF without CR is supported also
                        AddObject(pp2, lineLengthCounter, NULL); 
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
                    if (*pp2!=0) AddObject (pp2,lineLengthCounter, NULL);
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


bool TStringList::SaveToStream (TStream *stream)
{

    short newline=0x0a0d;

    TString filename;

    if (stream==NULL) return (false);
	  #ifdef __EXCEPTIONS
    try 
    {
		#endif
			
		for (int c=0; c<item_num; c++) {
				stream->WriteBuffer (buf+pstr[c],(int)(pstr[c+1]-pstr[c])-1);
				stream->WriteBuffer (&newline,2);
		}
				
    #ifdef __EXCEPTIONS				
    }		
    catch (...)
    {
        return false;
    };
		#endif
    return (true);	
}

bool TStringList::SaveToFile (const char* filename)
{
    bool result;
    TFileStream *fs;
    fs = new TFileStream (filename, efmCreate);
    result=SaveToStream (fs);
    delete fs;
    return  result;
};

TString TStringList::Strings (int i)
{
    if (i>item_num) i=item_num;
    return ( (char*)(buf+pstr[i]) );
}

TString TStringList::operator[] (int i)
{
    if (i>item_num) i=item_num;
    return ( (char*)(buf+pstr[i]) );
}

void* TStringList::Objects (int i)
{
    if (i>item_num) i=item_num;
    return ((void*)(obj[i]) );
}

char* TStringList::Get()
{
    return (buf);
};

int TStringList::GetLength()
{
    return buf_size;
}

int TStringList::GetCapacity()
{
    return buf_capacity;
}

void TStringList::Copy(TStringList* stl)
{
    if (!stl) return;
    this->Clear();

    this->fsorted = stl->fsorted;
    this->item_num = stl->item_num;
    this->item_capacity = stl->item_capacity;

    this->obj = (int*)realloc(this->obj,stl->item_capacity*sizeof(int));
    memcpy(this->obj,stl->obj,stl->item_num*sizeof(int));

    this->pstr = (int*)realloc(this->pstr,stl->item_capacity*sizeof(int));
    memcpy(this->pstr,stl->pstr,stl->item_num*sizeof(int));

    this->buf_capacity = stl->buf_capacity;
    this->buf_size = stl->buf_size;

    this->buf = (char*)realloc(this->buf,stl->buf_capacity);
    memcpy(this->buf,stl->buf,stl->buf_capacity);
}

