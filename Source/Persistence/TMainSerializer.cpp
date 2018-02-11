/*
 * Persistence Library / Persistence / TMainSerializer
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

#include "TMainSerializer.h"

TMainSerializer::TMainSerializer()
{
	xmlSerializationComplete = false;	
}

void TMainSerializer::OnStartElement ( TXMLParserInterface* Parser, const char *pszName, TXMLParamMap &attributes)
{
	for(int i = 0; i<registeredManagers.Count(); i++)
	{
		if (registeredManagers[i]->ManagerName==pszName)
		{
			registeredManagers[i]->LastXMLTalker = this;
			Parser->SetTalker(registeredManagers[i]);
		}
	}
}

void TMainSerializer::OnEndElement   ( TXMLParserInterface* Parser, const char *pszName)
{
	xmlSerializationComplete = true;
}

void TMainSerializer::OnCharacterData( TXMLParserInterface* Parser, const char *pszData)
{
	//nothing to do
}


bool TMainSerializer::LoadAllDataFromXML(TStream* stream)
{	
	Clear();
	TXMLParser xmlParser;
	TXMLCache  xmlCache;

	TSerializer::Init();
	TSerializer::Mode = esmXMLInput;
	TSerializer::DataStream = stream;
	TSerializer::XMLParser = &xmlParser;
	TSerializer::XMLCache  = &xmlCache;		
	xmlParser.SetTalker(this);

	xmlSerializationComplete = false;
	static unsigned char speedUpBuffer[XML_HELPER_BUFFER_SIZE];
	while(!stream->Eof())
	{
		long dataLength = stream->ReadBuffer((void*)&speedUpBuffer, XML_HELPER_BUFFER_SIZE);
		xmlParser.Parse((const char*) speedUpBuffer, dataLength);
	}	
	xmlParser.FlushCache();
	return xmlSerializationComplete;
}

bool TMainSerializer::LoadDataUpdateFromXML(TStream* stream)
{	
	TSerializer::Init();
	TSerializer::DataStream = stream;
	TSerializer::Mode = esmXMLInput;
	TSerializer::IncrementalChanges = true;
		
	Serialize(GetVersion());
	return false;
}


bool TMainSerializer::LoadAllDataFromBinary (TStream* stream)
{
	unsigned char magicByte;
	unsigned short version;

	Clear();
	TSerializer::Init();
	TSerializer::Mode = esmBinaryInput;
	TSerializer::IncrementalChanges = false;
	TSerializer::DataStream = stream;	
	TSerializer::DataStream->ReadByte(magicByte);
	if (magicByte!=SERIALIZER_BEGIN_FLAG) 
	{
		TSerializer::ErrorCode = eecIncorrectBinaryHeader;
		return false;
	}

	TSerializer::DataStream->ReadUWord(version);
	Serialize(version);

	TSerializer::DataStream->ReadByte(magicByte);
	if (magicByte!=SERIALIZER_END_FLAG) 
	{
		TSerializer::ErrorCode = eecIncorrectBinarySerialization;	
		return false;
	}

	return true;
}

bool TMainSerializer::LoadDataUpdateFromBinary (TStream* stream)
{
	unsigned char magicByte;
	unsigned short version;

	TSerializer::Init();
	TSerializer::Mode = esmBinaryInput;
	TSerializer::IncrementalChanges = true;
	TSerializer::DataStream = stream;
	TSerializer::DataStream->ReadByte(magicByte);
	if (magicByte!=SERIALIZER_BEGIN_FLAG) 
	{
		TSerializer::ErrorCode = eecIncorrectBinaryHeader;
		return false;
	}

	TSerializer::DataStream->ReadUWord(version);
	Serialize(version);
	
	TSerializer::DataStream->ReadByte(magicByte);
	if (magicByte!=SERIALIZER_END_FLAG) 
	{
		TSerializer::ErrorCode = eecIncorrectBinarySerialization;
		return false;
	}

	return false;
}


bool TMainSerializer::SaveAllDataToXML(TStream* stream, const char* rootName)
{
	TSerializer::Init();
	TSerializer::Mode = esmXMLOutput;
	TSerializer::DataStream = stream;
	TSerializer::DataStream->WritePlainText("<");
	TSerializer::DataStream->WritePlainText(rootName);
	TSerializer::DataStream->WritePlainText(">\r\n");
	Serialize(GetVersion());
	TSerializer::DataStream->WritePlainText("</");
	TSerializer::DataStream->WritePlainText(rootName);
	TSerializer::DataStream->WritePlainText(">\r\n");					
	return false;
}

bool TMainSerializer::SaveDataUpdateToXML(TStream* stream, const char* rootName)
{
	TSerializer::Init();
	TSerializer::Mode = esmXMLOutput;
	TSerializer::IncrementalChanges = true;
	TSerializer::DataStream = stream;
	TSerializer::DataStream->WritePlainText("<");
	TSerializer::DataStream->WritePlainText(rootName);
	TSerializer::DataStream->WritePlainText(">\r\n");
	Serialize(GetVersion());
	TSerializer::DataStream->WritePlainText("</");
	TSerializer::DataStream->WritePlainText(rootName);
	TSerializer::DataStream->WritePlainText(">\r\n");					
	return false;
}


bool TMainSerializer::SaveAllDataToBinary (TStream* stream)
{
	unsigned short version = GetVersion();
	TSerializer::Init();
	TSerializer::Mode = esmBinaryOutput;
	TSerializer::DataStream = stream;
	TSerializer::DataStream->WriteByte(SERIALIZER_BEGIN_FLAG);
	TSerializer::DataStream->WriteUWord(version);
	Serialize(version);
	TSerializer::DataStream->WriteByte(SERIALIZER_END_FLAG);
	return false;
}

bool TMainSerializer::SaveDataUpdateToBinary (TStream* stream)
{
	unsigned short version = GetVersion();
	TSerializer::Init();
	TSerializer::Mode = esmBinaryOutput;
	TSerializer::IncrementalChanges = true;
	TSerializer::DataStream = stream;
	TSerializer::DataStream->WriteByte(SERIALIZER_BEGIN_FLAG);
	TSerializer::DataStream->WriteUWord(version);
	Serialize(version);
	TSerializer::DataStream->WriteByte(SERIALIZER_END_FLAG);
	return false;
}


bool TMainSerializer::SaveToXSD(TStream* stream, const char* rootName)
{
	TSerializer::Init();
	TSerializer::Mode = esmXSDOutput;
	TSerializer::DataStream = stream;
	TSerializer::DataStream->WritePlainText("<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"urn:user-schema\" xmlns:NS=\"urn:user-schema\" elementFormDefault=\"qualified\" targetNamespace=\"urn:user-schema\">\r\n");		
	TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
	TSerializer::DataStream->WritePlainText(rootName);
	TSerializer::DataStream->WritePlainText("\"");
	TSerializer::DataStream->WritePlainText(">\r\n");
	TSerializer::DataStream->WritePlainText("<xsd:complexType>\r\n");
	TSerializer::DataStream->WritePlainText("<xsd:sequence>\r\n");
	Serialize(GetVersion());
	TSerializer::DataStream->WritePlainText("</xsd:sequence>\r\n");
	TSerializer::DataStream->WritePlainText("</xsd:complexType>\r\n");
	TSerializer::DataStream->WritePlainText("</xsd:element>\r\n");
	TSerializer::DataStream->WritePlainText("</xsd:schema>\r\n");		
	return false;
}

