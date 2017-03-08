/*
 * Persistence Library / FileUtils / TCachedStream
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

#include "tcachedstream.h"

TCachedStream::TCachedStream(TStream* mainStream, void* cache, int cacheSize)
{
	FParentStream = mainStream;
	FCache = (unsigned char*) cache;
	FCachePosition = 0;	
	FCacheSize = 0;
	FCacheMaxSize = cacheSize;
	FCacheOwned = false;			

	FCanRead = mainStream->CanRead();
	FCanWrite = mainStream->CanWrite();
	FCanSeek = false;

	FPosition = 0;
}

TCachedStream::TCachedStream(TStream* mainStream, int cacheSize)
{
	FParentStream = mainStream;
    if (cacheSize>0)
    {
	    FCache = (unsigned char*) malloc(cacheSize);
        FCacheOwned = true;
    } else {
        FCache = NULL;
        FCacheOwned = false;
    }
	FCachePosition = 0;	
	FCacheSize = 0;
	FCacheMaxSize = cacheSize;	

	FCanRead = mainStream->CanRead();
	FCanWrite = mainStream->CanWrite();
	FCanSeek = false;

	FPosition = 0;
}

TCachedStream::~TCachedStream()
{
	Close();
	if (FCacheOwned)
	{
        if (FCache)
        {
		    free(FCache);
        }
	}
    FCache=NULL;
    FCacheOwned = false;
    FCachePosition = 0;
    FCacheSize = 0;
    FCacheMaxSize = 0;
}

void TCachedStream::Close()
{
	FlushCache();
	if (FParentStream!=NULL)
	{ 
		FParentStream->Close();
        FParentStream = NULL;
	}
}

void TCachedStream::FlushCache()
{
	if (FCanWrite)
	{
		if (FCacheSize>0)
		{
			if (FCacheSize==FCachePosition)
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
   
   if ( (((long)FCachePosition)+dataLength) <= (long)FCacheSize)
   {
      //if all data are inside cache, just copy all data at once
      memcpy(data, FCache+FCachePosition, dataLength);
      FCachePosition+=dataLength;
      return dataLength;     
             
   } else {
      //if all data did not fit to cache, 
      //reads data from more parts 
      int bytesread = 0;
      unsigned char* pDst = (unsigned char*)data;
      while (dataLength>0)
      {
         //if all data from cache has been read...
         if (FCachePosition==FCacheSize)
         {
            //...read the next part
			DoReadOperation();  
            if (FCacheSize==0)
            {
                //"end of file" was reached
                break;
            }
         }

         //count the size of data which are available at the moment
         int templen = dataLength;
         int maxavailable = FCacheSize-FCachePosition;
         if (templen>maxavailable) 
         {
            templen = maxavailable;
         }         
         
         //copy the part of data to destination buffer
         memcpy(pDst, (void*)(FCache+FCachePosition), templen);         
         pDst = pDst + templen;
         dataLength-= templen;        
         bytesread+=templen;         
         FCachePosition+=templen;
      }      
      return bytesread;
   }
}

long TCachedStream::WriteBuffer(void* pData, long dataLength)
{
   if (pData==NULL)
   {
      return 0;
   }

   //current datasize in cache + datalength is lower that maximum cache size
   if ( (((long)FCacheSize)+dataLength) < (long)FCacheMaxSize )
   {
      memcpy(FCache+FCachePosition, pData, dataLength);
      FCachePosition+=dataLength;    
	  FCacheSize    +=dataLength;
      return dataLength;
      
   } else {
      int byteswritten = 0;
      unsigned char* pSrc = (unsigned char*)pData;
      while (dataLength>0)   
      {
		 int maxavailablecache = FCacheMaxSize-FCachePosition;
         int templen = dataLength;
         if (templen>maxavailablecache)
         {
            templen=maxavailablecache;
         }
         memcpy((void*)(FCache+FCachePosition), pSrc, templen);

         pSrc += templen;
         dataLength-=templen;         
         byteswritten+=templen;
         FCachePosition+=templen;
		 FCacheSize+=templen;

         if (FCacheSize==FCacheMaxSize)
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
	if (FParentStream==NULL)
	{
		return false;
	}
	int byteswritten = FParentStream->WriteBuffer((void*)FCache, FCacheSize);

	FPosition += FCacheSize;
	FCachePosition =  0;
	FCacheSize = 0;
	
	bool result = (byteswritten == FCacheMaxSize);   
	return result;
}

bool TCachedStream::DoReadOperation()
{   
	if (FParentStream==NULL)
	{
		return false;
	}
	FPosition += FCacheSize;
	FCacheSize = FParentStream->ReadBuffer((void*)FCache, FCacheMaxSize);
	FCachePosition = 0;

	return true;
}


long TCachedStream::Seek (long Offset, ESeekOrigin Origin)
{
	return 0;
}

long TCachedStream::GetPosition()
{
	return FPosition+FCachePosition;
}

long TCachedStream::GetSize()
{
	return FPosition+FCacheSize;
}

