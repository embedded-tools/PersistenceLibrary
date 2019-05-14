/*
 * Persistence Library / Basic types / TXMLDoc
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

#include "TXmlDoc.h"
#include "StringUtils.h"
#include "TFileStream.h"
    
TXMLDoc::TXMLDoc()
{
    parserLevel = NULL;
    parserData  = NULL;
    parserDataLength = 0;
    parserDataNeedUnalloc = false;
    parserPosition = 0;    
    endReached = false;	
	SetPool(NULL);
}

TXMLDoc::TXMLDoc(TXMLTagBasePool& tagPool)
{
    parserLevel = NULL;
    parserData  = NULL;
    parserDataLength = 0;
    parserDataNeedUnalloc = false;
    parserPosition = 0;    
    endReached = false;	

	SetPool(&tagPool);
}

TXMLDoc::~TXMLDoc()
{

}

void TXMLDoc::SetPool(TXMLTagBasePool* tagPool)
{
	TXMLTag::SetTagPool(tagPool);
}

TXMLTagBasePool* TXMLDoc::TagPool()
{
    return TXMLTag::TagPool;
}

TXMLTag* TXMLDoc::Root()
{
    if (TagPool()==NULL)
    {
        return NULL;
    }
    TXMLTag* root = TagPool()->GetChild(NULL, 1);
    if (root==NULL)
    {
        root = TagPool()->GetChild(NULL, 0);
    }
	return root;
}

TXMLTag* TXMLDoc::Header()
{
    TXMLTag* root = TagPool()->GetChild(NULL, 1);
    if (root==NULL)
    {
        return NULL;
    }
    TXMLTag* header = TagPool()->GetChild(NULL, 0);
    return header;
}

void TXMLDoc::Clear()
{
    //only xmltags are owned, all of them must be destroyed	        
    parserLevel = NULL;
    parserData  = NULL;
    parserDataLength = 0;
    parserPosition = 0;
    endReached = false;    
    if (TagPool()!=NULL)
    {
        TagPool()->Clear();
    }
}

bool TXMLDoc::LoadFromBuffer (char* rewriteableBuffer, int xmlLength)
{
    Clear();  
    if (TagPool()==NULL)
    {
        return false;
    }

    endReached = false;
    parserLevel = NULL;
    parserData = (char*) rewriteableBuffer;
    parserDataNeedUnalloc = false;
    
    if (xmlLength==0)
    {
        xmlLength = (int)strlen(rewriteableBuffer);
    }    
    parserDataLength = xmlLength;
    parserPosition = 0;
    bool res = ParseXML();
    return res;
}

bool TXMLDoc::LoadFromString(TString &rewriteableString)
 {
    Clear();
    if (TagPool()==NULL)
    {
        return false;
    }
    TagPool()->Clear();

    endReached = false;
    parserLevel = NULL;	
    
    parserDataLength = rewriteableString.Length();
    parserData = (char*)rewriteableString.ToPChar();
    parserDataNeedUnalloc = false;
    parserPosition = 0;
    bool res = ParseXML();
    return res;
}

bool TXMLDoc::LoadFromFile(const char* filename)
{
    Clear();
    if (TagPool()==NULL)
    {
        return false;
    }
    TagPool()->Clear();
    
    endReached = false;
    parserLevel = NULL;
    FILE* hFile = fopen(filename, "rb");
    if (hFile==NULL)
    {
        return false;
    }
    fseek(hFile, 0, SEEK_END);
    parserDataLength = ftell(hFile);
    parserData = (char*)malloc(parserDataLength);
    fclose(hFile);

    hFile = fopen(filename, "rb");
    fread(parserData, 1, parserDataLength, hFile);   
    parserDataNeedUnalloc = true;    
    parserPosition = 0;

    bool res = ParseXML();
    return res;
}

bool TXMLDoc::SaveToStream (TStream& stream)
{
	TXMLTag* header = Header();
	if (header)
	{
		header->SaveToStream(stream);
	}
	TXMLTag* root = Root();
	if (root)
	{
		root->SaveToStream(stream);
	}
	return true;
}

bool TXMLDoc::SaveToFile (const char* filename)
{
	TFileStream* fs = new TFileStream(filename, efmCreate);
	if (fs==NULL)
	{
		return false;
	}
	bool result = SaveToStream(*fs);
	fs->Close();
	delete fs;
	return result;
}

char TXMLDoc::NextChar()
{
    parserPosition++;
    return CurrentChar();
}

char TXMLDoc::NextNonEmptyChar()
{
    parserPosition++;
    char c = CurrentChar();
    while ((c==32) || (c==13) || (c==10))
    {
        c = NextChar();
    }
    return c;
}


char TXMLDoc::CurrentChar()
{
    if (parserData==NULL)
    {
        return 0;
    }
    if (endReached)
    {
        return 0;
    }

    char c = parserData[parserPosition];
    if (c==0)	
    {
        return 0;
    }
    if (parserPosition==parserDataLength)
    {
        endReached = true;
        return 0;
    }
    return c;
}

bool TXMLDoc::ParseXML()
{
    char c = CurrentChar();
    while(c!=0)
    {
        if (c=='<')
        {
            c = NextChar();
            if (c=='/')
            {
                if (parserLevel==NULL)
                {
                    return false;
                }
                parserLevel = parserLevel->ParentTag;

                if (parserLevel==NULL)
                {
                    return true;
                }
                char* endTagName = parserData + parserPosition;
                while((c!='>') && (c!=0))
                {
                    c = NextChar();	
                }
                parserData[parserPosition] = 0;

            } else {
                char* tagName = parserData + parserPosition;
                TXMLTag* newTag = TagPool()->CreateXMLTag(parserLevel);
                TagPool()->SetXMLTagName(newTag, tagName);

                c = NextChar();

                bool emptyTag = false;
                char attributeCount = 0;
                while(c!=0)
                {    
                    if (c=='=')
                    {
                        //= is not valid part of xml name,
                        //therefore is replaced by space
                        c=' ';
                    }
                    if (c==' ')
                    {                                                
                        //end of xmltag or begin of xmlattribute
                        parserData[parserPosition] = 0;
                        RemoveEscapedCharacters(tagName);

                        c = NextNonEmptyChar();
                        newTag->FirstAttribute = NULL;
                        newTag->LastAttribute = NULL;
                        if (c!='/')
                        {
                            newTag->FirstAttribute = parserData + parserPosition;
                            //reads any number of attributes
                            while(c!=0)
                            {
                                while(c!=0)
                                {   
                                    c=NextNonEmptyChar();
                                    if ((c=='=') || (c=='>'))
                                    {
                                        break;
                                    }                                    
                                }
                                if (c=='>') break;

                                //reads atrribute value                                
                                c=NextNonEmptyChar();                                
                                char separator = ' ';
                                if (c=='"') 
                                {
                                    separator='"';
                                    c = NextChar();
                                }
                                if (c==39) //apostrophe
                                {
                                    separator=39;
                                    c = NextChar();
                                }
                                char* attrValue = parserData + parserPosition;
                                while(c!=0)
                                {
                                    c=NextChar();
                                    if (c==separator)
                                    {
                                        parserData[parserPosition] = 0;
                                        RemoveEscapedCharacters(attrValue);
                                        break;
                                    }                                        
                                }     
                                attributeCount++;
                                newTag->LastAttribute = parserData + parserPosition + 1;

                                c=NextNonEmptyChar();
                                if ((c=='/') || (c=='>')) break;
                            }
                        }
                    }					
                    if (c=='>')
                    {
                        //end of tag name                        
                        parserData[parserPosition] = 0;
                        RemoveEscapedCharacters(tagName);
                    }
                    if (c=='/')
                    {
                        c = NextChar();
                        if (c=='>')
                        {
                            emptyTag = true;
                        }
                    }
                    if (c=='>')                    
                    {
                        c=NextNonEmptyChar();
                        char* tagValue = parserData + parserPosition;
                        newTag->Value = tagValue;
                        while(c!=0)
                        {
                            if (c=='<')
                            {
                                parserData[parserPosition] = 0;
                                RemoveEscapedCharacters(tagValue);

                                //quits xml attribute loop
                                break;
                            }                            
                            c = NextChar();
                        }
                        break;
                    }    
                    c=NextChar();
                }                                
                //end of one xml tag
                if (tagName[0]!='?')
                {
                    if (emptyTag)
                    {
                        if (parserLevel==NULL)
                        {
                            return true;
                        }
                    } else {                   
                        parserLevel = newTag;  
                    }
                }
                continue;
            }
        }
        c = NextNonEmptyChar();
    }
    return false;
}

void TXMLDoc::RemoveEscapedCharacters(char* value)
{
    char* tgt = value;
    char* src = value;
    while(*src!=0)
    {
        if (*src!='&') 
        {
            *tgt = *src;
            tgt++;
            src++;
            continue;
        }
        if ((src[1]=='l') && (src[2]=='t') && (src[3]==';'))
        {
            *tgt='<';
            tgt++;
            src+=4;
            continue;
        }        
        if ((src[1]=='g') && (src[2]=='t') && (src[3]==';'))
        {
            *tgt='>';
            tgt++;
            src+=4;
            continue;
        }        
        if ((src[1]=='a') && (src[2]=='m') && (src[3]=='p') && (src[4]==';'))
        {
            *tgt='&';
            tgt++;
            src+=5;
            continue;
        }        
        if ((src[1]=='q') && (src[2]=='u') && (src[3]=='o') && (src[4]=='t') && (src[5]==';'))
        {
            *tgt='\"';
            tgt++;
            src+=6;
            continue;
        }     
        *tgt='?';
        tgt++;
        while(*src!=';')
        {
            if (*src==0) break;
            src++;
        }
        src++;
    }
    if (src==tgt) return;

    *tgt=0;
    while(src!=tgt)
    {
        tgt++;
        *tgt=' ';        
    }
}

TXMLTag* TXMLDoc::SelectNode(const char* xpath)
{
    if (xpath==NULL)
    {
        return NULL;
    }
    TXMLTag* rootTag = TagPool()->GetChild(NULL, 0);
    if (rootTag==NULL)
    {
        return NULL;
    }
    if (xpath[0]=='/')
    {
        xpath++;
    }
	short n = StrPos("/",xpath);

    TShortString rootName;
    const char*  tagName = xpath;
    if (n==-1)
    {
        rootName=xpath;
        tagName=NULL;        
    } else {
        rootName.CopyFrom(xpath,n);
        tagName = xpath + n + 1;
    }
    if (rootName!=rootTag->GetName())
    {
        return NULL;
    }
    if (tagName!=NULL)
    {
        return rootTag->SelectNode(tagName);
    }
    return rootTag;
}

TXMLTagList* TXMLDoc::SelectNodes(const char* xpath, bool no_malloc)
{
	if (xpath==NULL)
	{
		return NULL;
	}
	TXMLTag* rootTag = TagPool()->GetChild(NULL, 0);
	if (rootTag==NULL)
	{
		return NULL;
	}
	if (xpath[0]=='/')
	{
		xpath++;
	}
	short n = StrPos("/",xpath);

	TShortString rootName;
	const char*  tagName = xpath;
	if (n==-1)
	{
		rootName=xpath;
		tagName=NULL;        
	} else {
		rootName.CopyFrom(xpath,n);
		tagName = xpath + n;
	}
	if (rootName!=rootTag->GetName())
	{
		return NULL;
	}
	if (tagName!=NULL)
	{
		return rootTag->SelectNodes(tagName, no_malloc);
	}
	return NULL;
}

unsigned short TXMLDoc::CountNodes(const char* xpath)
{
	if (xpath==NULL)
	{
		return 0;
	}
	TXMLTag* rootTag = TagPool()->GetChild(NULL, 0);
	if (rootTag==NULL)
	{
		return 0;
	}
	if (xpath[0]=='/')
	{
		xpath++;
	}
	short n = StrPos("/",xpath);

	TShortString rootName;
	const char*  tagName = xpath;
	if (n==-1)
	{
		rootName=xpath;
		tagName=NULL;        
	} else {
		rootName.CopyFrom(xpath,n);
		tagName = xpath + n;
	}
	if (rootName!=rootTag->GetName())
	{
		return 0;
	}
	if (tagName!=NULL)
	{
		return rootTag->CountNodes(tagName);
	}
	return 0;
}





