/*
 * Persistence Library / Persistence / TXMLCache
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


#ifndef TXMLBUILDER___H
#define TXMLBUILDER___H

#include "TXMLParserInterface.h"
#include "TXMLTalkerInterface.h"
#include "tstream.h"
#include "txmldoc.h"
#include "TXMLParser.h"

#define XMLCACHEMAXVALUECOUNT 16
#define XMLCACHEMAXATTRCOUNT 4
#define XMLMAXARRAYLENGTH 16

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

		
		TStaticDictionary<TCustomString<XMLMAXNAMESIZE>, TCustomString<XMLMAXVALUESIZE>, XMLCACHEMAXATTRCOUNT>  Attributes;
		TStaticDictionary<TCustomString<XMLMAXNAMESIZE>, TCustomString<XMLMAXVALUESIZE>, XMLCACHEMAXVALUECOUNT> Values;

		void RedirectTalker(TXMLParserInterface* parser);
		void RestoreTalker(TXMLParserInterface* parser);

		void Clear();
		virtual void OnStartElement ( TXMLParserInterface* Parser, const char *pszName, TXMLParamMap &attributes);
		virtual void OnEndElement   ( TXMLParserInterface* Parser, const char *pszName);
		virtual void OnCharacterData( TXMLParserInterface* Parser, const char *pszData);
};

#endif
