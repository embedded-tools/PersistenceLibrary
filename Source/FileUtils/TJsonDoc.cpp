#include "TJsonDoc.h"
#include <stdio.h>
#include "TFileStream.h"

TJsonDoc::TJsonDoc()
{
    parserLevel = NULL;
    parserData  = NULL;
    parserDataLength = 0;
    parserDataNeedUnalloc = false;
    parserPosition = 0;    
    endReached = false;	
    SetPool(NULL);
}

TJsonDoc::TJsonDoc(TJsonTagBasePool& tagPool)
{
    parserLevel = NULL;
    parserData  = NULL;
    parserDataLength = 0;
    parserDataNeedUnalloc = false;
    parserPosition = 0;    
    endReached = false;	

    SetPool(&tagPool);
}

TJsonDoc::~TJsonDoc()
{
	
}

void TJsonDoc::SetPool(TJsonTagBasePool* tagPool)
{
    TJsonTag::SetTagPool(tagPool);
}

TJsonTagBasePool* TJsonDoc::TagPool()
{
    return TJsonTag::TagPool;
}

TJsonTag* TJsonDoc::Root()
{
    if (TagPool()==NULL)
    {
        return NULL;
    }
    TJsonTag* root = TagPool()->GetChild(NULL, 1);
    if (root==NULL)
    {
        root = TagPool()->GetChild(NULL, 0);
    }
    return root;
}

TJsonTag* TJsonDoc::Header()
{
    TJsonTag* root = TagPool()->GetChild(NULL, 1);
    if (root==NULL)
    {
        return NULL;
    }
    TJsonTag* header = TagPool()->GetChild(NULL, 0);
    return header;
}

void TJsonDoc::Clear()
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

bool TJsonDoc::LoadFromBuffer (char* rewriteableBuffer, int jsonLength)
{
	Clear();
	if (TagPool()==NULL)
	{
		return false;
	}
	TagPool()->Clear();

	endReached = false;

    if (jsonLength==-1)
    {
        jsonLength = (int)strlen(rewriteableBuffer);
    }
	parserLevel = NULL;
	parserDataLength = jsonLength;
	parserData = rewriteableBuffer;
	parserDataNeedUnalloc = false;
	parserPosition = 0;

	TJsonTag* rootTag = TagPool()->CreateJsonTag(NULL);
	bool res = ParseValue(rootTag);
	return res;
}

bool TJsonDoc::LoadFromString(TString &rewriteableString)
{
	bool res = LoadFromBuffer((char*)rewriteableString.ToPChar(), rewriteableString.Length());
	return res;
}
bool TJsonDoc::LoadFromFile(const char* filename)
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

	parserPosition = 0;
    hFile = fopen(filename, "rb");
    fread(parserData, 1, parserDataLength, hFile);   


	bool res = LoadFromBuffer(parserData, parserDataLength);
    parserDataNeedUnalloc = true;    
	return res;    
}

bool TJsonDoc::SaveToStream (TStream& stream)
{
    int n = TagPool()->GetChildCount(NULL);
    for(int i = 0; i<n; i++)
    {
        TJsonTag* tag = TagPool()->GetChild(NULL, i);
        if (tag==NULL) continue;
        tag->SaveToStream(stream);
    }
    return true;
}

bool TJsonDoc::SaveToFile (const char* filename)
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

char TJsonDoc::NextChar()
{
    parserPosition++;
    return CurrentChar();
}

char TJsonDoc::NextNonEmptyChar()
{
    parserPosition++;
    unsigned char c = (unsigned char)CurrentChar();	
    while (c<=32)
    {
		if (c==0) return 0;
        c = (unsigned char)NextChar();
    }
    return c;
}


char TJsonDoc::CurrentChar()
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
    if (parserPosition>=parserDataLength)
    {
        endReached = true;
        return 0;
    }
    return c;
}

bool TJsonDoc::ParseObject(TJsonTag* currentTag)
{
    parserLevel = currentTag;

    char c = CurrentChar();
    if (c!='{') return false;

    c = NextNonEmptyChar();

    const char* valueName;
    bool res;

    res = true;
    while(c>0)
    {
        if (c=='}')
        {
            //end of object
            c = NextNonEmptyChar();
            return true;
        }
        valueName = ParseName(); 
        if (valueName==NULL) return false;

        TJsonTag* newTag = TagPool()->CreateJsonTag(parserLevel);	
        TagPool()->SetJsonTagName(newTag, valueName);

        res  = ParseValue(newTag); 
        if (!res) return false;
        
        //reads next character 
        c = CurrentChar();
        
        //then sets it to zero
        parserData[parserPosition] = 0;           

        if (c<=32)
        {
            c = NextNonEmptyChar();
        }
        if (c=='}')
        {
            c = NextNonEmptyChar();
            if (parserLevel)
            {
                parserLevel = parserLevel->GetParentTag();
            }
            return true;
        }
        if (c!=',')
        {
            //unknown syntax
            return false;
        }
        c = NextNonEmptyChar();
    }
    return false;
}

