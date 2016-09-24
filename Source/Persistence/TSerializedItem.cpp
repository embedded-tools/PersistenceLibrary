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

#include "TSerializedItem.h"
#include "TSerializedBaseCollection.h"


void TSerializedItem::WriteXsdInt(const char* paramName, const char* typeName, int value, int minValue, int maxValue)
{
	TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
	TSerializer::DataStream->WritePlainText(paramName);
	TSerializer::DataStream->WriteByte('"');
	TSerializer::DataStream->WritePlainText(" type=\"NS:");
	TSerializer::DataStream->WritePlainText(typeName);

	TSerializer::DataStream->WritePlainText("_MIN");
	char minBuf[16];
	LongIntToStr(minValue, minBuf, 16);
	if (minBuf[0]=='-') minBuf[0]='M';
	TSerializer::DataStream->WritePlainText(minBuf);					

	TSerializer::DataStream->WritePlainText("_MAX");
	char maxBuf[16];	
	LongIntToStr(maxValue, maxBuf, 16);
	if (maxBuf[0]=='-') maxBuf[0]='M';
	TSerializer::DataStream->WritePlainText(maxBuf);					

	TSerializer::DataStream->WritePlainText("\" />\r\n");
}

void TSerializedItem::WriteXsdUInt(const char* paramName, const char* typeName, unsigned int value, unsigned int minValue, unsigned int maxValue)
{
	TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
	TSerializer::DataStream->WritePlainText(paramName);
	TSerializer::DataStream->WriteByte('"');
	TSerializer::DataStream->WritePlainText(" type=\"NS:");
	TSerializer::DataStream->WritePlainText(typeName);

	TSerializer::DataStream->WritePlainText("_MIN");
	char minBuf[16];
	ULongIntToStr(minValue, minBuf, 16);
	TSerializer::DataStream->WritePlainText(minBuf);					

	TSerializer::DataStream->WritePlainText("_MAX");
	char maxBuf[16];
	ULongIntToStr(maxValue, maxBuf, 16);
	TSerializer::DataStream->WritePlainText(maxBuf);					

	TSerializer::DataStream->WritePlainText("\" />\r\n");
}


void TSerializedItem::SerializeChar (const char* paramName, signed char &value, signed char aMinValue, signed char aMaxValue)
{
	signed char defValue = 0;

	switch(TSerializer::Mode)
	{
		case esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteByteAsText(value);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");				
			}
			break;			
		case esmXMLInput: 
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					value = (char)StrToShortInt(textValue.ToPChar());
				}		
			}
			break;
		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteChar(value);
			}			
			break;
		case esmBinaryInput: 
			{
				TSerializer::DataStream->ReadChar(value);
			}
			break;
		case esmXSDOutput: 
			{
				WriteXsdInt(paramName, "BYTE", value, aMinValue, aMaxValue);
			}			
			break;
	}
}

void TSerializedItem::SerializeByte(const char* paramName, unsigned char &value, unsigned char aMinValue, unsigned char aMaxValue)
{
	switch(TSerializer::Mode)
	{
		case esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteUByteAsText(value);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");				
			}
			break;
		case esmXMLInput: 
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					value = (unsigned char)StrToULongInt(textValue.ToPChar());
				}
			}			
			break;

		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteByte(value);
			}			
			break;
		case esmBinaryInput: 
			{
				TSerializer::DataStream->ReadByte(value);
			}
			break;
		case esmXSDOutput: 
			{
				WriteXsdUInt(paramName, "BYTE", value, aMinValue, aMaxValue);
			}			
			break;
	}
}

void TSerializedItem::SerializeShortInt (const char* paramName, short &value, short aMinValue, short aMaxValue)
{
	switch(TSerializer::Mode)
	{
		case esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteWordAsText(value);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");								
			}
			break;
		case esmXMLInput: 
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					value = (short)StrToShortInt(textValue.ToPChar());
				}
			}
			break;
		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteWord(value);
			}			
			break;
		case esmBinaryInput: 
			{
				TSerializer::DataStream->ReadWord(value);
			}
			break;
		case esmXSDOutput: 
			{
				WriteXsdInt(paramName, "WORD", value, aMinValue, aMaxValue);
			}			
			break;
	}
}

void TSerializedItem::SerializeShortUInt (const char* paramName, unsigned short &value, unsigned short aMinValue, unsigned short aMaxValue)
{
	switch(TSerializer::Mode)
	{
		case esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteUWordAsText(value);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");												
			}
			break;
		case esmXMLInput: 
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					value = (unsigned short)StrToUShortInt(textValue.ToPChar());
				}
			}
			break;

		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteUWord(value);
			}			
			break;
		case esmBinaryInput: 
			{
				TSerializer::DataStream->ReadUWord(value);
			}
			break;
		case esmXSDOutput: 
			{
				WriteXsdUInt(paramName, "WORD", value, aMinValue, aMaxValue);
			}			
			break;
	}
}

