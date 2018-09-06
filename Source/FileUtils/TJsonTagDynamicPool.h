#ifndef TJSONTAG_DYNAMICPOOL___H
#define TJSONTAG_DYNAMICPOOL___H

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

#include <stdlib.h>
#include "TList.h"
#include "tjsontag.h"
#include "TJsonTagBasePool.h"


class TJsonTagDynamicPool : public TJsonTagBasePool
{
private:
	TList<TJsonTag*>  m_tags;

protected:        
    virtual TJsonTag*       GetJsonTag(short i);
    virtual unsigned short  GetJsonTagCount();        

public:
    TJsonTagDynamicPool();
    ~TJsonTagDynamicPool();

    virtual TJsonTag*       CreateJsonTag(TJsonTag* parentTag);
    virtual bool            DeleteJsonTag(TJsonTag* tag);
    virtual void            Clear();

};

#endif