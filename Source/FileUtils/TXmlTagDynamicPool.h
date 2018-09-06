/*
 * Persistence Library / Basic types / TXMLTagDynamicPool
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

#ifndef TXMLDYNAMICPOOL___H
#define TXMLDYNAMICPOOL___H

#include <stdlib.h>
#include "txmltag.h"
#include "txmltagbasepool.h"
#include "tobjectlist.h"


/**
 *  Class always creates new instances of TXMLTag. Can be used if you
 *  have a plenty of memory (e.g. hundreds of kilobytes or more)
 *
 */
class TXMLTagDynamicPool : public TXMLTagBasePool
{
    private:    
        TObjectList<TXMLTag>    Tags;

        virtual TXMLTag*        GetXMLTag(short i);
        virtual unsigned short  GetXMLTagCount();        

	public:

		TXMLTagDynamicPool();

        virtual TXMLTag*        CreateXMLTag(TXMLTag* parentTag);
        virtual bool            DeleteXMLTag(TXMLTag* tag);
        virtual void            Clear();

};

#endif
