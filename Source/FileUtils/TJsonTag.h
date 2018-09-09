#ifndef TJSONTAG___H
#define TJSONTAG___H

#include "TList.h"
#include "TShortString.h"
#include "TStream.h"

class TJsonTagBasePool;
class TJsonTag;

class TJsonTagList 
{
private:	
    TJsonTag*      iteratedParent;
    unsigned short iteratorIndex;

protected:
    TJsonTagList();

public:
    TJsonTagList(TJsonTag* parent);
    TJsonTag* First();
    TJsonTag* Next();
};

/**
 *  TJsonTag represents one json element (string, boolean, array, object or null)
 *
 *  TJsonDoc class itself does not make any static or dynamic memory 
 *  allocation.
 *
 */
class TJsonTag
{
	friend class TJsonTagBasePool;
	friend class TJsonTagList;
	friend class TJsonDoc;

    public:
        enum TagType
        {
            JSONString,
            JSONNumber,
            JSONBoolean,
            JSONNull,
            JSONArray,
            JSONObject
        };

    private:		
        TagType         m_tagType;
		TJsonTag*		m_parentTag;
		const char*		m_name;		
	    const char*     m_stringValue;
        int             m_intValue;

        void WriteCRLF(TStream& stream);
        void WriteSpaces(TStream& stream, int indent);
        
    public:
		TJsonTag();
        ~TJsonTag();

		void             Clear();
        TagType          GetType();
		const char*		 GetName();
		TShortString	 GetNameAsString();
		const char*		 GetValue();
		TShortString	 GetValueAsString();
		short			 GetValueAsShortInt(short defaultValue=0);
		long			 GetValueAsLongInt(long defaultValue=0);
        bool    		 GetValueAsBoolean();

		TJsonTag*		 GetParentTag();

        static TJsonTagBasePool*  TagPool;
        static void	SetTagPool(TJsonTagBasePool* tagPool);
       
        void SaveToStream(TStream& stream, int indent = 0);

};

#endif