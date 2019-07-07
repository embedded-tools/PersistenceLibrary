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

TStream::~TStream()
{
  //nothing to do
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
        if (maxLength>(text->GetBufferSize()-1))
        {
            return false;
        }
    }

    unsigned short length = 0;
    bool res = ReadUWord(length);
    if (!res) 
    {
        text->Clear();
        return false;
    }

    if (!text->Fill(' ', length))
    {
        //there is not enough memory for string
        return false;
    }

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
    long bytes = ReadBuffer(&c, 1);
    return bytes==1;
}

bool TStream::ReadByte(unsigned char  &c)
{
    c = 0;
    long bytes = ReadBuffer(&c, 1);
    return bytes==1;
}

bool TStream::ReadWord (short &w)
{
    w = 0;
    if (sizeof(short)>=2)
    {
        long bytes = ReadBuffer(&w, 2);
        return bytes==2;
    }
    return false;
}

bool TStream::ReadUWord(unsigned short &w)
{
    w = 0;
    if (sizeof(short)>=2)
    {
        long bytes = ReadBuffer(&w, 2);
        return bytes==2;
    }
    return false;
}

bool TStream::ReadInt (int &i)
{
    i = 0;
    if (sizeof(int)>=4)
    {
        long bytes = ReadBuffer(&i, 4);
        return bytes==4;
    }
    return false;
}

bool TStream::ReadUInt (unsigned int &i)
{
    i = 0;
    if (sizeof(int)>=4)
    {
        long bytes = ReadBuffer(&i, 4);
        return bytes==4;
    }
    return false;
}

bool TStream::ReadLong (long long &i)
{
    i = 0;
    if (sizeof(long long)>=8)
    {
        long bytes = ReadBuffer(&i, 8);
        return bytes==8;
    }
    return false;
}

bool TStream::ReadULong (unsigned long long &i)
{
    i = 0;
    if (sizeof(unsigned long long)>=8)
    {
        long bytes = ReadBuffer(&i, 8);
        return bytes==8;
    }
    return false;
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
                case ' ': WritePlainText("&nbsp;"); break;
                case '"': WritePlainText("&quot;"); break;
                case '<': WritePlainText("&lt;"); break;
                case '>': WritePlainText("&gt;"); break;
                case '&': WritePlainText("&amp;"); break;
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
    long bytes = WriteBuffer(&c, 1);
    return bytes==1;
}

bool TStream::WriteByte(unsigned char  c)
{
    long bytes = WriteBuffer(&c, 1);
    return bytes==1;
}

bool TStream::WriteWord (short w)
{
    long bytes = WriteBuffer(&w, 2);
    return bytes==2;
}

bool TStream::WriteUWord(unsigned short w)
{
    long bytes = WriteBuffer(&w, 2);
    return bytes==2;
}

bool TStream::WriteInt(int i)
{
    long bytes = WriteBuffer(&i, 4);
    return bytes==4;
}

bool TStream::WriteUInt (unsigned int i)
{
    long bytes = WriteBuffer(&i, 4);
    return bytes==4;
}

bool TStream::WriteLong (long long i)
{
    long bytes = WriteBuffer(&i, 8);
    return bytes==8;
}

bool TStream::WriteULong (unsigned long long i)
{
    long bytes = WriteBuffer(&i, 8);
    return bytes==8;
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
