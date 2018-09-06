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

#include "tjsontag.h"
#include "tjsontagbasepool.h"
#include "tstring.h"
#include "tstream.h"

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