void TSerializedItem::SerializeLongInt  (const char* paramName, long &value, long aMinValue, long aMaxValue)
{
	switch(TSerializer::Mode)
	{
		case esmXMLOutput:
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteLongAsText(value);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");												
			}
			break;
		case esmXMLInput: 
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					value = StrToLongInt(textValue.ToPChar());
				}
			}
			break;
		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteLong(value);
			}			
			break;
		case esmBinaryInput: 
			{
				TSerializer::DataStream->ReadLong(value);
			}
			break;
		case esmXSDOutput: 
			{
				WriteXsdInt(paramName, "INT", value, aMinValue, aMaxValue);
			}			
			break;
	}
}

void TSerializedItem::SerializeLongUInt (const char* paramName, unsigned long &value, unsigned long aMinValue, unsigned long aMaxValue)
{
	switch(TSerializer::Mode)
	{
		case esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteULongAsText(value);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");																
			}
			break;
		case esmXMLInput:
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					value = StrToULongInt(textValue.ToPChar());
				}
			}
			break;

		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteULong(value);
			}			
			break;
		case esmBinaryInput: 
			{
				TSerializer::DataStream->ReadULong(value);
			}
			break;

		case esmXSDOutput: 
			{
				WriteXsdUInt(paramName, "INT", value, aMinValue, aMaxValue);
			}			
			break;
	}
}

void TSerializedItem::SerializeUID  (const char* paramName, void** object, bool canBeNull, TSerializedBaseCollection* collection)
{
	TSerializedItem** referencedItem = (TSerializedItem**)(object);	
	unsigned long     referencedUID  = 0;
	if (referencedItem[0]!=NULL)
	{
		referencedUID = referencedItem[0]->GetUID();
	}

	switch(TSerializer::Mode)
	{
		case esmXMLOutput:
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteULongAsText(referencedUID);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");																
			}
			break;

		case esmXMLInput: 
			{
				TCustomString<XMLMAXNAMESIZE> keyName = paramName;
				if (TSerializer::XMLCache->Values.ContainsKey(keyName))
				{
					TCustomString<XMLMAXNAMESIZE> valueText = TSerializer::XMLCache->Values[keyName];
					int value = StrToLongInt(valueText.ToPChar());

					referencedItem[0] = collection->FindItem(value);
				} else {
					referencedItem[0] = NULL;
				}				
			}
			break;
		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteULong(referencedUID);
			}
			break;
		case esmBinaryInput:
			{
				TSerializer::DataStream->ReadULong(referencedUID);
				referencedItem[0] = collection->FindItem(referencedUID);
			}			
			break;
		case esmXSDOutput: 
			{
				TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText("\" type=\"NS:OBJECT");
				if (canBeNull)
				{
					TSerializer::DataStream->WritePlainText("_EMP");
				}
				TSerializer::DataStream->WritePlainText("\" />\r\n");
				break;
			}			
			break;
	}
}

