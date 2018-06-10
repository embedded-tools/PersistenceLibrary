#ifndef TJSONTAG___H
#define TJSONTAG___H

#include "TList.h"

class TJsonTag
{
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

        const char*     m_stringValue;        
        float           m_floatValue;        
        bool            m_boolValue;        
        TList<TJsonTag> m_subTags;

    public:
        TJsonTag();
        ~TJsonTag();

       
};

#endif