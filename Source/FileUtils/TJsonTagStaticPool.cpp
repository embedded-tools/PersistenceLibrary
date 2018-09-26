#include "TJsonTagStaticPool.h"

TJsonTagStaticPool::TJsonTagStaticPool()
{
    m_tagsCount = 0;
}

TJsonTag* TJsonTagStaticPool::GetJsonTag(short i)
{
	if (m_tagsCount>=JSONTAGSTATICPOOLSIZE) return NULL;

    return &m_tags[i];
}

unsigned short TJsonTagStaticPool::GetJsonTagCount()
{
	return m_tagsCount;
}

TJsonTag* TJsonTagStaticPool::CreateJsonTag(TJsonTag* parentTag)
{
	if (m_tagsCount>=JSONTAGSTATICPOOLSIZE)
	{
		return NULL;
	}

	TJsonTag* result = NULL;
	m_tags[m_tagsCount].Clear();	
	result = &m_tags[m_tagsCount++];
	SetJsonTagParent(result, parentTag);
    return result;
}

bool TJsonTagStaticPool::DeleteJsonTag(TJsonTag* tag)
{
    int i;
	int selectedIndex = -1;
	for (i = 0; i<m_tagsCount; i++)
	{
		if (tag == &m_tags[i])
		{
			selectedIndex = i;
		}
	}
	if (selectedIndex==-1)
	{
		return false;
	}
	for(i = selectedIndex; i<m_tagsCount-2; i++)
	{
		m_tags[i]=m_tags[i+1];
	}
	m_tags[m_tagsCount-1].Clear();
	m_tagsCount--;
    return NULL;
}

void TJsonTagStaticPool::Clear()
{
	for(int i = 0; i<m_tagsCount-1; i++)
	{
		m_tags[i].Clear();
	}	
	m_tagsCount = 0;
}
