/*
 * Persistence Library / FileUtils / TCachedStream
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

#include "TCachedStream.h"

TCachedStream::TCachedStream(TStream* mainStream, void* cache, int cacheSize)
{
    m_parentStream = mainStream;
    m_cache = (unsigned char*) cache;
    m_cachePosition = 0;	
    m_cacheSize = 0;
    m_cacheMaxSize = cacheSize;
    m_cacheOwned = false;			

    m_canRead = mainStream->CanRead();
    m_canWrite = mainStream->CanWrite();
    m_canSeek = false;

    m_position = 0;
}

TCachedStream::TCachedStream(TStream* mainStream, int cacheSize)
{
    m_parentStream = mainStream;
    if (cacheSize>0)
    {
	    m_cache = (unsigned char*) malloc(cacheSize);
        m_cacheOwned = true;
    } else {
        m_cache = NULL;
        m_cacheOwned = false;
    }
    m_cachePosition = 0;	
    m_cacheSize = 0;
    m_cacheMaxSize = cacheSize;	

    m_canRead = mainStream->CanRead();
    m_canWrite = mainStream->CanWrite();
    m_canSeek = false;

    m_position = 0;
}

TCachedStream::~TCachedStream()
{
	Close();
	if (m_cacheOwned)
	{
        if (m_cache)
        {
		    free(m_cache);
        }
	}
    m_cache=NULL;
    m_cacheOwned = false;
    m_cachePosition = 0;
    m_cacheSize = 0;
    m_cacheMaxSize = 0;
}

void TCachedStream::Close()
{
	FlushCache();
	if (m_parentStream!=NULL)
	{ 
		m_parentStream->Close();
        m_parentStream = NULL;
	}
}

void TCachedStream::FlushCache()
{
	if (m_canWrite)
	{
		if (m_cacheSize>0)
		{
			if (m_cacheSize==m_cachePosition)
			{
				DoWriteOperation();
			}
		}
	}
}

long TCachedStream::ReadBuffer (void* data, long dataLength)
{
    /*
	if (FCacheSize==0)
	{
        //fill the cache if cache is empty
		bool res = DoReadOperation();	
        if (!res)
        { 
            return 0;
        }
	}*/

   if (dataLength<1) 
   {
      //avoid to call memcpy with dataLength=0
      return 0;
   }
   
   if ( (((long)m_cachePosition)+dataLength) <= (long)m_cacheSize)
   {
      //if all data are inside cache, just copy all data at once
      memcpy(data, m_cache+m_cachePosition, dataLength);
      m_cachePosition+=dataLength;
      return dataLength;     
             
   } else {
      //if all data did not fit to cache, 
      //reads data from more parts 
      int bytesread = 0;
      unsigned char* pDst = (unsigned char*)data;
      while (dataLength>0)
      {
         //if all data from cache has been read...
         if (m_cachePosition==m_cacheSize)
         {
            //...read the next part
			DoReadOperation();  
            if (m_cacheSize==0)
            {
                //"end of file" was reached
                break;
            }
         }

         //count the size of data which are available at the moment
         int templen = dataLength;
         int maxavailable = m_cacheSize-m_cachePosition;
         if (templen>maxavailable) 
         {
            templen = maxavailable;
         }         
         
         //copy the part of data to destination buffer
         memcpy(pDst, (void*)(m_cache+m_cachePosition), templen);         
         pDst = pDst + templen;
         dataLength-= templen;        
         bytesread+=templen;         
         m_cachePosition+=templen;
      }      
      return bytesread;
   }
}

long TCachedStream::WriteBuffer(const void* pData, long dataLength)
{
   if (pData==NULL)
   {
      return 0;
   }

   //current datasize in cache + datalength is lower that maximum cache size
   if ( (((long)m_cacheSize)+dataLength) < (long)m_cacheMaxSize )
   {
      memcpy(m_cache+m_cachePosition, pData, dataLength);
      m_cachePosition+=dataLength;    
	  m_cacheSize    +=dataLength;
      return dataLength;
      
   } else {
      int byteswritten = 0;
      unsigned char* pSrc = (unsigned char*)pData;
      while (dataLength>0)   
      {
		 int maxavailablecache = m_cacheMaxSize-m_cachePosition;
         int templen = dataLength;
         if (templen>maxavailablecache)
         {
            templen=maxavailablecache;
         }
         memcpy((void*)(m_cache+m_cachePosition), pSrc, templen);

         pSrc += templen;
         dataLength-=templen;         
         byteswritten+=templen;
         m_cachePosition+=templen;
		 m_cacheSize+=templen;

         if (m_cacheSize==m_cacheMaxSize)
         {
			bool res = DoWriteOperation();
			if (res==false)
            {
               return 0;
            }
         }               
      }         
      return byteswritten;      
   }
}

bool TCachedStream::DoWriteOperation()
{   
	if (m_parentStream==NULL)
	{
		return false;
	}
	int byteswritten = m_parentStream->WriteBuffer((void*)m_cache, m_cacheSize);

	m_position += m_cacheSize;
	m_cachePosition =  0;
	m_cacheSize = 0;
	
	bool result = (byteswritten == m_cacheMaxSize);   
	return result;
}

bool TCachedStream::DoReadOperation()
{   
	if (m_parentStream==NULL)
	{
		return false;
	}
	m_position += m_cacheSize;
	m_cacheSize = m_parentStream->ReadBuffer((void*)m_cache, m_cacheMaxSize);
	m_cachePosition = 0;

	return true;
}


long TCachedStream::Seek (long Offset, ESeekOrigin Origin)
{
	return 0;
}

long TCachedStream::GetPosition()
{
	return m_position+m_cachePosition;
}

long TCachedStream::GetSize()
{
	return m_position+m_cacheSize;
}

