/*
 * Persistence Library / Persistence / TSerializedItem
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

#ifndef TSERIALIZATIONITEM___H
#define TSERIALIZATIONITEM___H

#include "tlist.h"
#include "tstring.h"
#include "stringutils.h"
#include "TSerializer.h"
#include "EItemState.h"

#define HELPER_BINARY_ARRAY_FLAG 0xF7

class TSerializationManager;
class TSerializedBaseCollection;

class TSerializedItem
{
	private:		

		void WriteXsdInt (const char* paramName, const char* typeName, int value, int minValue, int maxValue);
		void WriteXsdUInt(const char* paramName, const char* typeName, unsigned int value, unsigned int minValue, unsigned int maxValue);
	public:
		virtual unsigned long  GetUID()=0;				
		virtual unsigned short GetSubType()=0;
		virtual EItemState     GetState()=0;

		virtual void		   SetUID(unsigned long uid)=0;
		virtual void           SetSubType(unsigned short subType)=0;
		virtual void           SetState(EItemState state)=0;
		
		virtual void Serialize (unsigned short version)=0;

		void SerializeChar      (const char* paramName, signed char &value,   signed char aMinValue=-128, signed char aMaxValue=127);
		void SerializeByte      (const char* paramName, unsigned char &value, unsigned char aMinValue=0, unsigned char aMaxValue=255);
		void SerializeShortInt  (const char* paramName, short &value, short aMinValue=-32768, short aMaxValue=32767);
		void SerializeShortUInt (const char* paramName, unsigned short &value, unsigned short aMinValue=0, unsigned short aMaxValue=65535);
		void SerializeLongInt   (const char* paramName, long &value, long aMinValue=-2147483647, long aMaxValue=2147483647);
		void SerializeLongUInt  (const char* paramName, unsigned long &value, unsigned long aMinValue=0, unsigned long aMaxValue=4294967295);
		void SerializeUID       (const char* paramName, void** object, bool canBeNull, TSerializedBaseCollection* collection);
		void SerializeUIDArray  (const char* arrayName, void* objectList, int maxArrayLength, TSerializedBaseCollection* collection);
		void SerializeString(const char* paramName, TString& string, bool canBeEmpty, int minLenght=0, int maxLength=65536);

};

#endif
