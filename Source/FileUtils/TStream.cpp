/*
 * Persistence Library / FileUtils / TStream
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

#include "TStream.h"
#include <stdlib.h> 
#include <stdio.h>
#include "TString.h"
#include "TCustomString.h"
#include "StringUtils.h"

TStream::TStream()
{
    m_parentStream = NULL;
	m_canRead  = false;
	m_canWrite = false;
	m_canSeek  = false;
}

void TStream::Close()
{
    if (m_parentStream!=NULL)
    {
        m_parentStream->Close();
    }
}

long TStream::ReadBuffer   (void* Buffer, long Count)
{
    if (m_parentStream!=NULL)
    {
        return m_parentStream->ReadBuffer(Buffer, Count);
    }
    return 0;
}

long TStream::WriteBuffer (const void* Buffer, long Count)
{
    if (m_parentStream!=NULL)
    {
        return m_parentStream->WriteBuffer(Buffer, Count);
    }    
    return 0;
}

long TStream::Seek (long Offset, ESeekOrigin Origin)
{
    if (m_parentStream!=NULL)
    {
        return m_parentStream->Seek(Offset, Origin);
    }   
    return -1;
}

bool TStream::Eof()
{
	int position = GetPosition();
	int size     = GetSize();
	return position>=size;
}

bool TStream::ReadBinaryText(char* buffer, unsigned short bufferSize)
{
    unsigned short length = 0;
    bool res = ReadUWord(length);
    if (!res) return false;
    if (length<bufferSize)
    {
        int res = ReadBuffer((void*)buffer, length);
        buffer += length;
        *buffer = 0;
        return true;
    }
    return false;    
}



bool TStream::ReadBinaryText(TString* text, short maxLength)
{
	if (text->IsBufferStatic())
	{
		if (text->GetBufferSize()>=maxLength) return false;
	}

    unsigned short length = 0;
    bool res = ReadUWord(length);
    if (!res) 
	{
		text->Clear();
		return false;
	}

	text->Fill(' ', length);
	void* buffer = (void*) text->ToPChar();

	long bytesRead = ReadBuffer(buffer, length);
    if (bytesRead!=length)
    {
		text->Clear();
		return false;
	}
    return true;
}


bool TStream::ReadChar (signed char  &c)
{
    c = 0;
    long bytes = ReadBuffer(&c, sizeof(char));
    return bytes==sizeof(char);
}

bool TStream::ReadByte(unsigned char  &c)
{
    c = 0;
    long bytes = ReadBuffer(&c, sizeof(unsigned char));
    return bytes==sizeof(unsigned char);
}

bool TStream::ReadWord (short &w)
{
    w = 0;
    long bytes = ReadBuffer(&w, sizeof(short));
    return bytes==sizeof(short);
}

bool TStream::ReadUWord(unsigned short &w)
{
    w = 0;
    long bytes = ReadBuffer(&w, sizeof(unsigned short));
    return bytes==sizeof(unsigned short);
}

bool TStream::ReadInt (int &i)
{
    i = 0;
    long bytes = ReadBuffer(&i, sizeof(int));
    return bytes==sizeof(long);    
}

bool TStream::ReadUInt (unsigned int &i)
{
    i = 0;
    long bytes = ReadBuffer(&i, sizeof(long));
    return bytes==sizeof(long);    
}


bool TStream::ReadLong  (long &i)
{
    i = 0;
    long bytes = ReadBuffer(&i, sizeof(long));
    return bytes==sizeof(long);
}

bool TStream::ReadULong (unsigned long &i)
{
    i = 0;
    long bytes = ReadBuffer(&i, sizeof(unsigned int));
    return bytes==sizeof(unsigned int);
}

bool TStream::ReadLine(TString& line)
{
    unsigned char c;
    bool res = true;

    line.Clear();
    while(res)
    {
        res = ReadByte(c);
        if (res==false)
        {
            return false;            
        }
        if (c==13)
        {
            res = ReadByte(c);
            return res;            
        }
        line += c;
    }             
    return true;
}

bool TStream::WriteBinaryText(TString* text)
{
    unsigned short length = text->Length();
    WriteUWord(length);
    int res = WriteBuffer((void*)text->ToPChar(), length);
    return res==length;
}

bool TStream::WriteBinaryText(const char* text)
{
    unsigned short length = StrLen(text);
    WriteUWord(length);
    int res = WriteBuffer((void*)text, length);
    return res==length;
}

bool TStream::WritePlainText(TString* text)
{
	unsigned short length = text->Length();
	int res = WriteBuffer((void*)text->ToPChar(), length);
	return res==length;
}

bool TStream::WritePlainText(const char* text)
{
	unsigned short length = StrLen(text);
	int res = WriteBuffer((void*)text, length);
	return res==length;
}

bool TStream::WriteXMLEncodedText(TString* text)
{
	if (text->Length()>0)
	{
		for(char* pp = (char*)text->ToPChar(); *pp!=0; pp++)
		{
			switch(*pp)
			{
				case ' ': WritePlainText("&nbsp;");
				case '"': WritePlainText("&quot;");
				case '<': WritePlainText("&lt;");
				case '>': WritePlainText("&gt;");
				case '&': WritePlainText("&amp;");
				default: WriteByte(*pp);
			}
		}
	}
	return true;
}

bool TStream::WriteXMLEncodedText(const char* text)
{
	for(char* pp = (char*)text; *pp!=0; pp++)
	{
		switch(*pp)
		{
			case ' ': WritePlainText("&nbsp;"); break;
			case '"': WritePlainText("&quot;"); break;
			case '<': WritePlainText("&lt;"); break;
			case '>': WritePlainText("&gt;"); break;
			case '&': WritePlainText("&amp;"); break;
			default: WriteByte(*pp); break;
		}
	}
	return true;
}


bool TStream::WriteChar (signed char  c)
{
    long bytes = WriteBuffer(&c, sizeof(char));
    return bytes==sizeof(char);
}

bool TStream::WriteByte(unsigned char  c)
{
    long bytes = WriteBuffer(&c, sizeof(unsigned char));
    return bytes==sizeof(unsigned char);
}

bool TStream::WriteWord (short w)
{
    long bytes = WriteBuffer(&w, sizeof(short));
    return bytes==sizeof(short);
}

bool TStream::WriteUWord(unsigned short w)
{
    long bytes = WriteBuffer(&w, sizeof(unsigned short));
    return bytes==sizeof(unsigned short);
}

bool TStream::WriteInt(int i)
{
    long bytes = WriteBuffer(&i, sizeof(int));
    return bytes==sizeof(int);
}

bool TStream::WriteUInt (unsigned int i)
{
    long bytes = WriteBuffer(&i, sizeof(unsigned int));
    return bytes==sizeof(unsigned int);
}

bool TStream::WriteLong (long i)
{
    long bytes = WriteBuffer(&i, sizeof(long));
    return bytes==sizeof(long);
}

bool TStream::WriteULong (unsigned long i)
{
    long bytes = WriteBuffer(&i, sizeof(unsigned long));
    return bytes==sizeof(unsigned long);
}

bool TStream::WriteByteAsText (char  c)
{
	char text[20];
    ShortIntToStr(c, text, 20);
	WritePlainText(text);
	return true;
}

bool TStream::WriteUByteAsText(unsigned char  c)
{
	TCustomString<14> text;
	LongIntToStr(c, (char*)text.ToPChar(), 14);
	WritePlainText(text);
	return true;
}

bool TStream::WriteWordAsText (short w)
{
	TCustomString<14> text;
    ShortIntToStr(w, (char*)text.ToPChar(), 14);
	WritePlainText(text);
	return true;
}

bool TStream::WriteUWordAsText(unsigned short w)
{
    TCustomString<14> text;
    UShortIntToStr(w, (char*)text.ToPChar(), 14);
	WritePlainText(text);
	return true;
}

bool TStream::WriteLongAsText  (long i)
{
    TCustomString<14> text;
    LongIntToStr(i, (char*)text.ToPChar(), 14);
	WritePlainText(text);
	return true;
}

bool TStream::WriteULongAsText (unsigned long i)
{
    TCustomString<14> text;
    ULongIntToStr(i, (char*)text.ToPChar(), 14);
	WritePlainText(text);
	return true;
}

bool TStream::WriteLine(const char* line)
{
    unsigned short length = StrLen(line);
    int res = WriteBuffer((void*)line, length);
    res += WriteByte(13);
    res += WriteByte(10);    
    return (res==(length+2));
}
