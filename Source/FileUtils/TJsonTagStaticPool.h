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

#ifndef TJSONSTATICPOOL___H
#define TJSONSTATICPOOL___H

#include <stdlib.h>
#include "TJsonTag.h"
#include "TJsonTagBasePool.h"

#define JSONTAGSTATICPOOLSIZE 64

class TJsonTagStaticPool : public TJsonTagBasePool
{
private:
	TJsonTag    m_tags[JSONTAGSTATICPOOLSIZE];
	short       m_tagsCount;

protected:        
    virtual TJsonTag*       GetJsonTag(short i);
    virtual unsigned short  GetJsonTagCount();        

public:
    TJsonTagStaticPool();

    virtual TJsonTag*       CreateJsonTag(TJsonTag* parentTag);
    virtual bool            DeleteJsonTag(TJsonTag* tag);
    virtual void            Clear();

};



#endif
