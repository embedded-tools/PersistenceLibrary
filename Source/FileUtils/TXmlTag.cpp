/*
 * Persistence Library / Basic types / TXMLTag
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

#include "txmltag.h"
#include "stringutils.h"
#include "txmltagbasepool.h"

TXMLTagIterator::TXMLTagIterator()
{
	iteratedParent = NULL;
	iteratedName = NULL;
	iteratorIndex = 0;
}

TXMLTagIterator::TXMLTagIterator(TXMLTag* parent, const char* name)
{
    iteratedParent = parent;
    iteratedName = name;
    iteratorIndex = 0;
}

TXMLTag* TXMLTagIterator::First()
{
	iteratorIndex = 0;
	unsigned short n = TXMLTag::TagPool->GetChildCount(iteratedParent);
	for(unsigned short i = 0; i<n; i++)
	{
		TXMLTag* tag = TXMLTag::TagPool->GetChild(iteratedParent,i);
		if (iteratedName!=NULL)
		{
			if (strcmp(tag->GetName(), iteratedName)!=0) continue;
		}		
		return tag;
	}
	return NULL;
}

TXMLTag* TXMLTagIterator::Next()
{
	iteratorIndex++;

	unsigned short counter = iteratorIndex;
	unsigned short n = TXMLTag::TagPool->GetChildCount(iteratedParent);
	for(unsigned short i = 0; i<n; i++)
	{
		TXMLTag* tag = TXMLTag::TagPool->GetChild(iteratedParent,i);
		if (iteratedName!=NULL)
		{
			if (strcmp(tag->GetName(), iteratedName)!=0) continue;
		}		
		if (counter == 0)
		{
			return tag;
		}
		counter--;		
	}
	return NULL;
}

TXMLTagBasePool* TXMLTag::TagPool = NULL;

TXMLTag::TXMLTag()
{
	Init();
}

TXMLTag::~TXMLTag()
{
    
}

void TXMLTag::Init()
{
    Name = NULL;
    Value = NULL;
	ParentTag = NULL;
    FirstAttribute = NULL;
    LastAttribute = NULL;
}

void TXMLTag::SetTagPool(TXMLTagBasePool* tagPool)
{
	TagPool = tagPool;
}

const char* TXMLTag::GetName()
{
    return Name;
}

TShortString TXMLTag::GetNameAsString()
{
    return Name;
}

const char* TXMLTag::GetValue()
{
    return Value;
}

TShortString TXMLTag::GetValueAsString()
{
    return Value;
}

short TXMLTag::GetValueAsShortInt(short defaultValue)
{
    if (Value!=NULL) return StrToShortIntDef(Value, defaultValue);
    return defaultValue;
}

long TXMLTag::GetValueAsLongInt(long defaultValue)
{
    if (Value!=NULL) return StrToLongIntDef(Value, defaultValue);
    return defaultValue;
}

const char* TXMLTag::GetAttributeValue(const char* attributeName)
{
    char* pp = (char*)FirstAttribute;
    char* result = NULL;
    while(true)
    {
        while(*pp==32)      
        {
            pp++;
        }    
		if (pp>=LastAttribute) break;

        unsigned short attrNameLen = strlen(attributeName);
        bool nameMatch = true;
        for(unsigned short i = 0; i<attrNameLen; i++)
        {
            if (attributeName[i]!=pp[i])
            {
                nameMatch = false;
                break;
            }
        }    
        if ((pp[attrNameLen]!='=') && (pp[attrNameLen]!=' '))
        {
            nameMatch = false;
        }
        while((*pp!='=') && (*pp!=0))
        {
            pp++;
        }
        pp++;
        while(*pp==32)      
        {
            pp++;
        }    
        if ((*pp=='\"') || (*pp=='\''))
        {
            pp++;
        }
        result = pp;
        while(*pp!=0)
        {
            pp++;
        }
        pp++;
        if (nameMatch)
        {
            return result;
        }
    }
    return NULL;
}

TShortString TXMLTag::GetAttributeValueAsString(const char* attributeName)
{
    return GetAttributeValue(attributeName);
}

short TXMLTag::GetAttributeValueAsShortInt(const char* attributeName, short defaultValue)
{
	const char* value = GetAttributeValue(attributeName);
	if (value!=NULL) return StrToShortInt(value);
	return defaultValue;    
}

long TXMLTag::GetAttributeValueAsLongInt(const char* attributeName, long defaultValue)
{
    const char* value = GetAttributeValue(attributeName);
    if (value!=NULL) return StrToLongInt(value);
    return defaultValue;    
}

TXMLTag* TXMLTag::GetParentTag()
{
    return ParentTag;
}

short TXMLTag::GetChildCount()
{
	return TagPool->GetChildCount(this);
}

TXMLTag* TXMLTag::GetChild(short i)
{
	return TagPool->GetChild(this, i);
}

TXMLTag* TXMLTag::FindIterationParent(TXMLTag* parentTag, const char* xpath)
{
	if (xpath==NULL)
	{
		return parentTag;
	} else 
	{
		if (xpath[0]=='/')
		{
			xpath++;
		}
	}
    short n = StrPos('/',xpath);
    if (n==-1)
    {
        return parentTag;
    } else 
    {
        TShortString parentName(xpath, n);
        if (parentName.Length()==0)
        {
            return parentTag;
        }

        short childs = TagPool->GetChildCount(parentTag);
        for(short i = 0; i<childs; i++)
        {
            TXMLTag* newParent = TagPool->GetChild(parentTag, i);
            if (parentName==newParent->GetName())
            {
                return FindIterationParent(newParent, xpath+n+1);                         
            }
        }
    }
    return NULL;
}

const char* TXMLTag::ExtractChildName (const char* xpath)
{
    if (xpath==NULL)
    {
        return NULL;
    }
    unsigned short n = (unsigned short)strlen(xpath);
    for(unsigned short i = n-1; i!=0; i--)
    {
        if (xpath[i]=='/')
        {
            return &xpath[i+1];
        }
    }
    return xpath;
}

TXMLTag* TXMLTag::SelectNode(const char* xpath)
{
	if (xpath[0]=='/')
	{
		xpath++;
	}
    TXMLTag* iteratedParent = FindIterationParent(this, xpath);
	if (iteratedParent==NULL)
	{
		return NULL;
	}
    const char* iteratedName = ExtractChildName(xpath);

    unsigned short n = TagPool->GetChildCount(iteratedParent);
    for(unsigned short i = 0; i<n; i++)
    {
        TXMLTag* child = TagPool->GetChild(iteratedParent, i);
        if (iteratedName!=NULL)
        {
            if (strcmp(iteratedName, child->GetName())!=0) continue;
        }
        return child;        
    }
    return NULL;
}

TXMLTagIterator* TXMLTag::SelectNodes(const char* xpath, bool no_malloc)
{
	if (xpath[0]=='/')
	{
		xpath++;
	}
    TXMLTag* iteratedParent = FindIterationParent(this, xpath);
	if (iteratedParent==NULL)
	{
		return NULL;
	}
    const char* iteratedName = ExtractChildName(xpath);
    if (iteratedName!=NULL)
    {
        if (strlen(iteratedName)==0)
        {
            iteratedName = NULL;
        }
    }
	if (no_malloc)
	{		
		/*
		static TXMLTagIterator* staticIterator = NULL; 
		if (staticIterator==NULL) staticIterator = new TXMLTagIterator();
		staticIterator->iteratedName   = iteratedName;
		staticIterator->iteratedParent = iteratedParent;
		staticIterator->iteratorIndex  = 0;
		return staticIterator;
		*/
	}
    TXMLTagIterator* iterator = new TXMLTagIterator(iteratedParent, iteratedName);
    return iterator;
}

