/*
 * Persistence Library / Basic types / TXMLTagBasePool 
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

#ifndef TJSONBASEPOOL___H
#define TJSONBASEPOOL___H

#include <stdlib.h>
#include "TJsonTag.h"
#include "TJsonDoc.h"


/**
 *  Class designed for creating instances of XmlTags. It allows to create both
 *  static or dynamic xml tag pool.
 */
class TJsonTagBasePool
{
	friend class TJsonDoc;
	friend class TJsonXml;

protected:

	void				     SetJsonTagName(TJsonTag* tag, const char* tagName);
	void                     SetJsonTagParent(TJsonTag* tag, TJsonTag* tagParent);
	virtual TJsonTag*        GetJsonTag(short i) = 0;
	virtual unsigned short   GetJsonTagCount() = 0;        
    TJsonTagBasePool*        TagPool();

public:

    
	virtual TJsonTag*        CreateJsonTag(TJsonTag* parentTag) = 0;
	virtual bool             DeleteJsonTag(TJsonTag* tag) = 0;
	virtual void             Clear() = 0;

	unsigned short           GetChildCount(TJsonTag* parentTag);
	TJsonTag*                GetChild(TJsonTag* parentTag, unsigned short index);
 
};

#endif
