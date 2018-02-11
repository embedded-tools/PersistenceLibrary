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

#ifndef TXMLPARSER____H
#define TXMLPARSER____H

#define XMLMAXLEVELS    16
#define XMLMAXNAMESIZE  20
#define XMLMAXNAMESTACKSIZE 32
#define XMLMAXVALUESIZE	100
#define XMLMAXATTRIBUTECOUNT 4
#define XMLMAXATTRIBUTESIZE	20
#define XMLMINBUFFERSIZE ((XMLMAXVALUESIZE * 2) + (XMLMAXNAMESIZE * 2) + XMLMAXATTRIBUTECOUNT*(XMLMAXNAMESIZE + 4 + XMLMAXATTRIBUTESIZE) + 16)
#define XMLMAXBUFFERSIZE 512
#define XMLMAXTAGSIZE (XMLMAXNAMESIZE*2 + XMLMAXVALUESIZE + 5 + XMLMAXATTRIBUTECOUNT*(XMLMAXNAMESIZE + 4 + XMLMAXATTRIBUTESIZE))

#define APOSTROPHE 39

#include "TCustomstring.h"
#include "TStaticDictionary.h"
#include "TXMLTalkerInterface.h"
#include "TXMLParserInterface.h"


enum EXMLErrors 
{ 
	exeNoError = 0,
	exeEndOfFile,
		
	//errors
	exeError=0x100,
	exeBeginTagNotFound,
	exeEndTagNotFound, 
	exeInvalidAttribute,
	exeUnterminatedAttributeValue,
	exeUnterminatedTagName,
	exeCRLFInAttributeValue,	
	exeCRLFInTagValue,		
	exeTooMuchTreeLevels,
	exeBeginAndEndTagDiffers,
	exeMoreEndTagsThanBeginTags

};

class TXMLParamMap : public TStaticDictionary<TCustomString<XMLMAXNAMESIZE>,TCustomString<XMLMAXATTRIBUTESIZE>,XMLMAXATTRIBUTECOUNT>
{

};


class TXMLParser	: public TXMLParserInterface                      
{

private:

	static unsigned char					XMLBuffer[XMLMAXBUFFERSIZE];
	static int	   							XMLBufferDataSize;
	static int								XMLBufferPosition;
	static TCustomString<XMLMAXNAMESIZE>	XMLNameStack[XMLMAXLEVELS];
	static int								XMLNameStackPosition;	

	TXMLTalkerInterface*					currentTalker;	

	unsigned char CurrentChar();
	unsigned char NextChar();
	void SkipSpaces();
	void ReadTagName(TCustomString<XMLMAXNAMESIZE> &aName, unsigned char &delimiter);
	void ReadTagValue(TCustomString<XMLMAXVALUESIZE> &aResValue);
	void ReadAttributeValue(TCustomString<XMLMAXATTRIBUTESIZE> &aResValue);	

	bool EvaluateTags(bool aEvaluateAll=false);
	bool FindNextTag(TCustomString<XMLMAXNAMESIZE> &aTagName, TXMLParamMap &aAttributes, bool &beginTag, bool &endTag);

	void ClearCache();
	void PushTagName(TCustomString<XMLMAXNAMESIZE> &aName);	
	void PopTagName(TCustomString<XMLMAXNAMESIZE> &aName);

public:

	EXMLErrors LastError;

	TXMLParser();

	virtual void SetTalker(TXMLTalkerInterface* pTalker);
	virtual TXMLTalkerInterface* GetTalker();	

	virtual bool Parse (const char* pData, int dataLength);	
	void FlushCache();


	EXMLErrors GetLastError();

};


#endif