const char* TJsonDoc::ParseName()
{
    char c = CurrentChar();    

    const char* name   = &parserData[parserPosition];
    bool  alphaNumeric = false;
    while(c!=0)
    {
        if (c==':')
        {
            parserData[parserPosition] = 0;
          
            //name is now terminated by zero
            break;
        }        
        alphaNumeric = ((c>='A') && (c<='Z')) ||
                       ((c>='a') && (c<='z')) ||
                       ((c>='0') && (c<='9')) ||
                       ( c=='_');
        if (!alphaNumeric) return NULL;       

		    c =NextChar();
    }
    c = NextNonEmptyChar();
    return name;
}

bool TJsonDoc::ParseValue(TJsonTag* currentTag)
{
    char c = CurrentChar();
    if (c=='{')
    {
        currentTag->m_tagType = TJsonTag::JSONObject;
        return ParseObject(currentTag);
    }
    if (c=='[')
    {
        currentTag->m_tagType = TJsonTag::JSONArray;
        return ParseArray(currentTag);
    }
    if (c=='\"')
    {
        currentTag->m_tagType = TJsonTag::JSONString;
        return ParseString(currentTag);
    }
    if ( ((c>='0') && (c<='9')) || (c=='-') || (c=='+'))
    {
        currentTag->m_tagType = TJsonTag::JSONNumber;
        return ParseNumber(currentTag);
    }
    if (c=='n')
    {
        currentTag->m_stringValue = parserData + parserPosition;

        c=NextChar();
        if (c!='u') return false;

        c=NextChar();
        if (c!='l') return false;

        c=NextChar();
        if (c!='l') return false;

        c = NextChar();

        currentTag->m_tagType = TJsonTag::JSONNull;
        return true;
    }

    if (c=='t')
    {
        currentTag->m_stringValue = parserData + parserPosition;

        c=NextChar();
        if (c!='r') return false;

        c=NextChar();
        if (c!='u') return false;

        c=NextChar();
        if (c!='e') return false;

        c = NextChar();

        currentTag->m_tagType  = TJsonTag::JSONBoolean;
        currentTag->m_intValue = 1;
        return true;
    }

    if (c=='f')
    {
        currentTag->m_stringValue = parserData + parserPosition;

        c=NextChar();
        if (c!='a') return false;

        c=NextChar();
        if (c!='l') return false;

        c=NextChar();
        if (c!='s') return false;

        c=NextChar();
        if (c!='e') return false;

        c = NextChar();

        currentTag->m_tagType = TJsonTag::JSONBoolean;
        currentTag->m_intValue = 0;
        return true;
    }
    
    while(c!=0)
    {
        
        c = NextNonEmptyChar();
    }
    return false;
}

bool TJsonDoc::ParseArray(TJsonTag* currentTag)
{
    parserLevel = currentTag;

    char c = CurrentChar();
    if (c!='[')
    {
        return false;
    }
    c = NextNonEmptyChar();

    bool res;
    while(c>0)
    {        
        TJsonTag* newTag = TagPool()->CreateJsonTag(currentTag);
        res = ParseValue(newTag);
        if (!res)
        {
            TagPool()->DeleteJsonTag(newTag);
            break;
        }
        //last character is read
        c =   CurrentChar();

        //and then is set to zero
        parserData[parserPosition] = 0;

        if (c<=32)
        {
            c = NextNonEmptyChar();
        }
        if (c==']')
        {
            //end of array
            c = NextNonEmptyChar();
            if (parserLevel)
            {
                parserLevel = parserLevel->GetParentTag();
            }
            return true;
        }
        if (c!=',')
        {
            //unknown syntax
            return false;
        }

        c = NextNonEmptyChar();
    }    
    return false;
}

bool TJsonDoc::ParseNumber(TJsonTag* currentTag)
{
    currentTag->m_stringValue = parserData + parserPosition;

    char c = CurrentChar();
    bool neg = false;

    if (c=='+')
    {
        neg = true;
        c = NextNonEmptyChar();
    }
    if (c=='-')
    {
        neg = false;
        c = NextNonEmptyChar();
    }
    if ((c<'0') || (c>='9'))
    {
        return false;
    }
    int  n = 0;
    while(c>0)    
    {
        if ((c>='0') && (c<='9'))
        {
            n *= 10;
            n += c - '0';            
        } else {
            break;
        }   
        c = NextChar();
    }
    if (neg)
    {
        n = -n;
    }
    currentTag->m_intValue = n;

    return true;
}


bool TJsonDoc::ParseString(TJsonTag* currentTag)
{
    char c = CurrentChar();
    if (c!='\"')
    {
        return false;
    }
    c = NextChar();

    const char* text = parserData + parserPosition;
    while(c>0)   
    {
        if (c=='\"') 
        {
            parserData[parserPosition] = 0;
            currentTag->m_stringValue = text;            
            c = NextChar();
            return true;
        }
        c = NextChar();
    }
    return false;
}