unsigned short TXMLTag::CountNodes(const char* xpath)
{
	if (xpath[0]=='/')
	{
		xpath++;
	}
	TXMLTag* iteratedParent = FindIterationParent(this, xpath);
	if (iteratedParent==NULL)
	{
		return 0;
	}
	const char* iteratedName = ExtractChildName(xpath);
	if (iteratedName==NULL)
	{
		return 0;
	}
	unsigned short count = 0;
	unsigned short n = TagPool->GetChildCount(iteratedParent);
	for(unsigned short i = 0; i<n; i++)
	{
		TXMLTag* tag = TagPool->GetChild(iteratedParent, i);
		if (iteratedName)
		{
			if (strcmp(iteratedName, tag->GetName())!=0)
			{
				continue;
			}
		}
		count++;
	}
	return count;
}

void TXMLTag::SaveToStream(TStream& stream, int indent)
{
	for(int i = indent; i>0; i--)
	{
		stream.WritePlainText("  ");
	}
	stream.WriteChar('<');
	stream.WritePlainText(GetName());
	
	if (FirstAttribute)
	{
		char* pp = (char*)FirstAttribute;
		char* result = NULL;
		char separator=' ';
		while(true)
		{
			stream.WriteChar(' ');
			while(*pp==32)      
			{
				pp++;
			}
			if (pp>=LastAttribute) break;

			while((*pp!='=') && (*pp!=0))
			{				
				stream.WriteChar(*pp);
				pp++;
			}
			stream.WriteChar('=');
			stream.WriteChar('\"');
			pp++;
			while(*pp==32)      
			{
				pp++;
			}    
			if ((*pp=='\"') || (*pp=='\''))
			{
				separator=*pp;
				pp++;
			}
			result = pp;
			while((*pp!=0))
			{
				switch(*pp)
				{
					case '<': stream.WritePlainText("&lt;"); break;
					case '>': stream.WritePlainText("&gt;"); break;
					case '&': stream.WritePlainText("&amp;"); break;
					case '\"': stream.WritePlainText("&quot;"); break;
					default: stream.WriteChar(*pp); break;
				}
				pp++;
			}
			pp++;
			stream.WriteChar('\"');
		}		
		const char* name = GetName();
		if (name[0]=='?')
		{
			stream.WritePlainText(" ?>\r\n");
			return;
		}
	}

	stream.WriteChar('>');
	const char* value = GetValue();
	if (value[0])
	{
		while(*value!=0)
		{
			switch(*value)
			{
				case '<': stream.WritePlainText("&lt;"); break;
				case '>': stream.WritePlainText("&gt;"); break;
				case '&': stream.WritePlainText("&amp;"); break;
				case '\"': stream.WritePlainText("&quot;"); break;
				default: stream.WriteChar(*value);
			}
			value++;
		}
	} else {
		stream.WritePlainText("\r\n");
		int n = TagPool->GetChildCount(this);
		for (int i = 0; i<n; i++)
		{
			TXMLTag* child = TagPool->GetChild(this, i);
			child->SaveToStream(stream, indent + 1);
		}
		for(int i = indent; i>0; i--)
		{
			stream.WritePlainText("  ");
		}
	}	
	stream.WriteChar('<');
	stream.WriteChar('/');
	stream.WritePlainText(GetName());
	stream.WritePlainText(">\r\n");

}



