/*
 * Persistence Library / FileUtils / TStream
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

#ifndef TSTREAM___H
#define TSTREAM___H

#include "tstring.h"

#define EReadError      0x0000001
#define EWriteError     0x0000002
#define EMemStreamError 0x0000003
#define EFCreateError   0x0000004
#define EFOpenError     0x0000005

typedef enum 
{	
	esoFromBeginning =0x0000,
	esoFromCurrent   =0x0001,
	esoFromEnd       =0x0002
} ESeekOrigin;

class TStream
{
private:
protected:

    TStream* m_parentStream;
	bool m_canRead;
	bool m_canWrite;
	bool m_canSeek;

public:
    TStream();

    virtual void Close();
    virtual long ReadBuffer   (void* Buffer, long Count);
    virtual long WriteBuffer  (void* Buffer, long Count);
    virtual long Seek (long Offset, ESeekOrigin Origin)=0;
	virtual long GetPosition()=0;
	virtual long GetSize()=0;

	bool CanRead() { return m_canRead; };
	bool CanWrite() { return m_canWrite; };
	bool CanSeek() { return m_canWrite; };

	bool Eof();

    bool ReadBinaryText(TString* text, short maxLength);
    bool ReadBinaryText(char* buffer, unsigned short bufferSize);
    bool ReadChar (signed char  &c);
    bool ReadByte (unsigned char  &c);
    bool ReadWord (short &w);
    bool ReadUWord(unsigned short &w);
    bool ReadLong  (long &i);
    bool ReadULong (unsigned long   &i);	
    bool ReadLine(TString& line);	

    bool WriteBinaryText(TString* text);
    bool WriteBinaryText(const char* text);    
	bool WritePlainText(TString* text);
	bool WritePlainText(const char* text);
	bool WriteXMLEncodedText(TString* text);
	bool WriteXMLEncodedText(const char* text);
    bool WriteChar (signed char  c);
    bool WriteByte (unsigned char  c);
    bool WriteWord (short w);
    bool WriteUWord(unsigned short w);
    bool WriteLong  (long i);
    bool WriteULong (unsigned long   i);

    bool WriteByteAsText (char  c);
    bool WriteUByteAsText(unsigned char  c);
    bool WriteWordAsText (short w);
    bool WriteUWordAsText(unsigned short w);
    bool WriteLongAsText  (long i);
    bool WriteULongAsText (unsigned long   i);
    bool WriteLine(const char* line);
   
    
};



#endif
