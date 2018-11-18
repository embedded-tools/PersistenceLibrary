#include "TJsonTag.h"
#include "TJsonTagBasePool.h"

extern "C"
{
    #include "EscapeSequences.h"
};



TJsonTagList::TJsonTagList()
{
    iteratedParent = NULL;
    iteratorIndex = 0;
}

TJsonTagList::TJsonTagList(TJsonTag* parent)
{
    iteratedParent = parent;
    iteratorIndex = 0;
}

TJsonTag* TJsonTagList::First()
{
    iteratorIndex = 0;
    unsigned short n = TJsonTag::TagPool->GetChildCount(iteratedParent);
    for(unsigned short i = 0; i<n; i++)
    {
        TJsonTag* tag = TJsonTag::TagPool->GetChild(iteratedParent,i);
        return tag;
    }
    return NULL;
}

TJsonTag* TJsonTagList::Next()
{
    iteratorIndex++;

    unsigned short counter = iteratorIndex;
    unsigned short n = TJsonTag::TagPool->GetChildCount(iteratedParent);
    for(unsigned short i = 0; i<n; i++)
    {
        TJsonTag* tag = TJsonTag::TagPool->GetChild(iteratedParent,i);
        if (counter == 0)
        {
            return tag;
        }
        counter--;		
    }
    return NULL;
}

TJsonTagBasePool*  TJsonTag::TagPool = NULL;

TJsonTag::TJsonTag()
  : m_parentTag(NULL),
    m_name(NULL),
    m_stringValue(NULL),
    m_intValue(0),
    m_tagType(JSONNull)  
{

}

TJsonTag::~TJsonTag()
{

}

void TJsonTag::SetTagPool(TJsonTagBasePool* tagPool)
{
	TagPool = tagPool;
}

void TJsonTag::Clear()
{

}

TJsonTag::TagType TJsonTag::GetType()
{
    return m_tagType;
}

const char*	TJsonTag::GetName()
{
	return m_name;
}

TShortString TJsonTag::GetNameAsString()
{
	return m_name;
}

const char*	TJsonTag::GetValue()
{
	return m_stringValue;
}

TShortString TJsonTag::GetValueAsString()
{
	return m_stringValue;
}

short TJsonTag::GetValueAsShortInt(short defaultValue)
{
    if (m_tagType!=JSONNumber)
    {
        return defaultValue;
    }
    if (m_intValue>32767)
    {
        return defaultValue;
    }
    if (m_intValue<-32768)
    {
        return defaultValue;
    }
    return (short)m_intValue;
}

long TJsonTag::GetValueAsLongInt(long defaultValue)
{
    if (m_tagType!=JSONNumber)
    {
        return defaultValue;
    }
	return m_intValue;
}

bool TJsonTag::GetValueAsBoolean()
{
    return m_intValue == 1;
}

TJsonTag* TJsonTag::GetParentTag()
{
	return m_parentTag;
}

void TJsonTag::WriteCRLF(TStream& stream)
{
    stream.WriteBuffer("\r\n", 2);
}

void TJsonTag::WriteSpaces(TStream& stream, int indent)
{
    const char* spaces = "          "\
                         "          "\
                         "          "\
                         "          "\
                         "          "\
                         "          "\
                         "          "\
                         "          "\
                         "          "\
                         "          ";
    int spacesCount = indent * 4 ;
    if (spacesCount>100) spacesCount = 100;
    stream.WriteBuffer(spaces, spacesCount);
}

void TJsonTag::SaveToStream(TStream& stream, int indent)
{
    switch(m_tagType)
    {
        case JSONString:    
             {
                stream.WriteBuffer("\"", 1);
                int len = (int)strlen(m_stringValue);

                bool escCharsNeeded = false;
                for(int i = 0; i<len; i++)
                {
                    if (m_stringValue[i]<32)
                    {
                        escCharsNeeded = true;
                    }
                }
                if (escCharsNeeded)
                {
                    for(int i = 0; i<len; i++)
                    {
                        char c = m_stringValue[i];
                        char escChar;
                        if (GetEscapedChar(c, &escChar))
                        {
                            stream.WriteChar('\\');
                            stream.WriteChar(escChar);
                        } else {
                            stream.WriteChar(c);
                        }
                    }
                } else {
                    stream.WriteBuffer(m_stringValue, len); 
                }

                stream.WriteBuffer("\"", 1);
             }
             break;
        case JSONNumber:    stream.WriteLongAsText(m_intValue); break;
        case JSONBoolean:   stream.WritePlainText(m_stringValue); break;
        case JSONNull:      stream.WritePlainText("null"); break;
        case JSONArray:     
            {                               
                int n = TagPool->GetChildCount(this);
                bool arrayOfObjects = false;
                if (n>0)
                {
                    TJsonTag* first = TagPool->GetChild(this, 0);
                    if (first)
                    {
                        if (first->m_tagType==JSONObject)
                        {
                            arrayOfObjects = true;
                        }
                    }
                }
                if (arrayOfObjects)
                {
                    WriteCRLF(stream);
                    WriteSpaces(stream, indent);
                }                
                stream.WritePlainText("[");
                for(int i = 0; i<n; i++)
                {
                    if (i>0) 
                    {
                        stream.WritePlainText(", ");
                    }
                    TJsonTag* tag = TagPool->GetChild(this, i);
                    tag->SaveToStream(stream, indent+1);                    
                }
                if (arrayOfObjects)
                {
                    WriteCRLF(stream);
                    WriteSpaces(stream, indent);
                }                
                stream.WritePlainText("]");
            }
            break;
        case JSONObject:    
            {        
                if (indent>0)
                {
                    WriteCRLF(stream);
                }
                WriteSpaces(stream, indent);
                stream.WritePlainText("{");
                                
                int n = TagPool->GetChildCount(this);
                for(int i = 0; i<n; i++)
                {
                    TJsonTag* tag = TagPool->GetChild(this, i);
                    WriteCRLF(stream);
                    WriteSpaces(stream, indent+1);
                    stream.WritePlainText(tag->GetName());
                    stream.WritePlainText(": ");
                    tag->SaveToStream(stream, indent+1);
                    if (i!=n-1)
                    {
                        stream.WritePlainText(",");
                    }                    
                }
                WriteCRLF(stream);
                WriteSpaces(stream, indent);
                stream.WritePlainText("}");
            }
            break;
    }
}
