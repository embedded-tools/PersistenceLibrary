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

#include "txmltagstaticpool.h"

TXMLTagStaticPool::TXMLTagStaticPool()
{
    memset(Tags, 0, sizeof(Tags));
}

TXMLTag* TXMLTagStaticPool::GetXMLTag(short i)
{
    return &Tags[i];
}

unsigned short  TXMLTagStaticPool::GetXMLTagCount()
{
    unsigned short count = 0;
    for(unsigned short i = 0; i<XMLTAGSTATICPOOLSIZE; i++)
    {
        if (Tags[i].GetName()==NULL) continue;
        count++;
    }
    return count;
}

TXMLTag* TXMLTagStaticPool::CreateXMLTag(const char* name, TXMLTag* parentTag)
{
    if (name==NULL)
    {
        return NULL;
    }
    TXMLTag* newXmlTag = NULL;
    for(int i = 0; i<XMLTAGSTATICPOOLSIZE; i++)
    {
        if (Tags[i].GetName()==NULL)
        {
            newXmlTag=&Tags[i];
            SetXMLTagName(newXmlTag, name, parentTag);
            TagsCount++;
            break;
        }
    }
    return newXmlTag;
}

bool TXMLTagStaticPool::DeleteXMLTag(TXMLTag* tag)
{
    SetXMLTagName(tag, NULL, NULL);
    TagsCount--;
    return true;
}

void TXMLTagStaticPool::Clear()
{
    TagsCount = 0;
    for(int j = 0; j<XMLTAGSTATICPOOLSIZE; j++)
    {
        SetXMLTagName(&Tags[j], NULL, NULL);
    }    
}





