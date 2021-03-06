/*
 * Persistence Library / Basic types / TXMLTag
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

#ifndef XMLTAG___H
#define XMLTAG___H

#include <stdlib.h>
#include "TShortString.h"
#include "TStream.h"

#define XMLPATHMAXLENGTH 80
#define XMLTAGPOOLFOOTPRINT (sizeof(TXMLTag)*XMLTAGPOOLSIZE)

class TXMLTagBasePool;
class TXMLTag;

class TXMLTagList 
{
private:
    TXMLTag*       iteratedParent;
    const char*    iteratedName;
    unsigned short iteratorIndex;

protected:
	TXMLTagList();

public:
    TXMLTagList(TXMLTag* parent, const char* name=NULL);
    TXMLTag* First();
    TXMLTag* Next();
};


/**
 *  TXMLTag is used by TXmlDoc. TXMLTag does not allocate nor frees any memory.
 *  It just holds pointers to main input xml buffer.
 *
 *	Also offers methods to access xml tag properties and iterating xml sub tags.
 *  
 *  Methods SelectNode and SelectNodes use simplified version of xpath.
 *  It is simplified a lot - you can use just this format:
 *      Tag/SubTag/.../SubTag (no asterisks and brackets are supported yet)
 */
class TXMLTag
{
    friend class TXMLTagBasePool;
	friend class TXMLTagList;
    friend class TXMLDoc;

    protected:        

        const char*		Name;				
        const char*		Value;		
        const char*		FirstAttribute;
        const char*		LastAttribute;
        TXMLTag*		ParentTag;

        static TXMLTagBasePool* TagPool;
		static void	SetTagPool(TXMLTagBasePool* tagPool);

		void        Init();		
        TXMLTag*    FindIterationParent(TXMLTag* parentTag, const char* xpath);
        const char* ExtractChildName (const char* xpath);
		
    public:				

		TXMLTag();
		~TXMLTag();			

		void             Clear();
               
        const char*		 GetName();
        TShortString	 GetNameAsString();
        const char*		 GetValue();
        TShortString	 GetValueAsString();
        short			 GetValueAsShortInt(short defaultValue=0);
        long			 GetValueAsLongInt(long defaultValue=0);
        const char*		 GetAttributeValue(const char* attributeName);
        TShortString	 GetAttributeValueAsString(const char* attributeName);
        short			 GetAttributeValueAsShortInt(const char* attributeName, short defaultValue=0);
        long			 GetAttributeValueAsLongInt(const char* attributeName, long defaultValue=0);
        TXMLTag*         GetParentTag();
        short            GetChildCount();
        TXMLTag*         GetChild(short i);
        TXMLTag*         SelectNode(const char* xpath);
        TXMLTagList*     SelectNodes(const char* xpath, bool no_malloc=true);
		unsigned short   CountNodes (const char* xpath);

		void			 SaveToStream(TStream& stream, int indent = 0);
       
};

#endif
