/*
 * Persistence Library / Persistence / TXMLTypes
 *
 * Copyright (c) 2007-2016 Ondrej Sterba <osterba@inbox.com>
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

#ifndef TXMLTYPES___H
#define TXMLTYPES___H

#include "tlist.h"
#include "tcustomstring.h"

class TXMLType
{
	bool IsNumeric;
	bool IsHexadecimal;
	bool CanBeEmpty;
	bool Reserved;
	int  min;
	int  max;
	int  def;
};


class TXMLTypes
{
	private:
		TList<TXMLType> types;
		int	     typesCount;

	public:
		TXMLTypes();

		bool RegisterType(bool isNumeric, bool isHexadecimal, bool canBeEmpty, bool reserved, int min, int max, int def, TCustomString<48> &newName);

};


#endif
