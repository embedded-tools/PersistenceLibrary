#include "TXmlTagDynamicPool.h"


TXMLTagDynamicPool::TXMLTagDynamicPool()
{

}

TXMLTag* TXMLTagDynamicPool::GetXMLTag(short i)
{
	return Tags[i];
}

unsigned short TXMLTagDynamicPool::GetXMLTagCount()
{
	return Tags.Count();
}

TXMLTag* TXMLTagDynamicPool::CreateXMLTag(TXMLTag* parentTag)
{
	TXMLTag* tag = new TXMLTag();
	SetXMLTagParent(tag, parentTag);
	Tags.Add(tag);
	return tag;

}

bool TXMLTagDynamicPool::DeleteXMLTag(TXMLTag* tag)
{
	if (Tags.Contains(tag))
	{
		Tags.Remove(tag);
		return true;
	}
	return false;
}

void TXMLTagDynamicPool::Clear()
{
	Tags.Clear();
}
