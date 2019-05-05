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

#ifndef JSONDOC___H
#define JSONDOC___H

#include "TJsonTag.h"
#include "TJsonTagBasePool.h"
#include "TString.h"
#include "TStream.h"

/**
 *  TJsonDoc is a json parser designed for low memory embedded systems.
 *
 *  JsonDoc class itself does not make any static or dynamic memory 
 *  allocation. All memory allocations are done by tag pool class.
 *  TXmlTagStaticPool makes static memory allocations only, class
 *  TXmlTagDynamicPool uses dynamic memory allocation.
 *
 *  Json text data must be passed in rewriteable buffer (const char*
 *  data can't be accepted), because this buffer is modified like this:
 *
 *
 *  {                                {
 *    names: ["Karel",                  names\0 ["Karel\0,
 *            "Josef",                           "Josef\0
 *            "Marcel"]                          "Marcel\0
 *  }                                }
 *
 *  Strings like names and values are not reallocated, all string pointers
 *  point to original buffer directly.
 */
class TJsonDoc 
{
    friend class TJsonTag;

protected:

    TJsonTag*   parserLevel;
    char*       parserData;
    int         parserDataLength;
    bool        parserDataNeedUnalloc;
    int         parserPosition;
    bool        endReached;

    char        CurrentChar();
    char        NextChar();
    char        NextNonEmptyChar();

    const char* ParseName();
    bool        ParseValue(TJsonTag* currentTag);
    bool        ParseObject(TJsonTag* currentTag);
    bool        ParseArray(TJsonTag* currentTag);
    bool        ParseNumber(TJsonTag* currentTag);
    bool        ParseString(TJsonTag* currentTag);

    void        RemoveEscapedCharacters(char* value);

public:
    TJsonDoc();
    TJsonDoc(TJsonTagBasePool& tagPool);
    ~TJsonDoc();

    void        SetPool(TJsonTagBasePool* tagPool);
    TJsonTagBasePool* TagPool();
    TJsonTag*   Header();
    TJsonTag*   Root();        
    void        Clear();       	
    bool        LoadFromBuffer (char*   rewriteableBuffer, int jsonLength=-1);
    bool        LoadFromString (TString &rewriteableString);
    bool        LoadFromFile   (const char*   filename);
    bool		SaveToStream   (TStream& stream);
    bool        SaveToFile     (const char* filename);

};


#endif
