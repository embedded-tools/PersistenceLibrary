/*
 * Persistence Library / Basic types / TXMLTagBasePool 
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

#ifndef TXMLBASEPOOL___H
#define TXMLBASEPOOL___H

#include <stdlib.h>
#include "TXmlTag.h"


/**
 *  Class designed for creating instances of XmlTags. It allows to create both
 *  static or dynamic xml tag pool.
 */
class TXMLTagBasePool
{
	friend class TXMLDoc;

    protected:
		void					SetXMLTagName(TXMLTag* tag, const char* tagName); 
		void                    SetXMLTagParent(TXMLTag* tag, TXMLTag* tagParent);
        virtual TXMLTag*        GetXMLTag(short i) = 0;
        virtual unsigned short  GetXMLTagCount() = 0;        

	public:

        virtual TXMLTag*        CreateXMLTag(TXMLTag* parentTag) = 0;
        virtual bool            DeleteXMLTag(TXMLTag* tag) = 0;
        virtual void            Clear() = 0;

        unsigned short          GetChildCount(TXMLTag* parentTag);
        TXMLTag*                GetChild(TXMLTag* parentTag, unsigned short index);

};

#endif
