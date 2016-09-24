/*
 * Persistence Library / Persistence / TXMLTalkerInterface
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

#ifndef TXMLTALKER___H
#define TXMLTALKER___H

class TXMLParserInterface;
class TXMLParamMap;

class TXMLTalkerInterface
{
	public:
		virtual void OnStartElement ( TXMLParserInterface* Parser, const char *pszName, TXMLParamMap &attributes) = 0;
		virtual void OnEndElement   ( TXMLParserInterface* Parser, const char *pszName)=0;
		virtual void OnCharacterData( TXMLParserInterface* Parser, const char *pszData)=0;

};


#endif
