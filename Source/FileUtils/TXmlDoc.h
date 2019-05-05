/*
 * Persistence Library / Basic types / TXMLDoc
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

#ifndef XMLDOC___H
#define XMLDOC___H

#include "TXmlTag.h"
#include "TXmlTagBasePool.h"
#include "TString.h"


/**
 *  TXMLDoc is xml parser designed for low memory embedded systems.
 *
 *  It makes no dynamic memory allocations. It uses references to 
 *  existing XML buffer. It inserts terminal zeros in input buffer 
 *  and then uses direct references to this buffer.
 *
 *  E.g. buffer is modified like this:
 *
 *  <root>                           <root\0
 *    <name>Karel</name>                <name\0Karel\0/name>
 *    <name>Josef</name>                <name\0Josef\0/name> 
 *    <name>Marcel</name>               <name\0Marcel\0/name>
 *  </root>                          </root> 
 *
 *  Instances of TXmlTags are stored in tag pool. Tags can be also fully
 *  preallocated if you use TXMLTagStaticPool. 
 *
 */

class TXMLDoc
{
    friend class TXMLTag;

	protected:

        TXMLTag*    parserLevel;
		char*       parserData;
        int         parserDataLength;
        bool        parserDataNeedUnalloc;
		int         parserPosition;
        bool        endReached;
       
        char        CurrentChar();
        char        NextChar();
        char        NextNonEmptyChar();
        
        bool        ParseXML();			
        void        RemoveEscapedCharacters(char* value);
       
    public:
        TXMLDoc();
		TXMLDoc(TXMLTagBasePool& tagPool);
		~TXMLDoc();

        void        SetPool(TXMLTagBasePool* tagPool);
        TXMLTagBasePool* TagPool();
        TXMLTag*    Header();
		TXMLTag*    Root();        
		void        Clear();       	
        bool        LoadFromBuffer (char*   rewriteableBuffer, int xmlLength=0);
		bool        LoadFromString (TString &rewriteableString);
        bool        LoadFromFile   (const char*   filename);
		bool		SaveToStream   (TStream& stream);
		bool        SaveToFile     (const char* filename);

        TXMLTag*         SelectNode (const char* xpath);
        TXMLTagList*     SelectNodes(const char* xpath, bool no_alloc=true);
		unsigned short   CountNodes (const char* xpath);
        

};

#endif
