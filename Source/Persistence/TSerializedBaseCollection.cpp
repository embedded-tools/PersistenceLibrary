/*
 * Persistence Library / Math / TFixedPoint1024
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

#include "TSerializedBaseCollection.h"
#include "TSerializedItem.h"
#include "TSerializer.h"

TCustomString<XMLMAXNAMESIZE> TSerializedBaseCollection::lastName;
	
TSerializedBaseCollection::TSerializedBaseCollection()
{
	ManagerName = "ManagerNameNotSet";
	ElementName = "ElementNameNotSet";
}

unsigned long TSerializedBaseCollection::GetUID()	
{
	//collection has no UID
	return 0;
}

unsigned short TSerializedBaseCollection::GetSubType()
{
	//collection has no Type
	return 0;
}

unsigned short TSerializedBaseCollection::GetVersion()
{
	//collection has no version
	return 0;
}

EItemState TSerializedBaseCollection::GetState()
{
	//collection has no state
	return eisUnchanged;
}

void TSerializedBaseCollection::SetUID(unsigned long uid)
{
	//collection has no UID
}

void TSerializedBaseCollection::SetSubType(unsigned short subType)
{
	//collection has no subType
}

void TSerializedBaseCollection::SetState(EItemState state)
{
	//nothing to do
}


void TSerializedBaseCollection::WriteXsdManagerBegin(TCustomString<XMLMAXNAMESIZE> &name, bool minOccurs)
{
	TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
	TSerializer::DataStream->WritePlainText(name);
	TSerializer::DataStream->WritePlainText("\"");
	if (minOccurs)
	{
		TSerializer::DataStream->WritePlainText(" minOccurs=\"1\" maxOccurs=\"1\"");
	}
	TSerializer::DataStream->WritePlainText(">\r\n");
	TSerializer::DataStream->WritePlainText("<xsd:complexType>\r\n");
	TSerializer::DataStream->WritePlainText("<xsd:sequence>\r\n");
}

void TSerializedBaseCollection::WriteXsdManagerEnd(bool addUID, bool addVersion)
{
	TSerializer::DataStream->WritePlainText("</xsd:sequence>\r\n");
	if (addUID)
	{
		TSerializer::DataStream->WritePlainText("<xsd:attribute name=\"UID\" type=\"NS:OBJECT\" use=\"required\" />\r\n");
		TSerializer::DataStream->WritePlainText("<xsd:attribute name=\"Type\" type=\"NS:TYPE\" use=\"required\" />\r\n");
	}
	if (addVersion)
	{
		TSerializer::DataStream->WritePlainText("<xsd:attribute name=\"Version\" type=\"NS:VERSION\" use=\"required\" />\r\n");
	}
	TSerializer::DataStream->WritePlainText("</xsd:complexType>\r\n");
	TSerializer::DataStream->WritePlainText("</xsd:element>\r\n");
}

void TSerializedBaseCollection::SerializeHeader(unsigned short &version)
{
	switch (TSerializer::Mode)
	{
		case esmXMLOutput:
			{	
				//header serialization
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(ManagerName);
				TSerializer::DataStream->WritePlainText(" Version=\"");
				char text[16];
				ULongIntToStr(version, text, 16);
				TSerializer::DataStream->WritePlainText(text);				
				TSerializer::DataStream->WritePlainText("\">\r\n");	
			}
			break;

		case esmXMLInput:
			{
				//xml input is handled by XMLParser
			}
			break;

		case esmBinaryOutput:  
			{
				version = GetVersion();
				TSerializer::DataStream->WriteChar((char)MANAGER_BEGIN_FLAG);
				TSerializer::DataStream->WriteUWord(version);
			}
			break;

		case esmBinaryInput:  
			{
				unsigned char c;
				TSerializer::DataStream->ReadByte(c);
				if (c!= MANAGER_BEGIN_FLAG)
				{
					TSerializer::ErrorCode = eecIncorrectBinarySerialization;
					return;
				}
				TSerializer::DataStream->ReadUWord(version);
			}
			break;

		case esmXSDOutput: 
			{
				WriteXsdManagerBegin(ManagerName, true);
			}
			break;
	}
}

void TSerializedBaseCollection::SerializeBody(unsigned short version)
{
	switch (TSerializer::Mode)
	{
		case esmXMLOutput:
			{						
				//gets first item
				TSerializedItem* item = (TSerializedItem*)First();
				version = GetVersion();
				while(item!=NULL)
				{
					char text[16];

					TSerializer::DataStream->WritePlainText("<");
					TSerializer::DataStream->WritePlainText(ElementName);
					TSerializer::DataStream->WritePlainText(" UID=\"");					
					LongIntToStr(item->GetUID(), text, 16);
					TSerializer::DataStream->WritePlainText(text);
					TSerializer::DataStream->WriteChar('"');
					unsigned short subType = item->GetSubType();
					if (subType>0)
					{						
						TSerializer::DataStream->WritePlainText(" Type=\"");
						ShortIntToStr(subType, text, 16);
						TSerializer::DataStream->WritePlainText(text);
						TSerializer::DataStream->WriteChar('"');
					}
					TSerializer::DataStream->WritePlainText(">\r\n");				
					item->Serialize(version);
					TSerializer::DataStream->WritePlainText("</");
					TSerializer::DataStream->WritePlainText(ElementName);
					TSerializer::DataStream->WritePlainText(">\r\n");				

					//gets next item until all items are serialized
					item = (TSerializedItem*) Next();
				}
			}
			break;

		case esmXMLInput:
			{
				//xml input is handled by XMLParser
			}
			break;

		case esmBinaryInput:    //reading configuration data from binary stream
			{
				unsigned char c = 0;
				unsigned long UID = 0;
				unsigned short SubType = 0;

				TSerializer::DataStream->ReadByte(c);
				while(c==ELEMENT_FLAG)
				{
					TSerializer::DataStream->ReadULong(UID);
					TSerializer::DataStream->ReadUWord(SubType);
					TSerializedItem* item = FindItem(UID);
					if (item==NULL)
					{
						item = CreateItem(UID, SubType);
					}					
					item->Serialize(version);
					AddItem(item);

					TSerializer::DataStream->ReadByte(c);
				}
				if (c!=MANAGER_END_FLAG) 
				{
					TSerializer::ErrorCode = eecIncorrectBinarySerialization;
					return;				
				}
			}
			break;

		case esmBinaryOutput:
			{
				TSerializedItem* item = (TSerializedItem*)First();

				version = GetVersion();
				while(item!=NULL)
				{
					unsigned short subType = item->GetSubType();
					TSerializer::DataStream->WriteChar((char)ELEMENT_FLAG);
					TSerializer::DataStream->WriteULong(item->GetUID());
					TSerializer::DataStream->WriteUWord(subType);
					if (subType!=255)
					{
						
						item->Serialize(version);
					}
					item = (TSerializedItem*) Next();
				}
				TSerializer::DataStream->WriteChar((char)MANAGER_END_FLAG);
			}
			break;

		case esmXSDOutput:
			{
				WriteXsdManagerBegin(ElementName, false);
				TSerializedItem* item = (TSerializedItem*) First();
				if (item==NULL)
				{
					TSerializedItem* item = CreateItem(0,NULL);
					item->Serialize(GetVersion());
					delete item;
				} else {
					item->Serialize(GetVersion());
				}
				WriteXsdManagerEnd(true, false);
			}
			break;
	}

}

void TSerializedBaseCollection::SerializeFooter(unsigned short version)
{
	switch (TSerializer::Mode)
	{
		case esmXMLOutput:		//writing configuration data to xml stream
			{
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(ManagerName);
				TSerializer::DataStream->WritePlainText(">\r\n");				
			}
			break;

		case esmXMLInput:
			{
				//xml input is handled by XMLParser
			}
			break;

		case esmBinaryOutput:
		case esmBinaryInput:
			{
				//no footer required
			}
			break;

		case esmXSDOutput:   
			{
				WriteXsdManagerEnd(false, true);
			}
			break;

	}
}


void TSerializedBaseCollection::Serialize(unsigned short version)
{
	if (TSerializer::ErrorCode!=eecNone)
	{
		return;
	}
	SerializeHeader(version);
	SerializeBody(version);
	SerializeFooter(version);
}

void TSerializedBaseCollection::OnStartElement ( TXMLParserInterface* Parser, const char *pszName, TXMLParamMap &attributes)
{
	if (ManagerName==pszName)
	{
		TSerializer::XMLCache->Clear();
		TSerializer::XMLCache->RedirectTalker(Parser);
		return;
	}
	if (ElementName==pszName)
	{
		Serialize(GetVersion());
		newItemUID  = attributes["UID"];
		newItemType = attributes["Type"];
		TSerializer::XMLCache->Clear();
		TSerializer::XMLCache->RedirectTalker(Parser);
		if (newItemUID.Length()>0)  TSerializer::XMLCache->Attributes.Add("UID",  newItemUID);
		if (newItemType.Length()>0) TSerializer::XMLCache->Attributes.Add("Type", newItemType);
		
		return;
	}
	for (int i = 0; i<subManagers.Count(); i++)
	{
		if (subManagers[i]->ManagerName==pszName)
		{
			TSerializedBaseCollection* subManager = subManagers[i];
			subManager->LastXMLTalker = this;
			Parser->SetTalker(subManager);				
			return;
		}
	}
	lastName = pszName;
}

void TSerializedBaseCollection::OnEndElement   ( TXMLParserInterface* Parser, const char *pszName)
{
	if (ManagerName==pszName)
	{
		Parser->SetTalker(LastXMLTalker);
	} else 
	if (ElementName==pszName)
	{
		LoadItem();
	}
}

void TSerializedBaseCollection::OnCharacterData( TXMLParserInterface* Parser, const char *pszData)
{
	static TCustomString<XMLMAXNAMESIZE>	lastValue = pszData;
	TSerializer::XMLCache->Values.Add(lastName, lastValue);
}

bool TSerializedBaseCollection::LoadItem()
{
	TCustomString<XMLMAXVALUESIZE> uidAttribute  = TSerializer::XMLCache->Attributes["UID"];
	TCustomString<XMLMAXVALUESIZE> typeAttribute = TSerializer::XMLCache->Attributes["Type"];
	unsigned int   UID = StrToULongInt(uidAttribute);
	unsigned short SubType = StrToUShortInt(typeAttribute);

	if (SubType==255) //Signalizes that item needs to be deleted
	{
		TSerializedItem* item = FindItem(UID);
		if (item!=NULL) RemoveItem(item);
		return true;
	}

	TSerializedItem* item = FindItem(UID);
	if (item!=NULL)
	{
		item->Serialize(GetVersion());
		return true;
	} else 
	{
		item = (TSerializedItem*) CreateItem(UID, SubType);
		if (item!=NULL)
		{
			item->Serialize(GetVersion());
			return AddItem(item);
		}
	}
	return false;
}

TSerializedItem* TSerializedBaseCollection::FindItem(unsigned int uid)
{
	TSerializedItem* item = (TSerializedItem*) First();
	while(item!=NULL)
	{
		if (item->GetUID()==uid)
		{
			return item;
		}
		item = (TSerializedItem*) Next();
	}
	return NULL;
}
