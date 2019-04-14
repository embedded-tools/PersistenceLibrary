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


#ifndef TSERIALIZATION___H
#define TSERIALIZATION___H

#include "tstream.h"
#include "TSerializer.h"
#include "TSerializedBaseCollection.h"
#include "TXMLTalkerInterface.h"
#include "ESerializationMode.h"

#define XML_HELPER_BUFFER_SIZE 32
#define SERIALIZER_BEGIN_FLAG 0xF8
#define SERIALIZER_END_FLAG 0xF9


class TMainSerializer : TXMLTalkerInterface
{
	private:

		bool  xmlSerializationComplete;
	protected:

		TList<TSerializedBaseCollection*> registeredManagers;
		
		virtual bool Serialize(unsigned short version)=0;
		virtual void Clear()=0;
		virtual unsigned short GetVersion()=0;

		virtual void OnStartElement ( TXMLParserInterface* Parser, const char *pszName, TXMLParamMap &attributes);
		virtual void OnEndElement   ( TXMLParserInterface* Parser, const char *pszName);
		virtual void OnCharacterData( TXMLParserInterface* Parser, const char *pszData);
		
	public:		

		TMainSerializer();

		bool LoadAllDataFromXML(TStream* xmlStream);
		bool LoadDataUpdateFromXML(TStream* xmlStream);
		bool LoadAllDataFromBinary (TStream* binaryStream);
		bool LoadDataUpdateFromBinary (TStream* binaryStream);

		bool SaveAllDataToXML(TStream* stream, const char* rootName);
		bool SaveDataUpdateToXML(TStream* stream, const char* rootName);
		bool SaveAllDataToBinary (TStream* stream);
		bool SaveDataUpdateToBinary (TStream* stream);

		bool SaveToXSD(TStream* stream, const char* rootName);

};

#endif


