#ifndef TPARAMSTRING___H
#define TPARAMSTRING___H

#include "tstring.h"

class TParamString
{
	private:
		TString m_text;

		const char* FindValue(const char* valueName);

	public:
		TParamString(const char* text, int textLen=0, char nameSeparator='=', char valueSeparator=';' );
		~TParamString();

		char NameSeparator;
		char ValueSeparator;

		bool GetBoolValue(const char* valueName, bool& value);
		bool GetIntValue(const char* valueName, int& value);
		bool GetStringValue(const char* valueName, TString& value);
		bool GetStringValue(const char* valueName, char* buffer, const int bufferLength, int& textLength);

};



#endif