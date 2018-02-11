/*
 * Persistence Library / Persistence / TXMLCache
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
      
#include "TXMLCache.h"

TXMLCache::TXMLCache()
{   
	currentXmlDepth = 0;
	lastXmlTalker = NULL;
    lastName.Clear();
}

TXMLCache::~TXMLCache()
{
	currentXmlDepth = 0;
	lastXmlTalker = NULL;
}

void TXMLCache::RedirectTalker(TXMLParserInterface* parser)
{
	currentXmlDepth = 0;
    lastXmlTalker = parser->GetTalker();
	parser->SetTalker(this);
}

void TXMLCache::RestoreTalker(TXMLParserInterface* parser)
{
	parser->SetTalker(lastXmlTalker);			
}

void TXMLCache::Clear()
{
	lastName.Clear();
	Attributes.Clear();   

	TEnumerator<TXMLValuePair> it = Values.GetEnumerator();
	while(it.MoveNext())
	{
		it.Current().first.Clear();
		it.Current().second.Clear();
	}
    Values.Clear();
}

void TXMLCache::OnStartElement ( TXMLParserInterface* Parser, const char *elementName, TXMLParamMap &aAttributes)
{
	currentXmlDepth++;	
	if ( (currentXmlDepth==2) && (strcmp(elementName,"UID")==0) )
	{
		arrayFlag = true;	
	} else {
		arrayFlag = false;
		arrayValue.Clear();
		lastName = elementName;				
	}

	TEnumerator<TXMLAttributePair> it = aAttributes.GetEnumerator();
	while(it.MoveNext())
	{
		Attributes.Add(it.Current().first, it.Current().second);
	}	
}


/**
Sets value of current root tag.                                                                   
*/
void TXMLCache::OnCharacterData( TXMLParserInterface* Parser, const char* value)
{	
	if (!arrayFlag)
	{
		Values.Add(lastName, value);
		
	} else {
		if (arrayValue.Length()>0) arrayValue += ';';
		arrayValue += value;
		Values[lastName] = arrayValue;
	}	
}

/**
Method which receive event from xml event-based parser. Method closes
current tag (checks if the passed name and name of the current root tag is the same).
Current root tag is set to the tag which is parent of the current root. 
\n
\param aParser - reference to XMLParser
\param pszName - name of the tag
*/
void TXMLCache::OnEndElement   ( TXMLParserInterface* parser, const char *tagName)
{      
    currentXmlDepth--;
	if (currentXmlDepth<0)
	{	
      parser->SetTalker(lastXmlTalker);
	  lastXmlTalker->OnEndElement(parser, tagName);
	  lastXmlTalker = NULL;
   }  	
}

