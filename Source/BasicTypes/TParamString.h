/*
 * Persistence Library / Basic types / TParamString
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

#ifndef TPARAMSTRING___H
#define TPARAMSTRING___H

#include "TString.h"

/**
 *  This class can find multiple parameters in one string.
 *
 *  Example of input string: 
 *
 *  NAME="John";SURNAME="Doe";AGE=30;HEIGHT=180
 *  (value separator=';', name separator='=';)
 *
 *  Another example:
 *
 *  NAME:"John"
 *  SURNAME:"Doe"
 *  AGE:30
 *  HEIGHT:180
 *  (value separator='\n', name separator=':')
 * 
*/
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
		bool GetStringValue(const char* valueName, bool requireQuotas, TString& value);
		bool GetStringValue(const char* valueName, bool requireQuotas, char* buffer, const int bufferLength, int& textLength);

};



#endif
