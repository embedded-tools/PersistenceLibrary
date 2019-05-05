/*
 * Persistence Library / Math / TFixedPoint1024
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

#include "TSerializer.h"

TStream*			TSerializer::DataStream = NULL;
TXMLCache*			TSerializer::XMLCache = NULL;
TXMLParser*			TSerializer::XMLParser = NULL;

eSerializationMode	TSerializer::Mode;  
bool				TSerializer::IncrementalChanges = false;
unsigned short      TSerializer::TimeStamp = 0;
unsigned char       TSerializer::ErrorCode = 0;

void TSerializer::Init()
{
	if (DataStream!=NULL) 
	{
		delete DataStream;
		DataStream = NULL;
	}

	Mode = esmXSDOutput;
	IncrementalChanges = false;
	TimeStamp = 0;
	ErrorCode = 0;
}

