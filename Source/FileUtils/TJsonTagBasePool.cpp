#include "TJsonTagBasePool.h"

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

#include "txmltagbasepool.h"


void TJsonTagBasePool::SetJsonTagName(TJsonTag* tag, const char* tagName)
{
    tag->m_name      = tagName;
}

void TJsonTagBasePool::SetJsonTagParent(TJsonTag* tag, TJsonTag* tagParent)
{
	tag->m_parentTag = tagParent;
}

unsigned short TJsonTagBasePool::GetChildCount(TJsonTag* parentTag)
{
    unsigned short count = 0;
    unsigned short n = GetJsonTagCount();
    for(unsigned short i = 0; i<n; i++)
    {		
        TJsonTag* tag = GetJsonTag(i);
        if (tag==NULL) continue;
        if (tag->GetParentTag()!=parentTag) continue;
        count++;        
    }	
    return count;
}

TJsonTag* TJsonTagBasePool::GetChild(TJsonTag* parentTag, unsigned short index)
{
    unsigned short n = GetJsonTagCount();
    for(int i = 0; i<n; i++)
    {		
        TJsonTag* tag = GetJsonTag(i);
        if (tag->GetParentTag()!=parentTag) continue;
        if (index==0)
        {
            return tag;
        }
        index--;
    }	
    return NULL;
}