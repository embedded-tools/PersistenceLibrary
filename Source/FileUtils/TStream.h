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

#ifndef TSTREAM___H
#define TSTREAM___H

#include "TString.h"

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


/**
 * Stream is a generic data source or target.
 *
 * Typical scenario is:
 *   1. Open stream (e.g. filestream, memorystream or eepromstream)
 *   2. Write all data or read all data (but never both!)
 *   3. Close the stream
 *
 * TStream is an abstract class for all stream, eeprom stream, memory stream etc.
 * TStream has various method for reading/writing various data types
 * to binary stream.
 *
 * Typical usage:
 *
 * void MyClass::LoadData(TStream& data)  
 * {
 *    data.ReadLong(this->myInt);
 *    data.ReadBinaryText(this->myText);
 *    ...
 * }
 *
 * void MyClass::SaveData(TStream& data)  
 * {
 *    data.WriteLong(this->myInt);
 *    data.WriteBinaryText(this->myText);
 *    ...
 * }
 */
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
    virtual long WriteBuffer  (const void* Buffer, long Count);
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
    bool ReadInt   (int &i);          //32 bit 
    bool ReadUInt  (unsigned int &i); //32 bit	
    bool ReadLong  (long &i);         //64 bit (or 32bit if not supported) 
    bool ReadULong (unsigned long   &i);//64 bit (or 32bit if not supported)
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
    bool WriteInt  (int i);          //32 bit 
    bool WriteUInt (unsigned int i); //32 bit 
    bool WriteLong  (long i);        //64 bit (or 32bit if not supported)
    bool WriteULong (unsigned long   i);//64 bit (or 32bit if not supported)

    bool WriteByteAsText (char  c);
    bool WriteUByteAsText(unsigned char  c);
    bool WriteWordAsText (short w);
    bool WriteUWordAsText(unsigned short w);
    bool WriteLongAsText  (long i);
    bool WriteULongAsText (unsigned long   i);
    bool WriteLine(const char* line);
   
    
};



#endif
