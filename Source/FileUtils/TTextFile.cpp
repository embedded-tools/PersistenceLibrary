/*
 * Persistence Library / FileUtils / TStringList
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

#include <stdlib.h>
#include <stdio.h>
#include "ttextfile.h"

TTextFile::TTextFile (int maxLineWidth)
{
    m_fileName = NULL;
    m_fileHandle = 0;
    m_bufferDataCount = 0;
    m_bufferDataMaxCount = maxLineWidth+1;
    m_buffer = (unsigned char*)malloc(m_bufferDataMaxCount);  
    m_bufferIsStatic = false;
}

TTextFile::TTextFile (void* buffer, int bufferSize)
{
    m_fileName = NULL;
    m_fileHandle = 0;
    m_bufferDataCount = 0;
    m_bufferDataMaxCount = bufferSize;
    m_buffer = (unsigned char*) buffer;  
    m_bufferIsStatic = true;
}

TTextFile::~TTextFile ()
{
    if (m_fileHandle)
    {
        Close();
    }
    if (!m_bufferIsStatic)
    {
        if (m_buffer)
        {
            free(m_buffer);
            m_buffer = NULL;
        }
    }
}

bool TTextFile::Open(const char* fileName)
{
    m_fileHandle = fopen(fileName, "rb");
    m_bufferDataCount = 0;
    return   (((long)m_fileHandle)!=0) && 
			 (((long)m_fileHandle)!=-1);
}


void TTextFile::Close()
{
    if (m_fileHandle)
    {
        fclose(m_fileHandle);
        m_fileHandle = 0;
    }    
}

bool TTextFile::AddText(TString& line, long numberOfChars)
{
    long oldLength = line.Length();
	long newLength = line.Length()+numberOfChars;
	if (newLength>=65535)
	{
		newLength=65534;		
	}
	if (!line.SetLength((unsigned short)newLength))
	{
		return false;
	}
	newLength = line.Length();
    long dataToCopy = newLength-oldLength;

	unsigned char* stringBuffer = (unsigned char*)line.ToPChar();
    if (dataToCopy>0)
    {
        memcpy(stringBuffer+oldLength, m_buffer, dataToCopy);
    }    
    return dataToCopy>0;
}

bool TTextFile::ReadLine(TString& line, long maxLineLength)
{    
    bool lfFound = false;
    
	if (m_fileHandle==0) return false;

    line.Clear();
    while(true)
    {
        if ((m_bufferDataCount==0) && feof(m_fileHandle))
        {
            return false;
        }
        if (m_bufferDataCount<m_bufferDataMaxCount)
        {
            m_bufferDataCount+= (long)fread (m_buffer+m_bufferDataCount, 1, m_bufferDataMaxCount-m_bufferDataCount, m_fileHandle);
        }
        for (long i = 0; i<m_bufferDataCount; i++)
        {
            if (m_buffer[i]==10)
            {
                AddText(line, i+1);
                lfFound = true;

                int dataLeft = m_bufferDataCount-i-1;
                memcpy(m_buffer, m_buffer+i+1, dataLeft);
                m_bufferDataCount=dataLeft;
                break;
            }                
        }
        if (lfFound) break;

        AddText(line, m_bufferDataCount);
        m_bufferDataCount = 0;
    }
    return true;
}