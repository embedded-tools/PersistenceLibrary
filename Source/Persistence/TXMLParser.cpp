/*
 * Persistence Library / Persistence / TXMLParser
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

#include "TXMLParser.h"

unsigned char					TXMLParser::XMLBuffer[XMLMAXBUFFERSIZE];
int	   							TXMLParser::XMLBufferDataSize = 0;
int								TXMLParser::XMLBufferPosition = 0;
TCustomString<XMLMAXNAMESIZE>	TXMLParser::XMLNameStack[XMLMAXLEVELS];
int								TXMLParser::XMLNameStackPosition = 0;

TXMLParser::TXMLParser()
{
	LastError = exeNoError;
	currentTalker = NULL;
}

void TXMLParser::SetTalker(TXMLTalkerInterface* pTalker)
{
	currentTalker = pTalker;
}

TXMLTalkerInterface* TXMLParser::GetTalker()
{
	return currentTalker;
}

void TXMLParser::PushTagName(TCustomString<XMLMAXNAMESIZE> &aName)
{
	if (XMLNameStackPosition==XMLMAXNAMESTACKSIZE) 
	{
		LastError=exeTooMuchTreeLevels;
		return;	
	};
	XMLNameStack[XMLNameStackPosition]=aName;
	XMLNameStackPosition++;
}

void TXMLParser::PopTagName(TCustomString<XMLMAXNAMESIZE> &aName)
{
	if (XMLNameStackPosition==0) 
	{
		LastError=exeMoreEndTagsThanBeginTags;
		return;	
	}
	aName = XMLNameStack[XMLNameStackPosition-1];
	XMLNameStackPosition--;
}

void TXMLParser::SkipSpaces()
{
	while (CurrentChar()<=' ') 
	{
		NextChar();
		if (LastError!=exeNoError) break;
	}
}

void TXMLParser::ReadTagName(TCustomString<XMLMAXNAMESIZE> &aName, unsigned char &delimiter)
{
	unsigned char  c;

	aName.Clear();

	//tagName read
	while(LastError==exeNoError)
	{		
		c = CurrentChar();
		if (c==' ') 
		{
			delimiter=' ';
			break;
		}
		if (c=='>') 
		{
			delimiter='>';
			break;
		};
		if (c=='=') 
		{
			delimiter='=';
			break;
		}
		aName += c;
		NextChar();

		if (LastError==exeEndOfFile)
		{
			LastError=exeUnterminatedTagName;
		}
	}	
}

void TXMLParser::ReadTagValue (TCustomString<XMLMAXVALUESIZE> &aResValue)
{
	aResValue.Clear();

	//first white spaces are ignored (CR/LFs are ignored also)
	//(white space can be at inserted at beginning by using &nbsp;)
	SkipSpaces();

	//read chars until delimiter is found
	while (LastError==exeNoError) 
	{
		unsigned char c = CurrentChar();	

		//standard end of tag value
		if (c=='<') break;

		//tag value must not contain CR/LF
		if ((c==13) || (c==10))
		{
			//unterminated string
			LastError=exeCRLFInTagValue;
			break;
		}

		if (c<32) c = '?';

		if (c=='&')
		{
			static TCustomString<8> specialCharacter;
			specialCharacter.Clear();
			do
			{										
				specialCharacter+=c;
				if (c==';') break;
				c = NextChar();		
			} while(LastError==exeNoError);

			if      (specialCharacter=="&nbsp;") c = ' '; 
			else if (specialCharacter=="&lt;")   c = '<'; 
			else if (specialCharacter=="&gt;")   c = '>'; 
			else if (specialCharacter=="&amp;")  c = '&'; 
			else if (specialCharacter=="&quot;") c = '"';
		}	
		
		aResValue+=c;
		NextChar();

		if (LastError==exeEndOfFile)
		{
			LastError=exeEndTagNotFound;
		}
	}
}

void TXMLParser::ReadAttributeValue (TCustomString<XMLMAXATTRIBUTESIZE> &aResValue)
{
	aResValue.Clear();

	//supported attribute syntaxes:
	//  <tag attribute='1'>
	//  <tag attribute="1">
	unsigned char delimiter = CurrentChar();	
	if ((delimiter!='"') && (delimiter!=APOSTROPHE)) 
	{ 
		LastError=exeInvalidAttribute;
		return;
	};

	//step over delimiter
	NextChar();

	//read chars until delimiter is found
	while (LastError==exeNoError) 
	{
		unsigned char c = CurrentChar();
		if (c==delimiter) 
		{
			//step over delimiter
			NextChar();
			break;
		}
		if ((c==13) || (c==10))
		{
			//unterminated string
			LastError=exeCRLFInAttributeValue;
			break;
		}
		if (c<32) c = '?';
		aResValue+=CurrentChar();
		NextChar();
		if (LastError==exeEndOfFile)
		{
			LastError = exeUnterminatedAttributeValue;
		}
	}
}


EXMLErrors TXMLParser::GetLastError()
{
	return LastError;   
}

void TXMLParser::ClearCache()
{
	XMLBufferPosition=0;
	XMLBufferDataSize=0;
}

bool TXMLParser::Parse (const char* pData, int dataLength)
{
	LastError = exeNoError;

	if (XMLMINBUFFERSIZE>XMLMAXBUFFERSIZE)
	{
		return false;
	}

	if (pData==NULL) 
	{
		return false;
	}
	if  ((XMLBufferDataSize+dataLength)>XMLMAXBUFFERSIZE) 
	{
		do
		{
			int needed = XMLMAXBUFFERSIZE - XMLBufferDataSize;
			if (dataLength<needed) needed=dataLength;

			memcpy (XMLBuffer+XMLBufferDataSize, pData, needed);
			XMLBufferDataSize = XMLBufferDataSize + needed;
			dataLength = dataLength-needed;
			pData=pData+needed;

			if (XMLBufferDataSize>=XMLMAXBUFFERSIZE) 
			{
				EvaluateTags();	
				if (LastError>=exeError) 
				{
					return false;			
				}
			}
		}
		while (dataLength>0);
	} else {
		memcpy(XMLBuffer+XMLBufferDataSize, pData, dataLength);
		XMLBufferDataSize+=dataLength;	
		if (XMLBufferDataSize==XMLMAXBUFFERSIZE) 
		{
			EvaluateTags();
			if (LastError>=exeError) 
			{
				return false;			
			}
		}
	}	
	if (LastError==exeEndOfFile)
	{ 
		LastError=exeNoError;
	}
	return LastError==exeNoError;
}


void TXMLParser::FlushCache()
{
	EvaluateTags(true);  	
	ClearCache();
}

unsigned char TXMLParser::CurrentChar()
{
	if (XMLBufferDataSize==0) 
	{
		LastError=exeEndOfFile;
		return 0;
	}
	unsigned char res=0;
	if (XMLBufferPosition<XMLBufferDataSize) 
	{	
		res=XMLBuffer[XMLBufferPosition];
	} else {
		LastError=exeEndOfFile;
	}	
	return res;
}

unsigned char TXMLParser::NextChar()
{
	if (LastError==exeNoError)
	{
		XMLBufferPosition++;
	}
	return CurrentChar();
}


bool TXMLParser::EvaluateTags(bool aEvaluateAll)
{
	bool res=true;
	bool next=false;

	static TCustomString<XMLMAXNAMESIZE> tagname;
	static TXMLParamMap parammap;
	do 
	{
		tagname.Clear();
		parammap.Clear();
		
		bool begintag = false;
		bool endtag   = false;        
		if (FindNextTag(tagname, parammap, begintag, endtag)==false) 
		{
			res=false;
		};		
		if (LastError!=exeNoError) break;
		//next tag can be evaluated if: a) end of the buffer is in safe distance
		//                              b) flag evaluate all is set
		next = ((XMLBufferDataSize-XMLBufferPosition)>=XMLMAXTAGSIZE) || aEvaluateAll;
	} 
	while (next==true);

	//moves unparsed data to the beginning of the buffer
	memcpy(XMLBuffer, XMLBuffer+XMLBufferPosition, XMLBufferDataSize-XMLBufferPosition);
	XMLBufferDataSize -= XMLBufferPosition;
	XMLBufferPosition=0;

	return res;
}

bool TXMLParser::FindNextTag(TCustomString<XMLMAXNAMESIZE> &aTagName, TXMLParamMap &aAttributes, bool &beginTag, bool &endTag)
{
	beginTag = false;
	endTag   = false;
	aTagName.Clear();
	aAttributes.Clear();

	unsigned char delimiter = 0;

	while(LastError==exeNoError)
	{	
		//looks for beginning of XML tag
		SkipSpaces();
		if (LastError!=exeNoError) 
		{
			return (LastError<exeError);
		}
		
		//is it xml tag beginning?
		if (CurrentChar()!='<') 
		{
			LastError = exeBeginTagNotFound;
			return false;
		}	

		//determines if it is begin or end of xml tag ("<tag..." or "</tag...")
		NextChar();
		if (CurrentChar()=='/') 
		{
			endTag=true;
			NextChar();
		} else {
			beginTag=true;
		}	

		//reads name of the tag
		ReadTagName(aTagName, delimiter);
		if (LastError!=exeNoError) return false;

		//is it tag or comment?   	   	
		if (aTagName=="!--")
		{
			//comment was found. In such case ignores all xml syntaxes until "-->" is found
			char buf[3]; 
			buf[0]=0;
			buf[1]=0;
			buf[2]=0;	   

			while (LastError==exeNoError)
			{
				buf[0]=buf[1];
				buf[1]=buf[2];
				buf[2]=CurrentChar();
				NextChar();
				if ((buf[0]=='-') && (buf[1]=='-') && (buf[2]=='>'))
				{            
					break;
				}		      
			}			
			//valid tag name not foound (it was just a comment)
			//therefore in next iteration tries to find valid xml tag name
			continue;
		} else {

			//tag was found
			break;
		}
	}

	if (endTag)
	{
		static TCustomString<XMLMAXNAMESIZE> oldTag;
		PopTagName(oldTag);
		if (aTagName!=oldTag)
		{
			//begin and end tag differs
			LastError = exeBeginAndEndTagDiffers;
		}
	}
	if (delimiter=='>')
	{
		NextChar();
	}

	if (LastError!=exeNoError) return false;

	if (beginTag & (delimiter==' '))
	{	
		//tries to read attributes		
		while (LastError==exeNoError) 
		{
			//skips white spaces before attribute name
			SkipSpaces();		
			if (CurrentChar()=='/') 
			{
				endTag=true;
				NextChar();
				continue;
			}
			if (CurrentChar()=='>') 
			{
				//steps over '>'
				NextChar();
				break;
			}

			static TCustomString<XMLMAXNAMESIZE> attrName;
			static TCustomString<XMLMAXATTRIBUTESIZE> attrValue;
			//reads attribute name
			ReadTagName (attrName, delimiter);

			//expected delimiter is '='
			if (delimiter=='=') 
			{				
				//step over '='
				NextChar();   		

				//reads attribute value
				ReadAttributeValue(attrValue);				

				//and stores into attributes list
				aAttributes.Add(attrName, attrValue);
			} else {
				LastError = exeInvalidAttribute;
			}			
		}
	} 

	if (LastError!=exeNoError) return false;

	//reads xml tag value
	static TCustomString<XMLMAXVALUESIZE> tagValue;
	if (beginTag && !endTag) 
	{
		//stores tag name into stack
		PushTagName(aTagName);
		SkipSpaces();
		if (CurrentChar()=='<') 
		{
			//value is empty
			tagValue.Clear();
		} else {
			//reads xml tag value
			ReadTagValue(tagValue);
		}
	}
	if (LastError!=exeNoError) return false;

	//now raises event according to xml tag type:
	// - OnStartElement
	// - OnCharacterData
	// - OnEndElement
	if (beginTag) 
	{	           
		if (currentTalker!=NULL)
		{
			currentTalker->OnStartElement(this, aTagName.ToPChar(), aAttributes);
		}
	}	
	if (beginTag && !endTag) 
	{
		if (tagValue.Length()>0) 
		{	      
			if (currentTalker!=NULL)
			{
				currentTalker->OnCharacterData(this, tagValue.ToPChar());	
			}
		}
	}	
	if (endTag) 
	{
		if (currentTalker!=NULL)
		{
			currentTalker->OnEndElement(this, aTagName.ToPChar());
		}
	}
	return true;
}
