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


#ifndef TXMLBUILDER___H
#define TXMLBUILDER___H

#include "TXMLParserInterface.h"
#include "TXMLTalkerInterface.h"
#include "TStream.h"
#include "TXmlDoc.h"
#include "TXMLParser.h"
#include "TPair.h"

#define XMLCACHEMAXVALUECOUNT 16
#define XMLCACHEMAXATTRCOUNT 4
#define XMLMAXARRAYLENGTH 16

typedef TXMLParamMap TXMLAttributeDictionary;
typedef TPair<TCustomString<XMLMAXNAMESIZE>, TCustomString<XMLMAXATTRIBUTESIZE> > TXMLAttributePair;

typedef TStaticDictionary<TCustomString<XMLMAXNAMESIZE>, TCustomString<XMLMAXVALUESIZE>, XMLCACHEMAXVALUECOUNT> TXMLValueDictionary;
typedef TPair<TCustomString<XMLMAXNAMESIZE>, TCustomString<XMLMAXVALUESIZE> > TXMLValuePair;


class TXMLCache : public TXMLTalkerInterface
{
	private:
		bool									arrayFlag;
		TCustomString<XMLMAXARRAYLENGTH*11+2>	arrayValue;

		int								currentXmlDepth;
		TXMLTalkerInterface*			lastXmlTalker;
		TCustomString<XMLMAXNAMESIZE>	lastName;

	public:
		TXMLCache();
		~TXMLCache();

		
		TXMLAttributeDictionary Attributes;
		TXMLValueDictionary Values;

		void RedirectTalker(TXMLParserInterface* parser);
		void RestoreTalker(TXMLParserInterface* parser);

		void Clear();
		virtual void OnStartElement ( TXMLParserInterface* Parser, const char *pszName, TXMLParamMap &attributes);
		virtual void OnEndElement   ( TXMLParserInterface* Parser, const char *pszName);
		virtual void OnCharacterData( TXMLParserInterface* Parser, const char *pszData);
};

#endif
