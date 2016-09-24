/*
 * Persistence Library / Basic types / TXMLTagBasePool 
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

#ifndef TXMLBASEPOOL___H
#define TXMLBASEPOOL___H

#include <stdlib.h>
#include "txmltag.h"


/**
 *  Class designed for creating instances of XmlTags. It allows to create both
 *  static or dynamic xml tag pool.
 */
class TXMLTagBasePool
{
    protected:
        void SetXMLTagName(TXMLTag* tag, const char* tagName, TXMLTag* tagParent);
        virtual TXMLTag*        GetXMLTag(short i) = 0;
        virtual unsigned short  GetXMLTagCount() = 0;        

	public:

        virtual TXMLTag*        CreateXMLTag(const char* tagName, TXMLTag* parentTag) = 0;
        virtual bool            DeleteXMLTag(TXMLTag* tag) = 0;
        virtual void            Clear() = 0;

        unsigned short          GetChildCount(TXMLTag* parentTag);
        TXMLTag*                GetChild(TXMLTag* parentTag, unsigned short index);

};

#endif
