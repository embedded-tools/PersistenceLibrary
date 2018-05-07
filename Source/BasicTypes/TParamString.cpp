#include "tparamstring.h"

TParamString::TParamString(const char* text, int textLen, char nameSeparator, char valueSeparator )
: m_text(text, textLen, true, true)
{
    NameSeparator  = nameSeparator;
    ValueSeparator = valueSeparator;
}

TParamString::~TParamString()
{

}

const char* TParamString::FindValue(const char* valueName)
{
	unsigned short position = 0;

	const char* value = NULL;
    int nameLength = 0;
	while(true)
	{
		const char* name = m_text.ToPChar() + position;
        nameLength = 0;
		while(m_text[position++]!=NameSeparator)
		{
            nameLength++;
			if (position>=m_text.Length())
			{
				break;
			}        
		}
		if (strncmp(name, valueName, nameLength)==0)
		{
			value = m_text.ToPChar()+position;
            return value;
		}		

        while(m_text[position++]!=ValueSeparator)
        {
	        if (position>=m_text.Length())
	        {
		        return NULL;
	        }						
        }
    }
		

	return NULL;	
}

bool TParamString::GetBoolValue(const char* valueName, bool& value)
 {
  	const char* text = FindValue(valueName);
	value = false;
	if (text==NULL) return false;
	if (strncmp(text, "false", 5)==0) 
	{
		value = false;
		return true;
	}
	if (strncmp(text, "true", 4)==0) 
	{
		value = true;
		return true;
	}
	return false;
}

bool TParamString::GetIntValue(const char* valueName, int& value)
{
	const char* text = FindValue(valueName);
	value = 0;
	if (text==NULL) return false;

	int n = 0;
	int sgn = 1;
	while(*text==' ')
	{
		text++;
	}
	if (*text=='-')
	{
		sgn = -1;
		text++;
	}
	if ((*text<'0') || (*text>'9')) 
	{			
		return false;
	}
	while(true)
	{
		if ((*text<'0') || (*text>'9')) 
		{			
			break;
		}
		char c = *(text++) - '0';
		n *= 10;
		n += c;        
	}
	n *= sgn;	
    value = n;
	return true;
}

bool TParamString::GetStringValue(const char* valueName, TString& value)
{
	const char* text = FindValue(valueName);
	value = false;
	if (text==NULL) return false;

	char c = 0;
	while(true)
	{
		c = *text++;
		if (c==0) break;
		if (c==ValueSeparator)
		{
			break;
		}	
        value.Append(c);
	}
	return true;
}

bool TParamString::GetStringValue(const char* valueName, char* buffer, const int bufferLength, int& textLength)
{
	const char* text = FindValue(valueName);
	if (text==NULL) return false;

	int i = 0;
	char c = 0;
	while(true)
	{
		c = *text++;
		if (c==0) break;
		if (c==ValueSeparator)
		{
			break;
		}	
		if (i>=bufferLength) return false;
		buffer[i] = c; i++;
	}
    buffer[i] = 0;
	textLength = i;
	return true;
}
