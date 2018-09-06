/*
 * Persistence Library / Basic types / TXMLTagStaticPool
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

#include "txmltagstaticpool.h"

TXMLTagStaticPool::TXMLTagStaticPool()
{
    memset(m_tags, 0, sizeof(m_tags));
}

TXMLTag* TXMLTagStaticPool::GetXMLTag(short i)
{
    return &m_tags[i];
}

unsigned short  TXMLTagStaticPool::GetXMLTagCount()
{
    unsigned short count = 0;
    for(unsigned short i = 0; i<XMLTAGSTATICPOOLSIZE; i++)
    {
        if (m_tags[i].GetName()==NULL) continue;
        count++;
    }
    return count;
}

TXMLTag* TXMLTagStaticPool::CreateXMLTag(TXMLTag* parentTag)
{
    TXMLTag* newXmlTag = NULL;
    for(int i = 0; i<XMLTAGSTATICPOOLSIZE; i++)
    {
        if (m_tags[i].GetName()==NULL)
        {
            newXmlTag=&m_tags[i];
			SetXMLTagParent(newXmlTag, parentTag);
            m_tagsCount++;
            break;
        }
    }

    return newXmlTag;
}

bool TXMLTagStaticPool::DeleteXMLTag(TXMLTag* tag)
{
    SetXMLTagName(tag, NULL);
	SetXMLTagParent(tag, NULL);
    m_tagsCount--;
    return true;
}

void TXMLTagStaticPool::Clear()
{
    m_tagsCount = 0;
    for(int j = 0; j<XMLTAGSTATICPOOLSIZE; j++)
    {
        SetXMLTagName(&m_tags[j], NULL);
		SetXMLTagParent(&m_tags[j], NULL);
    }    
}