void TSerializedItem::SerializeUIDArray (const char* arrayName, void* objectList, int maxArrayLength, TSerializedBaseCollection* collection)
{
	TObjectList<TSerializedItem>* selectedList = (TObjectList<TSerializedItem>*) objectList;

	switch(TSerializer::Mode)
	{
		case esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(arrayName);
				TSerializer::DataStream->WritePlainText(">");

				for(int i = 0; i<selectedList->Count(); i++)
				{
					TSerializedItem* item = (TSerializedItem*) selectedList->Items(i);

					unsigned int uid = item->GetUID();
					TSerializer::DataStream->WritePlainText("<UID>");

					char uidBuf[16];
					ULongIntToStr(uid, uidBuf, 16);
					TSerializer::DataStream->WritePlainText(uidBuf);
					TSerializer::DataStream->WritePlainText("</UID>\r\n");
				}
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(arrayName);
				TSerializer::DataStream->WritePlainText(">\r\n");																
			}
			break;

		case esmXMLInput: 
			{
				static TCustomString<XMLMAXVALUESIZE> textUIDs;				
				static TCustomString<12>textUID;

				unsigned int UID;
				textUIDs = TSerializer::XMLCache->Values[arrayName];

				int n = textUIDs.IndexOf(';');
				while(n>0)
				{
					textUID.CopyFrom(textUIDs.ToPChar(), n);
					UID = StrToULongInt(textUID);
					TSerializedItem* item = collection->FindItem(UID);
					if (item!=NULL) 
					{
						selectedList->Add(item);
					}
					char* dest = (char*)textUIDs.ToPChar();
					char* src  = dest + n + 1;
					textUIDs.CopyFrom(src);
					n=textUIDs.IndexOf(';');
				}	
				UID = StrToULongInt(textUIDs);
				TSerializedItem* item = collection->FindItem(UID);
				if (item!=NULL) 
				{
					selectedList->Add(item);
				}
			}
			break;

		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteByte(HELPER_BINARY_ARRAY_FLAG);
				TSerializer::DataStream->WriteUWord(selectedList->Count());
				for(unsigned short i = 0; i<selectedList->Count(); i++)
				{
					TSerializedItem* item = (*selectedList)[i];			
					TSerializer::DataStream->WriteULong(item->GetUID());
				}
			}			
			break;

		case esmBinaryInput: 
			{
				unsigned char flag = 0;
				TSerializer::DataStream->ReadByte(flag);
				if (flag!=HELPER_BINARY_ARRAY_FLAG) return;

				selectedList->Clear();
				unsigned short len = 0;
				unsigned long  uid = 0;
				TSerializer::DataStream->ReadUWord(len);

				for(unsigned short i = 0; i<len; i++)
				{
					TSerializer::DataStream->ReadULong(uid);
					TSerializedItem* item = collection->FindItem(uid);
					if (item!=NULL)
					{
						selectedList->Add(item);
					}					
				}
			}
			break;
		case esmXSDOutput: 
			{
				TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
				TSerializer::DataStream->WritePlainText(arrayName);
				TSerializer::DataStream->WritePlainText("\" >\r\n" );
				TSerializer::DataStream->WritePlainText("  <xsd:complexType>\r\n");
				TSerializer::DataStream->WritePlainText("    <xsd:sequence>\r\n");
				TSerializer::DataStream->WritePlainText("      <xsd:element name=\"UID\" type=\"NS:OBJECT\"");
				if (maxArrayLength>0)
				{
					TSerializer::DataStream->WritePlainText(" maxOccurs=\"");
					TSerializer::DataStream->WriteLongAsText(maxArrayLength);
					TSerializer::DataStream->WritePlainText("\" ");
				}					
				TSerializer::DataStream->WritePlainText(" />\r\n");
				TSerializer::DataStream->WritePlainText("    </xsd:sequence>\r\n");
				TSerializer::DataStream->WritePlainText("  </xsd:complexType>\r\n");
				TSerializer::DataStream->WritePlainText("</xsd:element>\r\n");
			}			
			break;
	}
}

void TSerializedItem::SerializeString(const char* paramName, TString& string, bool canBeEmpty, int minLength, int maxLength)
{
	switch(TSerializer::Mode)
	{
		case  esmXMLOutput: 
			{
				TSerializer::DataStream->WritePlainText("<");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">");
				TSerializer::DataStream->WriteXMLEncodedText(string);
				TSerializer::DataStream->WritePlainText("</");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText(">\r\n");
			}
			break;
		case esmXMLInput: 
			{
				TCustomString<XMLMAXVALUESIZE> &textValue = TSerializer::XMLCache->Values[paramName];
				if (textValue.Length()>0)
				{
					string = textValue;
				}		
			}
			break;

		case esmBinaryOutput: 
			{
				TSerializer::DataStream->WriteBinaryText(string);				
			}
			break;

		case esmBinaryInput: 
			{				
				TSerializer::DataStream->ReadBinaryText(string, maxLength);
			}
			break;

		case esmXSDOutput: 
			{
				TSerializer::DataStream->WritePlainText("<xsd:element name=\"");
				TSerializer::DataStream->WritePlainText(paramName);
				TSerializer::DataStream->WritePlainText("\" type=\"NS:STRING");
				if (canBeEmpty)
				{
					TSerializer::DataStream->WritePlainText("_EMP");
				}
				if (minLength>0)
				{
					TSerializer::DataStream->WritePlainText("_MIN");
					char buf[10];
					LongIntToStr(minLength, buf, 10);
					TSerializer::DataStream->WritePlainText(buf);					
				}
				if (maxLength>0)
				{
					TSerializer::DataStream->WritePlainText("_MAX");
					char buf[10];
					LongIntToStr(maxLength, buf, 10);
					TSerializer::DataStream->WritePlainText(buf);					
				}
				TSerializer::DataStream->WritePlainText("\" />\r\n");
				break;
			}
	}
}


