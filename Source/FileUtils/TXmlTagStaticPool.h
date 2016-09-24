/*
 * Persistence Library / Basic types / TXMLTagStaticPool
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

#ifndef TXMLSTATICPOOL___H
#define TXMLSTATICPOOL___H

#include <stdlib.h>
#include "txmltag.h"
#include "txmltagbasepool.h"

#define XMLTAGSTATICPOOLSIZE 64


/**
 *  Class preallocated fixed number of TXMLTag. These XML Tag instances are reused
 *  for all loaded Xml documents. 
 *  
 *  Such reusing of existing instances avoids memory fragmentation on embedded 
 *  systems with low memory size.
 */
class TXMLTagStaticPool : public TXMLTagBasePool
{
    private:    
        TXMLTag    Tags[XMLTAGSTATICPOOLSIZE];
        short      TagsCount;

        virtual TXMLTag*        GetXMLTag(short i);
        virtual unsigned short  GetXMLTagCount();        

	public:

		TXMLTagStaticPool();

        virtual TXMLTag*        CreateXMLTag();
        virtual bool            DeleteXMLTag(TXMLTag* tag);
        virtual void            Clear();

};

#endif
