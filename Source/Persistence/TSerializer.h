/*
 * Persistence Library / Persistence / TSerializer
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

#ifndef SERIALIZATIONPARAMS____H
#define SERIALIZATIONPARAMS____H

#include "txmldoc.h"
#include "txmltag.h"
#include "tstream.h"
#include "ESerializationMode.h"
#include "TXMLTypes.h"
#include "TXMLCache.h"
#include "TXMLParser.h"
                
enum EErrorCode
{     
	eecNone = 0,
	eecIncorrectBinaryHeader,
	eecIncorrectBinarySerialization
};
                                                                                                                                                                                      
class TSerializer
{        
   protected:
              
	public:		
		static  void Init();

		static	TStream*			DataStream;		
		static	TXMLCache*			XMLCache;
		static	TXMLParser*			XMLParser;

		static	ESerializationMode	Mode;  
		static	bool				IncrementalChanges;		
		static	unsigned short		TimeStamp;
		static  unsigned char       ErrorCode;
	   	   	      	   	
};

#endif
