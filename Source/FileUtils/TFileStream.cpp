/*
 * Persistence Library / FileUtils / TFileStream
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

#include "TFileStream.h"

TFileStream::TFileStream(const char* filename, EFileMode fileMode)
{
	switch(fileMode)
	{
		case efmCreate:		m_fileHandle = fopen(filename,"wb"); 						m_canRead=false;
			        	m_canWrite=m_fileHandle!=0;  
                                	m_canSeek=false; 
					break;
		case efmOpenRead:	m_fileHandle = fopen(filename,"rb");
					m_canRead=m_fileHandle!=0;
					m_canWrite=false;
					m_canSeek=m_fileHandle!=0;
					break;
		case efmOpenReadWrite:  m_fileHandle = fopen(filename,"rb+");
					m_canRead=m_fileHandle!=0;
					m_canWrite=m_fileHandle!=0;
					m_canSeek=m_fileHandle!=0;
					break;
		case efmOpenWrite:	m_fileHandle = fopen(filename,"wb");
					m_canRead=false;
					m_canWrite=m_fileHandle!=0;
					m_canSeek=false; 
					break;
		default:		m_fileHandle=NULL;
					m_canRead=false;
					m_canWrite=false;
					m_canSeek=false; 
					break;

    }
}

