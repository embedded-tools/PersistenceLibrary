#include "TJsonTagDynamicPool.h"

TJsonTagDynamicPool::TJsonTagDynamicPool()
{
    
}

TJsonTagDynamicPool::~TJsonTagDynamicPool()
{
    Clear();
}


TJsonTag* TJsonTagDynamicPool::GetJsonTag(short i)
{
    if (i<0) return NULL;
    if (i>=m_tags.Count()) return NULL;

    return m_tags[i];
}

unsigned short TJsonTagDynamicPool::GetJsonTagCount()
{
    return m_tags.Count();
}

TJsonTag* TJsonTagDynamicPool::CreateJsonTag(TJsonTag* parentTag)
{
    TJsonTag* result = new TJsonTag();
    m_tags.Add(result);
    SetJsonTagParent(result, parentTag);
    return result;
}

bool TJsonTagDynamicPool::DeleteJsonTag(TJsonTag* tag)
{
    int selectedIndex = -1;
    for (int i = 0; i<m_tags.Count(); i++)
    {
        if (tag == m_tags[i])
        {
            selectedIndex = i;            
            m_tags.RemoveAt(i);    
            delete tag;

            return true;
        }
    }    
    return false;
}

void TJsonTagDynamicPool::Clear()
{
    for(int i = 0; i<m_tags.Count()-1; i++)
    {
        delete m_tags[i];
    }	
    m_tags.Clear();
}
